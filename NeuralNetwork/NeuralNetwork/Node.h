#pragma once
#include <set>
using namespace std;

namespace NeuralNetwork
{
class NodeRelation;

class Node
{
public:
	Node();
	set<Node*>	GetAllPreviouNodes();
	set<Node*>	GetAllNextNodes();	
	set<NodeRelation*>& GetPreviousNodeRelations();
	set<NodeRelation*>& GetNextNodeRelations();
	void	AddPreivousRelation(NodeRelation* PreviousRelation);
	void	AddNextRelation(NodeRelation* NextRelation);
	void	SetValue(const double Value);
	double	GetValue() const;
	void CalcOutput();
	double GetNextWeightedDeleta();

private:
	double m_LimitValue;
	double m_Value;
	set<NodeRelation*> m_PreviousRelations;
	set<NodeRelation*> m_NextRelations;
};
}