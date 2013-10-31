#pragma once

namespace NeuralNetwork
{
class InputData
{
public:
	/*InputData(const int DataNum)
	{
	}*/
	void AddValue(const double Value)
	{
		m_vValue.push_back(Value);
	}

	const vector<double>& GetValues() const { return m_vValue; }
private:
	vector<double> m_vValue;
};

class OutputData
{
public:
	/*OutputData(const int DataNum)
	{
	}*/

	void AddValue(const double Value)
	{
		m_vValue.push_back(Value);
	}

	const vector<double>& GetValues() const { return m_vValue; }
private:
	vector<double> m_vValue;
};

class LearningData
{
public:
	LearningData() : m_Input(NULL), m_Output(NULL) {}
	~LearningData()
	{
		delete m_Input;
		delete m_Output;
	}
	void SetLearningData(const InputData* Input, const OutputData* Output)
	{ 
		m_Input = Input;
		m_Output = Output;
	}
	const InputData* GetInputData() const { return m_Input; }
	const OutputData* GetOutputData() const { return m_Output; }

private:
	const InputData*	m_Input;
	const OutputData*	m_Output;
};
}