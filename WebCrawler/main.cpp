

#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <map>
#include <fcntl.h>	
#include <io.h>	
//#include <boost/regex.hpp>
#include <conio.h>
#include <iostream>

//#include <sstream>
#include "DocumentID.h"
#include "Link.h"
#include "MD5Hash.h"
#include "URL.h"
#include "DocumentContents.h"
#include "FileUtility.h"
#include "WebDocumentDecomposer.h"
#include "SeedURLLoader.h"
using namespace std;

class CrawledSaveData
{
public:
	CrawledSaveData(const URL& url, const string& WebDocumentFilePath)
		:m_URL(url), 
		m_WebDocumentFilePath(WebDocumentFilePath)
	{
	}

public:
	URL		getURL() const { return m_URL; }
	string	getWebDocumentFilePath() const { return m_WebDocumentFilePath; }
	
private:
	URL		m_URL;
	string	m_WebDocumentFilePath;
};

class Spider
{
public:
private:

};

class DocumentContentsTable
{
public:
	vector<DocumentContents> m_vDocContents;
};


class DocumentIDTable
{
public:
	class DocIdAndUrl
	{
	public:
		DocIdAndUrl(const DocumentID& docID, const URL& url)
			:DocID(docID),
			Url(url)
		{
		}
		DocumentID	DocID;
		URL			Url;
	};

	void add(const URL& url)
	{
		string hashValue = MD5HashFunc::getHashValue(url.get());
		DocIdAndUrl pair(DocumentID(hashValue), URL(url.get()));
		m_vDocID.push_back(pair);
	}

private:
	vector<DocIdAndUrl> m_vDocID;
};

class LinkTable
{
public:
private:
};

class VisitedURLTable
{
public:
	void add(const DocumentID& docID, const URLPointer& url)
	{
		if(isVisited(docID))
		{
			return;
		}

		m_UrlMap[docID] = url;
	}

	bool isVisited(const DocumentID& docID)
	{
		map<DocumentID, URLPointer>::iterator it = m_UrlMap.begin();
		for(; it != m_UrlMap.end(); it++)
		{
			if(it->first.get() == docID.get())
			{
				return true;
			}
		}

		return false;
	}
private:
	map<DocumentID, URLPointer> m_UrlMap;
};

static string
getOutputFilePath(const string& OutputFolderPath, const string& url)
{
	string hashValue = MD5HashFunc::getHashValue(url);
	return OutputFolderPath + "\\" + hashValue + ".html";
}

int main(int argc, char* argv[])
{
	if(argc != 7)
	{
		return -1;
	}

	_setmode(_fileno(stdout), _O_BINARY);
	_setmode(_fileno(stderr), _O_BINARY);
	setlocale(LC_ALL,"");
	
	const string SeedURLFilePath		= string(argv[1]);	// SeedFilePath	
	const int CrawlNum					= atoi(argv[2]);	// CrawlするWebの数
	const string SaveFolderPath			= string(argv[3]);	// Web保存先フォルダパス
	
	const string OutputTextFolderPath	= string(argv[4]);
	const string OutputLinkFilePath		= string(argv[5]);
	const string OutputURLFilePath		= string(argv[6]);

	const string LogFilePath			= SaveFolderPath + "\\Log.txt";

	// SeedURL
	queue<URLPointer> qURL;
	SeedURLLoader::Load(SeedURLFilePath, qURL);

	// 巡回したURLとその保存先の一覧
	VisitedURLTable visitedURL;

	int Counter = 0;
	while(Counter < CrawlNum)
	{
		if(qURL.empty())
		{
			cout << "探索先がなくなりました。" << endl;
			_getch();
			return 0;
		}

		Sleep(1000);

		// URLを取得
		URLPointer url = qURL.front();
		qURL.pop();
		
		const DocumentID docID(MD5HashFunc::getHashValue(url->get()));
		if(visitedURL.isVisited(docID))
		{
			continue;
		}
		Counter++;

		// 指定したURLをDocumentIDをファイル名として保存
		string OutputFilePath = getOutputFilePath(SaveFolderPath, url->get());
		FileUtil::SaveHttpFile(OutputFilePath, url->get(), LogFilePath);
		
		visitedURL.add(docID, url);
		
		// 保存したWeb DocumentからContentsを抽出
		WebDocumentDecomposer Decomposer(url);
		Decomposer.Decompose(OutputFilePath);

		// Contextsの出力
		Decomposer.OutputContents(OutputTextFolderPath, OutputLinkFilePath, OutputURLFilePath);

		// URLを取得
		vector<URLPointer> extractedURLs;
		Decomposer.GetExtractedURLs(extractedURLs);
		for(size_t i = 0; i < extractedURLs.size(); i++)
		{
			qURL.push(extractedURLs[i]);
		}
	}

	const string OutputFolder = OutputLinkFilePath.substr(0, OutputLinkFilePath.rfind("\\"));
	const string OutputFilePath = OutputFolder + "\\Link_GraphViz.dot";
	ofstream fout(OutputFilePath.c_str(), ios::app);
	fout << "}" << endl;

	cout << "Counter分収集完了" << endl;
	_getch();

	return 0;
}