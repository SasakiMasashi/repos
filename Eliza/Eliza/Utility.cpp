#include "Utility.h"

namespace Eliza
{
	int GetId(const int Size)
	{
		return (int)(((float)rand() / 32768.0) * Size);
	}

	void TrimDecompose(const string& Line, const string& Key, string& BeforeKey, string& AfterKey)
	{
		BeforeKey.clear();
		AfterKey.clear();
		if(Key.empty())
			return;

		string::size_type pos = Line.find(Key);
		if(string::npos == pos)
			return;

		if(0 != pos)
			BeforeKey = Line.substr(0, pos - 1);
		if(pos + Key.length() + 1 < Line.length())
			AfterKey = Line.substr(pos + Key.length() + 1);

		if(AfterKey.length() - 1 == AfterKey.find(".") || AfterKey.length() - 1 == AfterKey.find("?"))
			AfterKey = AfterKey.substr(0, AfterKey.length() - 1);
	}

	string Replace(const string& Base, const string& Old, const string& New)
	{
		string Before, After;
		TrimDecompose(Base, Old, Before, After);
		if(Before.empty() && After.empty())
			return Base;
		if(!Before.empty())
			Before = Before + string(" ");
		if(!After.empty())
			After = string(" ") + After;
		return Before + New + After;
	}
}