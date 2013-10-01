#pragma once
#include <string>
#include <queue>
#include <fstream>
using namespace std;

class SeedURLLoader
{
public:
	static int Load(const string& FilePath, queue<URLPointer>& qSeedURL)
	{
		ifstream fin(FilePath.c_str());
		if(!fin)
		{
			return -1;
		}

		while(!fin.eof())
		{
			string Line;
			getline(fin, Line);
			if(!Line.empty())
			{
				qSeedURL.push(URLPointer(new URL(Line)));
			}
		}
		return 0;
	}
};