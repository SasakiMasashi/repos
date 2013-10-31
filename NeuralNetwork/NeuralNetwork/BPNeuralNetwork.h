#pragma once
#include <string>
#include <vector>
using namespace std;

namespace NeuralNetwork
{
class LearningData;
class InputData;
class OutputData;
class Layer;

class BPNeuralNetwork
{
public:
	BPNeuralNetwork();
	~BPNeuralNetwork();
	void AddLayer(const string& Name, const int NodeSize);
	void Learn(LearningData& Data);
	void Run(const InputData* Input);
	void BackPropagation(const OutputData* CorrectOutput);
	double GetOutputValue() const ;
	const vector<Layer*>& GetAllLayer() const;

private:
	string m_Name;
	vector<Layer*> m_vLayer;
};

}