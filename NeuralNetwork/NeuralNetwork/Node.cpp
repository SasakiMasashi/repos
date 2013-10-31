#include "Node.h"
#include "NodeRelation.h"
#include <math.h>
namespace NeuralNetwork
{

Node::Node() 
: m_LimitValue(0.0), 
m_Value(0.0),
m_PreviousRelations(),
m_NextRelations()
{

}

set<Node*>	
Node::GetAllPreviouNodes()
{	
	set<Node*> PreviousNodes;
	set<NodeRelation*>::const_iterator it = m_PreviousRelations.begin();
	for(; it != m_PreviousRelations.end(); it++)
	{
		PreviousNodes.insert((*it)->GetPreviousNode());
	}
	return PreviousNodes;
}

set<Node*>	
Node::GetAllNextNodes()
{
	set<Node*> NextNodes;
	set<NodeRelation*>::const_iterator it = m_NextRelations.begin();
	for(; it != m_NextRelations.end(); it++)
	{
		NextNodes.insert((*it)->GetNextNode());
	}
	return NextNodes;
}
	
set<NodeRelation*>& 
Node::GetPreviousNodeRelations()	
{ 
	return m_PreviousRelations; 
}

set<NodeRelation*>& 
Node::GetNextNodeRelations()		
{ 
	return m_NextRelations; 
}

void	
Node::AddPreivousRelation(NodeRelation* PreviousRelation)
{
	m_PreviousRelations.insert(PreviousRelation);
}

void	
Node::AddNextRelation(NodeRelation* NextRelation)
{
	m_NextRelations.insert(NextRelation);
}

void	
Node::SetValue(const double Value)	
{ 
	m_Value = Value; 
}

double 
Node::GetValue() const					
{ 
	return m_Value; 
}

namespace
{
	double Convey(const double Input)
	{
		return 1.0 / (1.0 + exp(-Input));
	}
}

void 
Node::CalcOutput()
{
	m_Value = 0.0;
	set<NodeRelation*>::const_iterator it = m_PreviousRelations.begin();
	for(; it != m_PreviousRelations.end(); it++)
	{
		Node* PreviousNode = (*it)->GetPreviousNode();
		double PreviousValue = PreviousNode->GetValue();
		double Weight = (*it)->GetWeight();
		m_Value += PreviousValue * Weight;
	}
	m_Value = Convey(m_Value);
}

double 
Node::GetNextWeightedDeleta()
{
	double Result = 0.0;
	set<NodeRelation*>::iterator it = m_NextRelations.begin();
	for(; it != m_NextRelations.end(); it++)
	{
		Result += (*it)->GetDelta() * (*it)->GetWeight();
	}
	return Result;
}

}