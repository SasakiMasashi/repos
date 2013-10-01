#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "DocumentID.h"
#include "Link.h"
#include "URL.h"
#include "Title.h"
#include "Text.h"
using namespace std;

class DocumentContents
{
public:
	DocumentContents()
		:m_Title(), 
		 m_DocID(string()),
		 m_URL(string()),
		 m_Text(),
		 m_vLink()
	{}

	// setter
	void setLinks(const vector<Link>& vLink) { m_vLink = vLink; } 
	void setURL(const URL& url) { m_URL = url; } 
	void setDocumentID(const DocumentID& docID) { m_DocID = docID; }
	void setTitle(const Title& title) { m_Title = title; }
	void addText(const string& text) { m_Text.add(text); }

	// getter
	const vector<Link>& getLinks() const { return m_vLink; }
	const URL&			getURL() const { return m_URL; }
	const DocumentID&	getDocumentID() const { return m_DocID; }
	const Title&		getTitle() const { return m_Title; }
	const Text&			getText() const { return m_Text; }

private:
	Title			m_Title;
	DocumentID		m_DocID;
	URL				m_URL;
	Text			m_Text;
	vector<Link>	m_vLink;
};