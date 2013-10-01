#include <vector>
#include <string>
#include <map>
#include <fstream>
//#include "DocumentID.h"
using namespace std;

class Node;
typedef Node* Link;
class GraphHandler;

class Node
{
public:
	explicit Node(const string& docID)
		:m_DocID(docID),
		 m_PreRank(0.0),
		 m_Rank(0.0),
		 m_TeleportaionRate(0.15),
		 m_RandomLinkProb(0.0)
	{
	}

	void addInLink(Link& Link)
	{
		m_vInLink.push_back(Link);
	}

	void addOutLink(Link& Link)
	{
		m_vOutLink.push_back(Link);
	}

	int getInLinkNum() const
	{
		return (int)m_vInLink.size();
	}

	int getOutLinkNum() const
	{
		return (int)m_vOutLink.size();
	}

	void setDefaultRank(const double rank)
	{
		m_PreRank = rank;
		m_Rank = rank;
	}

	double getTeleportationRate() const { return m_TeleportaionRate;}

	void setRandomLinkProb(const double randomLinkProb)
	{
		m_RandomLinkProb = randomLinkProb;
	}

	void setTeleportationRate(const double teleportationRate)
	{
		m_TeleportaionRate = teleportationRate;
	}

	void addRank(const double plusRank)
	{
		m_Rank += plusRank;
	}

	void calcPageRank()
	{
		double rankSum = 0.0;
		for(size_t i = 0; i < m_vInLink.size(); i++)
		{
			rankSum += m_vInLink[i]->getPreRank() / m_vInLink[i]->getOutLinkNum();
		}
		rankSum *= (1.0 - m_TeleportaionRate);
		rankSum += m_TeleportaionRate * m_RandomLinkProb;

		setRank(rankSum);
	}

	string getDocumentID() const { return m_DocID; }
	double	getRank() const { return m_Rank; }
	double	getPreRank() const { return m_PreRank; }
	void	setRank(const double rank) { m_Rank = rank; }
	void	generationShift() { m_PreRank = m_Rank; }

private:
	double m_PreRank;
	double m_Rank;
	double m_RandomLinkProb;
	double m_TeleportaionRate;
	string m_DocID;
	vector<Link> m_vInLink;
	vector<Link> m_vOutLink;
};

class Graph
{
public:
	typedef map<string, Node*> NodeMap;
	friend class GraphHandler;

	~Graph()
	{
		for(size_t i = 0; i < m_vNode.size(); i++)
		{
			delete m_vNode[i];
			m_vNode[i] = NULL;
		}
		m_vNode.clear();
	}

	void addNode(Node* node) 
	{
		m_vNode.push_back(node);
		m_NodeMap[node->getDocumentID()] = node;
	}

	bool existNode(const string& docID)
	{
		return (m_NodeMap.end() != m_NodeMap.find(docID));
	}

	Node* getNode(const string& docID)
	{
		NodeMap::iterator it = m_NodeMap.find(docID);
		if(m_NodeMap.end() != it)
		{
			return it->second;
		}
		return NULL;
	}

	int getNodeNum() const 
	{
		return (int)m_vNode.size();
	}

	


private:
	NodeMap m_NodeMap;
	vector<Node*> m_vNode;
};

class GraphHandler
{
public:
	explicit GraphHandler(Graph& graph)
		:m_Graph(graph)
	{
	}

	void setDefaultRank(const double rank)
	{
		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			m_Graph.m_vNode[i]->setDefaultRank(rank);
		}
	}

	void setTeleportationRate(const double teleportationRate)
	{
		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			m_Graph.m_vNode[i]->setTeleportationRate(teleportationRate);
		}
	}

	void setRandomLinkProb(const double randomLinkProb)
	{
		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			m_Graph.m_vNode[i]->setRandomLinkProb(randomLinkProb);
		}
	}

	int Create(const string& InputFilePath)
	{
		ifstream fin(InputFilePath.c_str());
		if(!fin)
		{
			return -1;
		}

		const string fromSt = "from: ";
		const string toSt	= "to: ";

		Node* fromNode = NULL;
		Node* toNode = NULL;

		while(!fin.eof())
		{
			string Line;
			getline(fin, Line);
			if(Line.empty())
			{
				continue;
			}

			string docID;
			if(string::npos != Line.find(fromSt))
			{
				docID = Line.substr(fromSt.size());
				if(m_Graph.existNode(docID))
				{
					fromNode = m_Graph.getNode(docID);
				}
				else
				{
					fromNode = new Node(docID);
					m_Graph.addNode(fromNode);
				}
			}

			if(string::npos != Line.find(toSt))
			{
				docID = Line.substr(toSt.size());
				if(m_Graph.existNode(docID))
				{
					toNode = m_Graph.getNode(docID);
				}
				else
				{
					toNode = new Node(docID);
					m_Graph.addNode(toNode);
				}
				fromNode->addOutLink(toNode);
				toNode->addInLink(fromNode);
			}
		}
		return 0;
	}

	double getPreVanishRank()
	{
		double sum = 0.0;
		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			if(0 == m_Graph.m_vNode[i]->getOutLinkNum())
			{
				sum += (1.0 - m_Graph.m_vNode[i]->getTeleportationRate()) * m_Graph.m_vNode[i]->getPreRank();
			}
		}
		return sum / (double)m_Graph.m_vNode.size();
	}

	void setPreVanishRankSum(const double vanishValue)
	{
		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			m_Graph.m_vNode[i]->addRank(vanishValue);
		}
	}

	void calcPageRank(const int roundNum)
	{
		int counter = 0;
		while(counter < roundNum)
		{
			generationShift();
			calcPageRank();
			setPreVanishRankSum(getPreVanishRank());

			counter++;
		}
	}

	void Output(const string& OutputFolderPath, const string& URLListFilePath)
	{
		// URLリストの読込
		ifstream fin(URLListFilePath.c_str());
		if(!fin)
		{
			return;
		}

		map<string, string> docID2UrlMap;
		while(!fin.eof())
		{
			string Line;
			getline(fin, Line);
			if(Line.empty())
			{
				continue;
			}

			std::string::size_type spos = Line.find("\t");
			std::string::size_type epos = Line.rfind("\t");
			if(string::npos == spos)
			{
				continue;
			}
			string url = Line.substr(0, spos);
			string docID = Line.substr(epos + 1);
			if(docID2UrlMap.end() == docID2UrlMap.find(docID))
			{
				docID2UrlMap.insert(pair<string, string>(docID, url));
			}
		}

		// PageRank結果の出力
		const string& OutputFilePath = OutputFolderPath + "\\PageRankResult.txt";
		ofstream fout(OutputFilePath.c_str());
		if(!fout)
		{
			return;
		}

		fout << "documentID \tURL \t PageRank \t InLinkNum \t OutLinkNum \n";

		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			string docID = m_Graph.m_vNode[i]->getDocumentID();
			string url;
			map<string, string>::iterator it = docID2UrlMap.find(docID);
			if(docID2UrlMap.end() != it)
			{
				url = it->second;
			}

			fout << docID << "\t" 
				 << url << "\t" 
				 << m_Graph.m_vNode[i]->getRank() << "\t"
				 << m_Graph.m_vNode[i]->getInLinkNum() << "\t"
				 << m_Graph.m_vNode[i]->getOutLinkNum() << "\n";
		}
	}

private:
	void generationShift() const
	{
		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			m_Graph.m_vNode[i]->generationShift();
		}
	}

	void calcPageRank() const
	{
		for(size_t i = 0; i < m_Graph.m_vNode.size(); i++)
		{
			m_Graph.m_vNode[i]->calcPageRank();
		}
	}

private:
	Graph& m_Graph;
};

int main(int argc, char* argv[])
{
	if(argc != 5)
	{
		return -1;
	}

	const string InputLinkFilePath = string(argv[1]);
	const int roundNum = atoi(argv[2]);
	const string OutputFolderPath = string(argv[3]);
	const string URLListFilePath = string(argv[4]);

	// グラフの生成
	Graph graph;
	GraphHandler Handler(graph);
	Handler.Create(InputLinkFilePath);

	// 初期値をセット
	const double inverseGlobalValue = 1.0 / (double)(graph.getNodeNum());
	Handler.setDefaultRank(inverseGlobalValue);
	Handler.setRandomLinkProb(inverseGlobalValue);

	double teleportationRate = 0.15;
	Handler.setTeleportationRate(teleportationRate);

	// PageRankの計算	
	Handler.calcPageRank(roundNum);

	// 計算結果を出力
	Handler.Output(OutputFolderPath, URLListFilePath);

	return 0;
}