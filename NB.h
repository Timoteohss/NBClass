#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <atomic>
#include <map>

class NB
{
public:
	NB(NB& cl1, NB& cl2, std::string nomeClasse);
	NB(std::string nomeClasse);
	NB(std::string nomeClasse, int tipoClasse);
	std::vector<std::string> combinaTexto();
	void bagOfWords(std::string combinaClasse, bool total = false);
	void calcProb(NB& total);
	float totalProb(NB& prob, NB& total);
	int tipoClasse;

private:
	int i_tam = 0;
	float f_prob = 1.0f;
	std::atomic<int> i_voc = 0;
	int i_nClasse = 0;
	int i_tClasse = 0;
	int i_nPalavrasClasse = 0;
	int i_contPalavra = 0;
	bool b_pronto = false;
	std::ifstream file_in;
	std::ofstream file_out;
	std::string s_nomeClasse;
	std::string s_nomeArquivo;
	std::vector<std::string> vs_combinado;
	std::map<std::string, std::string> mss_categoria;
	std::map<std::string, int> msi_bow;
	std::map<std::string, float> msf_probCalc;

};