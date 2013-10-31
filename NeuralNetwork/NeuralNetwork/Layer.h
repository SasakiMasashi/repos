#pragma once
#include <vector>
using namespace std;

namespace NeuralNetwork
{
class Node;

class Layer
{
public:
	Layer();
	~Layer();
	Layer*		GetPreviousLayer();
	Layer*		GetNextLayer();
	const vector<Node*>& GetAllNodes()	const;
	void		AddNode(const int NodeNum) ;

private:
	vector<Node*>	m_Nodes;
	Layer*		m_PreviousLayer;
	Layer*		m_NextLayer;
};
}