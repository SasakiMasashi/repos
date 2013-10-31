#include "Layer.h"
#include "Node.h"

namespace NeuralNetwork
{

Layer::Layer()
:m_PreviousLayer(NULL),
m_NextLayer(NULL),
m_Nodes()
{
}

Layer::~Layer()
{
	for(size_t i = 0; i < m_Nodes.size(); i++)
		delete m_Nodes[i];
	m_Nodes.clear();
}

Layer*
Layer::GetPreviousLayer()	
{ 
	return m_PreviousLayer; 
}

Layer*		
Layer::GetNextLayer()		
{ 
	return m_NextLayer; 
}

const vector<Node*>& 
Layer::GetAllNodes()	const	
{ 
	return m_Nodes; 
}
	
void		
Layer::AddNode(const int NodeNum) 
{
	for(int i = 0; i < NodeNum; i++)
	{
		Node* node = new Node();
		m_Nodes.push_back(node);
	}
}
}