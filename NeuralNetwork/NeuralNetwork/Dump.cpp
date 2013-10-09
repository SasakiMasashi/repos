#include "Dump.h"
#include "BPNeuralNetwork.h"
#include <iostream>
#include "Node.h"
#include "NodeRelation.h"
#include "Layer.h"
#include <fstream>
using namespace std;

namespace NeuralNetwork
{
	void Dump(const BPNeuralNetwork& BP, const string& FilePath)
	{
		ofstream fout(FilePath.c_str(), ios::app);
		if(!fout)
			return;

		fout << "+++++++++++++++++++++++" << endl;
		fout << "NodeValue:" << endl;
		const vector<Layer*>& vLayer = BP.GetAllLayer();
		for(size_t i = 0; i < vLayer.size(); i++)
		{
			fout << "\tLayer: " << i << endl;
			const vector<Node*>& vNode = vLayer[i]->GetAllNodes();
			for(size_t j = 0; j < vNode.size(); j++)
			{
				fout << "\t\tNodeValue: " << vNode[j]->GetValue() << endl;
			}
		}

		fout << "RelationWeight:" << endl;
		for(size_t i = 0; i < vLayer.size() - 1; i++)
		{
			fout << "\tLayer " << i << " to " << i + 1 << endl;
			const vector<Node*>& vNode = vLayer[i]->GetAllNodes();
			for(size_t j = 0; j < vNode.size(); j++)
			{
				const set<NodeRelation*>& Relations = vNode[j]->GetNextNodeRelations();
				set<NodeRelation*>::const_iterator cit = Relations.begin();
				for(; cit != Relations.end(); cit++)
				{
					fout << "\t\tWeight: " << (*cit)->GetWeight() << " , Delta: " << (*cit)->GetDelta() << endl;
				}
			}
		}
	}
}