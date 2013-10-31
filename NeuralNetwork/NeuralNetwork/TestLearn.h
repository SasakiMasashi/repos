#pragma once

namespace NeuralNetwork
{
	class BPNeuralNetwork;
	namespace TestCase1
	{
		void Learn(const int LearnNum, BPNeuralNetwork& BP);
		void Run(BPNeuralNetwork& BP, const double Input1, const double Input2);
	}
}
