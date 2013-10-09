#include "TestLearn.h"
#include "BPNeuralNetwork.h"
#include "LearningData.h"
#include "Dump.h"
#include <iostream>
using namespace std;
namespace NeuralNetwork
{
	namespace TestCase1
	{
	static void CreateInputData1(LearningData& Data)
	{
		InputData*	Input = new InputData();
		Input->AddValue(1.0);
		Input->AddValue(0.0);
		OutputData*	Output = new OutputData();
		Output->AddValue(1.0);
		Data.SetLearningData(Input, Output);
	}

	static void CreateInputData2(LearningData& Data)
	{
		InputData*	Input = new InputData();
		Input->AddValue(0.0);
		Input->AddValue(0.0);
		OutputData*	Output = new OutputData();
		Output->AddValue(0.0);
		Data.SetLearningData(Input, Output);
	}

	static void CreateInputData3(LearningData& Data)
	{
		InputData*	Input = new InputData();
		Input->AddValue(1.0);
		Input->AddValue(1.0);
		OutputData*	Output = new OutputData();
		Output->AddValue(0.0);
		Data.SetLearningData(Input, Output);
	}

	static void CreateInputData4(LearningData& Data)
	{
		InputData*	Input = new InputData();
		Input->AddValue(0.0);
		Input->AddValue(1.0);
		OutputData*	Output = new OutputData();
		Output->AddValue(1.0);
		Data.SetLearningData(Input, Output);
	}

	//#define DUMP

	void
	Learn(const int LearnNum, BPNeuralNetwork& BP)
	{
		const string FileName = "test.txt";

		cout << "LearnStart-LearnNum: " << LearnNum << endl;
		for(int i = 0; i < LearnNum; i++)
		{
			//cout << "\t: " << i << "\n";

#ifdef DUMP
			Dump(BP, FileName);
#endif

			// data1
			{
				LearningData Data;
				CreateInputData1(Data);
				BP.Learn(Data);
			}

#ifdef DUMP
			Dump(BP, FileName);
#endif

			// data2
			{
				LearningData Data;
				CreateInputData2(Data);
				BP.Learn(Data);
			}

#ifdef DUMP
			Dump(BP, FileName);
#endif
			// data3
			{
				LearningData Data;
				CreateInputData3(Data);
				BP.Learn(Data);
			}

#ifdef DUMP
			Dump(BP, FileName);
#endif
			// data4
			{
				LearningData Data;
				CreateInputData4(Data);
				BP.Learn(Data);
			}
		}
		cout << "LearnEnd" << endl;
	}

	void Run(BPNeuralNetwork& BP, const double Input1, const double Input2)
	{
		cout << "TestRun----------" << endl;
		InputData Input;
		Input.AddValue(Input1);
		cout << "\tInput1: " << Input1 << endl;
		Input.AddValue(Input2);
		cout << "\tInput2: " << Input2 << endl;
		BP.Run(&Input);
		double Result = BP.GetOutputValue();
		cout << "Output: " << Result << endl;
	}
	}
}