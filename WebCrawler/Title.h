#pragma once 
#include <string>
using namespace std;

class Title
{
public:
	Title() {}
	explicit Title(const string& title)
		:m_Title(title)
	{
	}
	string get() { return m_Title; }
private:
	string m_Title;
};