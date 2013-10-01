#pragma once
#include "URL.h"
#include "DocumentContents.h"
#include "MD5Hash.h"


class WebDocumentDecomposer
{
public:
	explicit WebDocumentDecomposer(const URLPointer& url)
	{
		m_ParentURL = url;
	}

	void Decompose(string SavedWebDoc)
	{
		ifstream fin(SavedWebDoc.c_str());
		if(!fin)
		{
			return;
		}

		while(!fin.eof())
		{
			string Line;
			getline(fin, Line);
			if(!Line.empty())
			{
				// URL抽出
				vector<string> vHttpMatched;
				ExtractURLString(Line, vHttpMatched);
				if(!vHttpMatched.empty())
				{
					for(size_t i = 0; i < vHttpMatched.size(); i++)
					{
						URLPointer url = URLPointer(new URL(vHttpMatched[i]));
						url->setParentURL(m_ParentURL);
						m_vURL.push_back(url);
					}
				}

				// Title抽出
				Title title;
				ExtractTitleString(Line, title);
				m_Contents.setTitle(Title(title));

				// Text抽出
				string text;
				ExtractTextString(Line, text);
				m_Contents.addText(text);
			}
		}


		for(size_t i = 0; i < m_vURL.size(); i++)
		{
			if(ExistBase())
			{
				m_vURL[i]->setBaseFilePath(m_BaseString);
			}
			else
			{
				m_vURL[i]->setBaseFilePath(m_ParentURL->get());
			}
		}

		// Linkの構成と登録
		vector<Link> vLink;
		ConstructLink(vLink);
		m_Contents.setLinks(vLink);

		// URL登録
		m_Contents.setURL(URL(m_ParentURL->get()));

		// DocumentID登録
		m_Contents.setDocumentID(DocumentID(MD5HashFunc::getHashValue(m_ParentURL->get())));
	}

	void GetExtractedURLs(vector<URLPointer>& urls)
	{
		urls = m_vURL;
	}

	void OutputContents(const string& OutputTextFolderPath,
						const string& OutputLinkFilePath,
						const string& OutputURLFilePath)
	{
		// Textの出力
		OutputText(OutputTextFolderPath);		

		// リンクの出力
		OutputLink(OutputLinkFilePath);
		OutputLinkForGraphViz(OutputLinkFilePath);

		// URLの出力
		OutputURL(OutputURLFilePath);
			
	}

private:

	void OutputText(const string& OutputTextFolderPath)
	{
		if(OutputTextFolderPath.empty())
		{
			return;
		}

		const DocumentID& docID = m_Contents.getDocumentID();
		const string FilePath = OutputTextFolderPath + "\\" + docID.get() + ".txt";

		ofstream fout(FilePath.c_str());
		if(!fout)
		{
			return;
		}

		fout << m_Contents.getText().get() << "\n";
	}

	void OutputLink(const string& OutputLinkFilePath)
	{
		if(OutputLinkFilePath.empty())
		{
			return;
		}

		ofstream fout(OutputLinkFilePath.c_str(), ios::app);
		if(!fout)
		{
			return;
		}
		
		const vector<Link>& vLink = m_Contents.getLinks();
		if(vLink.empty())
		{
			return;
		}

		fout << "<link-start>\n";
		fout << "from: " << vLink.front().getFromDocID().get() << "\n";
		for(size_t i = 0; i < vLink.size(); i++)
		{
			fout << "to: " << vLink[i].getToDocID().get() << "\n";
		}
		fout << "<link-end>\n";
	}

	void OutputLinkForGraphViz(const string& OutputLinkFilePath)
	{
		const string OutputFolder = OutputLinkFilePath.substr(0, OutputLinkFilePath.rfind("\\"));
		const string OutputFilePath = OutputFolder + "\\Link_GraphViz.dot";

		ofstream fout(OutputFilePath.c_str(), ios::app);
		if(!fout)
		{
			return;
		}

		const vector<Link>& vLink = m_Contents.getLinks();
		if(vLink.empty())
		{
			return;
		}

		static int counter = 0;
		counter++;

		if(1 == counter)
		{
			fout << "digraph Link {\n";
			fout << "	node [width=0.05 height=0.05 fixedsize=true label=\"\"];\n";
			fout << "	edge [arrowsize=0.05 style=\"setlinewidth(0.5)\"];\n";
		}

		static vector<string> vDocID;

		for(size_t i = 0; i < vLink.size(); i++)
		{
			int FromID = -1;
			int ToID = -1;

			// 敢えて逐次検索
			for(size_t j = 0; j < vDocID.size(); j++)
			{
				// FromのID
				if(0 == vDocID[j].compare(vLink[i].getFromDocID().get()))
				{
					FromID = (int)j;
				}

				// ToのID
				if(0 == vDocID[j].compare(vLink[i].getToDocID().get()))
				{
					ToID = (int)j;
				}
			}

			if(-1 == FromID)
			{
				vDocID.push_back(vLink[i].getFromDocID().get());
				FromID = (int)vDocID.size() - 1;
			}

			if(-1 == ToID)
			{
				vDocID.push_back(vLink[i].getToDocID().get());
				ToID = (int)vDocID.size() - 1;
			}

			fout << "\t" << FromID << " -> " << ToID << ";\n";
		}		
	}

	void OutputURL(const string& OutputURLFilePath)
	{
		if(OutputURLFilePath.empty())
		{
			return;
		}

		ofstream fout(OutputURLFilePath.c_str(), ios::app);
		if(!fout)
		{
			return;
		}

		const URL& url			= m_Contents.getURL();
		const DocumentID& DocID = m_Contents.getDocumentID();

		fout << DocID.get() << "\t\t" << url.get() << "\n";
	}

	void ConstructLink(vector<Link>& vLink) const 
	{
		vLink.clear();
		const DocumentID FromDocID(MD5HashFunc::getHashValue(m_ParentURL->get()));

		for(size_t i = 0; i < m_vURL.size(); i++)
		{
			const DocumentID ToDocID(MD5HashFunc::getHashValue(m_vURL[i]->get()));
			vLink.push_back(Link(FromDocID, ToDocID));
		}
	}

	void ExtractTitleString(const string& Line, Title& title)
	{
		// タグの表現の仕方は多数あるので以下だけだとまずい→要変更
		const string StartTitleTag		= "<title>";
		const string EndTitleTag		= "</title>";
		
		static bool startTitle = false;
		static bool endTitle = false;
		static string text;

		if(0 == Line.compare(StartTitleTag))
			startTitle = true;
		
		if(0 == Line.compare(EndTitleTag))
			endTitle = true;
		
		if(startTitle && endTitle)
		{
			std::string::size_type startPos = Line.find(StartTitleTag);
			std::string::size_type endPos = Line.find(EndTitleTag);

			// 開始タグと終了タグが同時に存在する
			if(std::string::npos != startPos && std::string::npos != endPos)
			{
				text = Line.substr(startPos + StartTitleTag.size(), endPos - startPos - StartTitleTag.size());
			}
			else if(std::string::npos != startPos && std::string::npos == endPos)
			{
				text += Line.substr(0, endPos);
			}
			else if(std::string::npos == startPos && std::string::npos != endPos)
			{
				// エラー
			}
			else
			{
				// 既にタイトルは抽出済み
			}
		}
		else if(startTitle && !endTitle)
		{
			std::string::size_type pos = Line.find(StartTitleTag);
			if(std::string::npos != pos)
			{
				text += Line.substr(pos + StartTitleTag.size());				
			}
			else
			{
				text += Line;
			}
		}
		else if(!startTitle && endTitle)
		{
			//エラー
		}
		else
		{
			// いまだタイトルが見つからない。
		}
		title = Title(text);
	}

	// タグにはさまれた全ての文字列を抽出する
	void ExtractTextString(const string& Line, string& text)
	{
		// state一覧
		// -1:未分類
		//  0: startTagの中
		//  1: Text位置
		//  2: endTagの中

		if(Line.empty())
		{
			return;
		}

		stringstream ssResult;

		static int state = -1;

		string::size_type spos = Line.find("<");
		string::size_type epos = Line.find(">");

		// 状態は変化しない
		if(spos == string::npos && epos == string::npos) // "<"も">"も存在しない
		{			
			if(1 == state)
			{
				text = Line;
			}
			return;
		}
		else if(spos != string::npos && epos == string::npos) // "<"が存在
		{
			// 一つの行に"<"だけが複数存在することはありえない。あるとすれば一つ(テキスト中のは無視）
			// またtagの中にtagが来ることはありえない
			if(1 == state)
			{
				if(0 != spos)
				{
					text = Line.substr(0, spos);
				}

				if(spos != (int)Line.size() - 1)
				{
					if('/' != Line.at(spos + 1))
					{
						state = 0;
					}
					else
					{
						state = 2;
					}
				}
				else
				{
					state = 0;
				}
			}
			return;
		}
		else if(spos == string::npos && epos != string::npos) // ">"が存在
		{
			// 一つの行に">"だけが複数存在することはありえない。あるとすれば一つ(テキスト中のは無視）
			// またtagの中にtagが来ることはありえない
			if(0 == state || 2 == state)
			{
				if(epos != (int)Line.size() - 1)
				{
					text = Line.substr(epos + 1);
				}
				state = 1;
			}
			return;	
		}
		else // "<"も">"も存在
		{
			for(size_t i = 0; i < Line.size(); i++)
			{
				if('<' == Line.at(i))
				{
					if(i == Line.size() - 1)
					{
						state = 0;
					}
					else
					{
						if('/' == Line.at(i + 1))
						{
							state = 2;
						}
						else
						{
							state = 0;
						}
					}
					continue;
				}
				else if('>' == Line.at(i))
				{
					state = 1;
					continue;
				}

				if(1 == state)
				{
					ssResult << Line.at(i);
				}
			}
			text = ssResult.str();
		}
	}

	void ExtractURLString(const string& str, vector<string>& MatchedSt)
	{
		std::string::size_type pos = str.find("href");
		if(std::string::npos != pos)
		{
			string::size_type equal	= str.find("=", pos + 1);
			if(str.at(equal + 1) == '\"')
			{
				string::size_type startDQ	= str.find("\"", pos + 1);
				string::size_type endDQ		= str.find("\"", startDQ + 1);
				string Mid = str.substr(startDQ + 1, endDQ - startDQ - 1);
				if(std::string::npos != str.find("base href"))
				{
					m_BaseString = Mid;
				}
				else
				{
					MatchedSt.push_back(Mid);
				}
			}
			else
			{
				string::size_type apos	= str.find(">", pos + 1);
				string Mid = str.substr(equal + 1, apos - equal - 1);
				MatchedSt.push_back(Mid);
			}
		}
		/*boost::regex reg("s?https?://[-_.!~*'()a-zA-Z0-9;?:@&=+$,%#/]+/?");

		boost::smatch result;
		if(boost::regex_search(str.begin(), str.end(), result, reg))
		{
			if(std::string::npos != str.find("base href"))
			{
				m_BaseString = result.str(0);
				return;
			}

			for(size_t i = 0; i < result.size(); i++)
			{
				MatchedSt.push_back(result.str(i));
			}
		}*/
	}

	bool ExistBase() const 
	{
		return !m_BaseString.empty();
	}

private:
	DocumentContents	m_Contents;
	URLPointer			m_ParentURL;
	vector<URLPointer>	m_vURL;
	string				m_BaseString;
};