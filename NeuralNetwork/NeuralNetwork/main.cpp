

#include <conio.h>
#include <iostream>
#include "BPNeuralNetwork.h"
#include "LearningData.h"
#include "TestLearn.h"
using namespace std;
using namespace NeuralNetwork;

int main(int argc, char* argv[])
{
	BPNeuralNetwork BP;
	BP.AddLayer("Input", 2);
	BP.AddLayer("Hidden", 3);
	BP.AddLayer("Output", 1);

	const int LearnNum = 50000;
	TestCase1::Learn(LearnNum, BP);

	TestCase1::Run(BP, 0.0, 0.0);
	TestCase1::Run(BP, 0.0, 1.0);
	TestCase1::Run(BP, 1.0, 0.0);	
	TestCase1::Run(BP, 1.0, 1.0);

	_getch();
	return 0;
}
