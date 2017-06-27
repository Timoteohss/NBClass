 #include <iostream>
#include <vector>
#include "NB.h"

int main()
{
	NB class1("class1");
	NB class2("class2");
	NB total(class1, class2, "tudoMisturado");

	class1.calcProb(total);
	class2.calcProb(total);

	int nOfTestDocs = 0;
	int corrClass = 0;
	float accuracy = 0.0f;
	std::cout << "Entre o numero de documentos" << std::endl;
 	std::cin >> nOfTestDocs;

	NB test("teste", 1);
	if (test.totalProb(class1, total) >= test.totalProb(class2, total))
	{
		std::cout << "O teste pertence a classe 1" << std::endl;
		if (test.tipoClasse == 1)
		{
			corrClass++;
			accuracy = (float)corrClass / nOfTestDocs;
			std::cout << "A precisao eh de " << accuracy << std::endl;
		}
	}
	else
	{
		std::cout << "O teste pertence a classe 2" << std::endl;
		if (test.tipoClasse == 1)
		{
			corrClass++;
			accuracy = (float)corrClass / nOfTestDocs;
			std::cout << "A precisao eh de " << accuracy << std::endl;
		}
	}

	std::cin.ignore();
	system("pause");
	return 0;
}