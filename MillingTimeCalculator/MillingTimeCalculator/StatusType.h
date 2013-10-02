#pragma once
namespace GCode
{

enum StatusType
{
	NOERROR,
	BAD_PARAM,
	BAD_MEMBER,
};

const double Eps = 0.00005;
const double RAPIDSPEED = 18000.0; // Žb’è’l

} //namespace GCode