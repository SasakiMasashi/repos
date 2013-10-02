#pragma once
#include <boost/optional.hpp>
#include <string>
#include "CLineState.h"
using namespace std;

namespace GCode
{

class GCodeLineParser
{
	static boost::optional<double> GetDigit(const string& Line, string::size_type pos);

public:
	static CLineState Parse(const string& Line);
};

} // namespace GCode