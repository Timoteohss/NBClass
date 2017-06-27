#include "NB.h"
#include <cmath>
#include <omp.h>
#include <cassert>
#include <algorithm>
#include <iostream>


NB::NB(std::string nomeClasse, int tipoClasse)
{
	NB::tipoClasse = tipoClasse;
	s_nomeClasse = nomeClasse;
	std::cout << "Entre com um arquivo: " + s_nomeClasse << std::endl;
	std::cin >> s_nomeArquivo;
	mss_categoria[s_nomeArquivo] = s_nomeClasse;
	combinaTexto();
	bagOfWords(s_nomeClasse + ".txt");
}

NB::NB(NB& cl1, NB& cl2, std::string nomeClasse)
{
	s_nomeClasse = nomeClasse;
	file_out.open("misturaTudo.txt");
	assert(file_out.is_open() == true);

	auto comb1 = cl1.combinaTexto();
	i_tam = comb1.size();
	for (int i = 0; i < i_tam; i++) {
		vs_combinado.push_back(comb1[i]); //OK
		std::cout << comb1[i];
	}
	
	i_tam = cl2.combinaTexto().size();
	auto comb2 = cl2.combinaTexto();
	for (int i = 0; i < i_tam; i++) {
		vs_combinado.push_back(comb2[i]); //OK
		std::cout << comb2[i];
	}
	
	i_tam = vs_combinado.size();

	for (int i = 0; i < i_tam; i++) {
		file_out << vs_combinado[i] << std::endl;
		std::cout << i + 1 << ". " << vs_combinado[i] << std::endl;
	}
	
	file_out.close();
	i_tClasse = cl1.i_tClasse + cl2.i_tClasse;
	bagOfWords("misturaTudo.txt", true);
	std::cout << "acabou!" << std::endl;
}

NB::NB(std::string nomeClasse)
{
	s_nomeClasse = nomeClasse;
	int documento = 1;
	while (b_pronto == false) {
		std::cout << "Nome do documento[" << documento << "]: " + s_nomeClasse << std::endl;
		documento++;
		std::cin >> s_nomeArquivo;
		if (s_nomeArquivo != "q") {
			mss_categoria[s_nomeArquivo] = s_nomeClasse;
			i_nClasse++;
			i_tClasse++;
		}
		else {
			b_pronto = true;
		}
	}
	combinaTexto();
	bagOfWords(s_nomeClasse + ".txt");
}

std::vector<std::string> NB::combinaTexto() {

	std::string temp;
	std::string classCombine = s_nomeClasse + ".txt";
	std::vector <std::string> tmp;
	std::map<std::string, std::string>::iterator it;

	//file_out.open(classCombine);
	//assert(file_out.is_open() == true);

	for (it = mss_categoria.begin(); it != mss_categoria.end(); it++) {
		file_in.open(it->first + ".txt");
		assert(file_in.is_open() == true);

		while (file_in >> temp) {
			file_out << temp << std::endl;
			tmp.push_back(temp);
		}
		
		file_in.close();
	}
	file_out.close();
	return tmp;
}

void NB::bagOfWords(std::string classCombine, bool isTotal)
 {

	std::map<std::string, int>::iterator it;
	std::string temp;
	std::vector<std::string> tp;
	std::string name = s_nomeClasse + "_bow.txt";
	int tam;

	file_in.open(classCombine);
	//assert(file_in.is_open() == true);

	file_out.open(name);
	assert(file_out.is_open() == true);

	while (file_in >> temp) {
		tp.push_back(temp);
	}

	tam = tp.size();
	std::vector<int> count(tam, 1);
	bool found;
	std::vector<std::string> temporario;

#pragma omp parallel for collapse(2)
	{
		for (int i = 0; i < tam; i++) {
			for (int j = 0; j < (tam - i - 1); j++) {
				if (tp[i] == tp[j + i + 1] && (j + i + 1) < tam) {
						count[i]++;
						std::cout << tp[i] << ":" << count[i] << std::endl;						
				}
			}
		}
	}

	for (int i = tam - 1; i >= 0; i--) {
		if (count[i] > 20) //AQUIIIIIIIII
		{
			msi_bow[tp[i]] = count[i];
			std::cout << msi_bow[tp[i]];
		}
	}

	for (it = msi_bow.begin(); it != msi_bow.end(); it++) {
		file_out << it->first << ": " << it->second << std::endl;
		std::cout << it->first << ": " << it->second << std::endl;
	}
	std::cout << std::endl;

	if (isTotal == true) {
		for (it = msi_bow.begin(); it != msi_bow.end(); it++) {
			i_voc += 1;
			std::cout << i_voc << std::endl;
		}
	}
	else {
		for (it = msi_bow.begin(); it != msi_bow.end(); it++) {
			i_nPalavrasClasse += it->second;
		}
		std::cout << i_nPalavrasClasse << std::endl;
	}

	std::cout << "wut" << std::endl;
	file_in.close();
	file_out.close();
}

void NB::calcProb(NB& total) {

	std::map<std::string, int> ::iterator it;
	std::map<std::string, int> ::iterator it2;
	std::map<std::string, float> ::iterator it3;

	file_out.open(s_nomeClasse + "_prob.txt");
	assert(file_out.is_open() == true);


		for (it = total.msi_bow.begin(); it != total.msi_bow.end(); it++) {
			for (it2 = msi_bow.begin(); it2 != msi_bow.end(); it2++) {
				if (it->first == it2->first) {
					msf_probCalc[it->first] = (float)((it2->second) + 1) / (i_nPalavrasClasse + total.i_voc);
					std::cout << msf_probCalc[it->first] << std::endl;
					break;
				}
				else {
					msf_probCalc[it->first] = (float)(1) / (i_nPalavrasClasse + total.i_voc);
					std::cout << msf_probCalc[it->first] << std::endl;
				}
			}
		}


	for (it3 = msf_probCalc.begin(); it3 != msf_probCalc.end(); it3++) {
		//std::cout << it3->first << ": " << it3->second << std::endl;
		file_out << it3->first << ": " << it3->second << std::endl;
	}
	file_out.close();
}

float NB::totalProb(NB& prob, NB& total) {

	std::map<std::string, int> ::iterator it;
	std::map<std::string, int> ::iterator it2;
	std::map<std::string, float> ::iterator it3;

	file_out.open(s_nomeClasse + "_" + prob.s_nomeClasse + "_prob.txt");
	if (file_out.fail()) {
		perror("cannot write to");
	}
	f_prob = 1.0f;

		for (it = msi_bow.begin(); it != msi_bow.end(); it++) {
			for (it3 = prob.msf_probCalc.begin(); it3 != prob.msf_probCalc.end(); it3++) {
				if (it->first == it3->first) {
					i_contPalavra = 0;
					f_prob = f_prob + log(pow((it3->second), (it->second)));
					break;
				}
				else {
					i_contPalavra++;
					if (i_contPalavra == prob.msf_probCalc.size()) {
						f_prob = f_prob + log(1.0f / (prob.i_nPalavrasClasse + total.i_voc));
					}
				}
			}
		}
	f_prob = f_prob + log((float)(prob.i_nClasse) / total.i_tClasse); //log aqui
	std::cout << f_prob << std::endl;
	file_out << "A probabilidade de  " << s_nomeClasse << " pertencer a " << prob.s_nomeClasse << " eh: " << f_prob << std::endl;
	file_out.close();
	return f_prob;
}
