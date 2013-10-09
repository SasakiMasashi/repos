#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "BPNeuralNetwork.h"
#include "Layer.h"
#include "LearningData.h"
#include "Node.h"
#include "NodeRelation.h"

namespace NeuralNetwork
{
BPNeuralNetwork::BPNeuralNetwork()
:m_Name(),
m_vLayer()
{
	srand((unsigned) time(NULL));
}

BPNeuralNetwork::~BPNeuralNetwork()
{
	for(size_t i = 0; i < m_vLayer.size(); i++)
	{
		const vector<Node*>& vNode = m_vLayer[i]->GetAllNodes();
		for(size_t j = 0; j < vNode.size(); j++)
		{
			set<NodeRelation*>& Relations = vNode[j]->GetNextNodeRelations();
			set<NodeRelation*>::iterator it = Relations.begin();
			for(; it != Relations.end(); it++)
				delete (*it);
			Relations.clear();
		}
		delete m_vLayer[i];
	}
	m_vLayer.clear();
}

void
BPNeuralNetwork::AddLayer(const string& Name, const int NodeSize)
{
	m_Name = Name;
	Layer* layer = new Layer();
	layer->AddNode(NodeSize);
	if(!m_vLayer.empty())
	{
		Layer* PreviousLayer = m_vLayer.back();
		vector<Node*> AllPreviousNodes = PreviousLayer->GetAllNodes();
		vector<Node*> AllCurrentNodes = layer->GetAllNodes();
		vector<Node*>::const_iterator pit = AllPreviousNodes.begin();
		for(; pit != AllPreviousNodes.end(); pit++){
			vector<Node*>::const_iterator cit = AllCurrentNodes.begin();
			for(; cit != AllCurrentNodes.end(); cit++){
				NodeRelation* Relation = new NodeRelation(*pit, *cit);
				(*pit)->AddNextRelation(Relation);
				(*cit)->AddPreivousRelation(Relation);
			}
		}
	}
	m_vLayer.push_back(layer);
}

void 
BPNeuralNetwork::Learn(LearningData& Data)
{
	// é¿çs
	const InputData* Input = Data.GetInputData();

	const vector<double>& vValue = Input->GetValues();
	//for(size_t i = 0; i < vValue.size(); i++)
	//	cout << "\t\tInput: " << i + 1 << " : " << vValue[i] << endl;
	Run(Input);

	//cout << "\t\tOutput: " << GetOutputValue() << endl;
	
	// åÎç∑ãtì`îd
	const OutputData* CorrectOutput = Data.GetOutputData();
	BackPropagation(CorrectOutput);
}

void 
BPNeuralNetwork::Run(const InputData* Input)
{
	const vector<double>& vValue = Input->GetValues();
	Layer* layer = m_vLayer.front();
	if(!layer)
		return;

	const vector<Node*>& vInputNode = layer->GetAllNodes();
	if(vInputNode.size() != vValue.size())
		return;

	for(size_t i = 0; i < vInputNode.size(); i++)
	{
		vInputNode[i]->SetValue(vValue[i]);
	}

	for(size_t i = 1; i < m_vLayer.size(); i++)
	{
		const vector<Node*>& vNode = m_vLayer[i]->GetAllNodes();
		for(size_t j = 0; j < vNode.size(); j++)
		{
			vNode[j]->CalcOutput();
		}
	}
}

void 
BPNeuralNetwork::BackPropagation(const OutputData* CorrectOutput)
{
	const vector<double>& vCorrectOutput = CorrectOutput->GetValues();
	Layer* LastLayer = m_vLayer.back();
	vector<Node*> vLastNode = LastLayer->GetAllNodes();
	if(vCorrectOutput.size() != vLastNode.size())
		return;

	for(size_t i = 0; i < vLastNode.size(); i++)
	{
		set<NodeRelation*> PreviousNodeRelations = vLastNode[i]->GetPreviousNodeRelations();
		set<NodeRelation*>::iterator it = PreviousNodeRelations.begin();
		for(; it != PreviousNodeRelations.end(); it++)
		{
			double Delta = vLastNode[i]->GetValue() * ( 1 - vLastNode[i]->GetValue()) * (vCorrectOutput[i] - vLastNode[i]->GetValue());
			(*it)->SetDelta(Delta);
			double NewWeight = (*it)->GetWeight() + Delta * (*it)->GetPreviousNode()->GetValue();
			(*it)->SetWeight(NewWeight);
		}
	}

	for(int i = (int)(m_vLayer.size()) - 2; i > 0; i--)
	{
		vector<Node*> vNode = m_vLayer[i]->GetAllNodes();
		for(size_t j = 0; j < vNode.size(); j++)
		{
			set<NodeRelation*>& vPreviousRelation = vNode[j]->GetPreviousNodeRelations();
			set<NodeRelation*>::iterator it = vPreviousRelation.begin();
			for(; it != vPreviousRelation.end(); it++)
			{
				double Delta = vNode[j]->GetValue() * ( 1 - vNode[j]->GetValue() ) * vNode[j]->GetNextWeightedDeleta();
				(*it)->SetDelta(Delta);
				double NewWeight = (*it)->GetWeight() + Delta * (*it)->GetPreviousNode()->GetValue();
				(*it)->SetWeight(NewWeight);
			}
		}
	}
}

double 
BPNeuralNetwork::GetOutputValue() const 
{
	Layer* LastLayer = m_vLayer.back();
	const vector<Node*>& vLastNode = LastLayer->GetAllNodes();
	if(vLastNode.size() != 1)
		return -1.0;

	Node* node = vLastNode.front();
	return node->GetValue();
}

const vector<Layer*>& 
BPNeuralNetwork::GetAllLayer() const 
{ 
	return m_vLayer; 
}

}