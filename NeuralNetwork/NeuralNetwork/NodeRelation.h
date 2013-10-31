#pragma once
#include "Node.h"

namespace NeuralNetwork
{
	class Node;

	class NodeRelation
	{
	public:
		NodeRelation(Node* const PreviousNode, Node* const NextNode) 
			: m_PreviousNode(PreviousNode), m_NextNode(NextNode)
		{
			m_Weight = rand() / (RAND_MAX + 0.1);
		}

		Node*	GetPreviousNode()	{ return m_PreviousNode; }
		Node*	GetNextNode()		{ return m_NextNode; }
		double	GetWeight()			{ return m_Weight; }
		void	SetWeight(const double Weight) { m_Weight = Weight; }
		double	GetDelta()			{ return m_Delta; }
		void	SetDelta(const double Delta) { m_Delta = Delta; }

	private:
		double	m_Delta;
		double	m_Weight;
		Node* const	m_PreviousNode;
		Node* const m_NextNode;
	};
}