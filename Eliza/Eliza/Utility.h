#pragma once
#include <string>
using namespace std;

namespace Eliza
{
	int GetId(const int Size);

	void TrimDecompose(const string& Line, 
				   const string& Key, 
				   string& BeforeKey, 
				   string& AfterKey);

	string Replace(const string& Base, 
				   const string& Old, 
				   const string& New);
}