#pragma once
#include <string>
using namespace std;

class DocumentID
{
public:
	explicit DocumentID(const string& id)
		:m_Id(id)
	{
	}
	string get() const { return m_Id; }

	bool operator<(const DocumentID& docID) const
	{
		return (this->get() < docID.get());
	}
private:
	string m_Id;
};