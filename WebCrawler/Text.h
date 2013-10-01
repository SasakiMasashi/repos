#pragma once 
#include <string>
using namespace std;

class Text
{
public:
	Text() {}
	explicit Text(const string& text)
		:m_Text(text)
	{
	}

	void add(const string& text) { m_Text += text; }

	string get() const{ return m_Text; }
private:
	string m_Text;
};