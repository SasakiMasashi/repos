#include "GCodeLineParser.h"

namespace GCode
{

boost::optional<double> 
GCodeLineParser::GetDigit(const string& Line, string::size_type pos)
{
	if(string::npos == pos || pos >= Line.size())
	{
		return boost::none_t();
	}
	
	string::size_type start = pos;

	char c = 0;
	//while(isdigit(c = char(Line[pos]) || c == '.')) pos++;
	while(pos < Line.size() && !isalpha(c = char(Line[pos]))) pos++;

	string::size_type end = pos;

	string digit = Line.substr(start, end - start);
	if(digit.empty())
	{
		return boost::none_t();
	}

	return atof(digit.c_str());
}

CLineState 
GCodeLineParser::Parse(const string& Line)
{
	string::size_type pos = 0;
	CLineState State;

	char C = 0;
	while(pos < Line.size())
	{
		C = char(Line[pos]);
		if(C == '(' || C == ':')
		{
			break;
		}

		pos++;

		if(isalpha(C))
		{
			State.SetValue(C, GetDigit(Line, pos));
		}
	}

	return State;
}

} // namespace GCode