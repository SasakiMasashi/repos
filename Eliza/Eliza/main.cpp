#include <iostream>
#include <fcntl.h>	// _O_BINARY
#include <io.h>	// _setmode, _access
#include <conio.h>
#include "ApplyRule.h"
#include "RuleDatabase.h"
using namespace std;

int main(int argc, char* argv[])
{
	_setmode(_fileno(stdout), _O_BINARY);	// cout
	_setmode(_fileno(stderr), _O_BINARY);	// cerr
	setlocale(LC_ALL,"");

	bool jpn_Mode = true;

	// “ú–{ŒêÓ°ÄÞ
	if(jpn_Mode)
	{
		Eliza::RuleDatabase2 RuleDB;
		Eliza::ApplyRule Apply(&RuleDB);
		
		string Line, PreLine;

		do
		{
			cout << "Ž„     > ";
			getline(cin, Line);
			if(		string::npos != Line.find("exit")
				||	string::npos != Line.find("end")
				||	string::npos != Line.find("quit"))
			{
				break;
			}

			if(string::npos != Line.find("loop"))
				Line = PreLine;

			string Answer = Apply.Do(Line);
			cout << "ƒGƒŠƒU > " << Answer << "\n\n";

			PreLine = Line;

		}while(1);
	}

	// ‰pŒêÓ°ÄÞ
	else
	{
		Eliza::RuleDatabase RuleDB;
		Eliza::ApplyRule Apply(&RuleDB);
		
		string Line, PreLine;

		do
		{
			cout << "You  > ";
			getline(cin, Line);
			if(		string::npos != Line.find("exit")
				||	string::npos != Line.find("end")
				||	string::npos != Line.find("quit"))
			{
				break;
			}

			if(string::npos != Line.find("loop"))
				Line = PreLine;

			string Answer = Apply.Do(Line);
			cout << "ELIZA> " << Answer << "\n\n";

			PreLine = Line;

		}while(1);
	}

	return 0;
}