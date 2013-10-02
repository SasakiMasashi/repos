#include "GCodeLine.h"
#include <sstream>
#include <iomanip>

namespace GCode
{
CGcodeLine::CGcodeLine(const string& Line, const CLineState& RelativeState, const CLineState& AbsoluteState)
	:m_Line(Line), 
	 m_RelativeState(RelativeState), 
	 m_AbsoluteState(AbsoluteState)
{}


std::string 
CGcodeLine::GetInterpolationTypeString() const
{
	switch(GetInterpolationType())
	{
	case CLineState::G00: return string("G00");
	case CLineState::G01: return string("G01");
	case CLineState::G02: return string("G02");
	case CLineState::G03: return string("G03");
	}
	return string("none");
}

std::string 
CGcodeLine::GetPlaneTypeString() const
{
	switch(GetPlaneType())
	{
	case CLineState::G17: return string("G17");
	case CLineState::G18: return string("G18");
	case CLineState::G19: return string("G19");
	}
	return string("none");
}

string 
CGcodeLine::CreateLine(const int DecimalPlace) const
{
	stringstream sst;
	if(HasInterplationCommand()) sst << GetInterpolationTypeString();
	if(HasX())	sst << "X" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetX();
	if(HasY())	sst << "Y" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetY();
	if(HasZ())	sst << "Z" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetZ();
	if(HasI())	sst << "I" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetI();
	if(HasJ())	sst << "J" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetJ();
	if(HasK())	sst << "K" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetK();
	if(HasF())	sst << "F" << setprecision(DecimalPlace) << setiosflags(ios::fixed) << GetF();
	return sst.str();
}

void 
CGcodeLine::ChangeRelativeState(const CLineState& AddState, const int DecimalPlace)
{
	ofstream fout("ChangeRelativeState.txt", ios::app);
	Dump(fout);

	m_RelativeState += AddState;
	m_AbsoluteState += AddState;
	m_Line = CreateLine(DecimalPlace);
	ClearAbsoluteState();

	Dump(fout);
}

void 
CGcodeLine::SetValue(const char ch, const double value, const int DecimalPlace)
{
	m_RelativeState.SetValue(ch, value);
	m_Line = CreateLine(DecimalPlace);
	ClearAbsoluteState();
}

bool 
CGcodeLine::IsValid() const 
{ 
	return (HasX() || HasY() || HasZ()); 
}

void 
CGcodeLine::WriteLine(ofstream& fout)
{
	fout << m_Line << "\n";
}

void 
CGcodeLine::Dump(ofstream& fout) const
{
	fout << m_Line << ",";
	fout << "\tVariation( ";
	m_RelativeState.Dump(fout);
	fout << " ), ";
	fout << "\tStatus( ";
	m_AbsoluteState.Dump(fout);
	fout << " ) \n";
}


} // namespace GCode