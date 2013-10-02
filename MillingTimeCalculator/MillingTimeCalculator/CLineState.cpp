#include "CLineState.h"
#include "StatusType.h"
#include <math.h>

namespace GCode
{

CLineState::CLineState() 
	:m_X(boost::none_t()), 
	 m_Y(boost::none_t()), 
	 m_Z(boost::none_t()), 
	 m_F(boost::none_t()),
	 m_I(boost::none_t()), 
	 m_J(boost::none_t()), 
	 m_K(boost::none_t()),
	 m_IType(boost::none_t()),
	 m_PType(boost::none_t())
{
}

CLineState::CLineState(const CLineState& State)
{
	SetState(State, *this);
}

CLineState::~CLineState()
{
}

const CLineState& 
CLineState::operator=(const CLineState& State)
{
	SetState(State, *this);
	return *this;
}

const CLineState& 
CLineState::operator+=(const CLineState& State)
{
	*this = (*this + State);
	return *this;
}

CLineState 
CLineState::operator+(const CLineState& State) const
{
	CLineState ResultState = *this;
	AddState(State, ResultState);
	return ResultState;
}

void 
CLineState::AddState(const CLineState& From, CLineState& To) const
{
	if(From.m_X)		To.m_X		= From.m_X.get();
	if(From.m_Y)		To.m_Y		= From.m_Y.get();
	if(From.m_Z)		To.m_Z		= From.m_Z.get();
	if(From.m_F)		To.m_F		= From.m_F.get();			
	if(From.m_I)		To.m_I		= From.m_I.get();
	if(From.m_J)		To.m_J		= From.m_J.get();
	if(From.m_K)		To.m_K		= From.m_K.get();
	if(From.m_IType)	To.m_IType	= From.m_IType.get();
	if(From.m_PType)	To.m_PType	= From.m_PType.get();

	if(!From.m_I)		To.m_I		= boost::none_t();
	if(!From.m_J)		To.m_J		= boost::none_t();
	if(!From.m_K)		To.m_K		= boost::none_t();
}

void 
CLineState::SetState(const CLineState& From, CLineState& To) const
{
	AddState(From, To);

	if(!From.m_X)		To.m_X		= boost::none_t();
	if(!From.m_Y)		To.m_Y		= boost::none_t();
	if(!From.m_Z)		To.m_Z		= boost::none_t();
	if(!From.m_F)		To.m_F		= boost::none_t();	
	if(!From.m_I)		To.m_I		= boost::none_t();
	if(!From.m_J)		To.m_J		= boost::none_t();
	if(!From.m_K)		To.m_K		= boost::none_t();
	if(!From.m_IType)	To.m_IType	= boost::none_t();
	if(!From.m_PType)	To.m_PType	= boost::none_t();

	/*To.m_X		= From.m_X;
	To.m_Y		= From.m_Y;
	To.m_Z		= From.m_Z;
	To.m_F		= From.m_F;
	To.m_I		= From.m_I;
	To.m_J		= From.m_J;
	To.m_K		= From.m_K;
	To.m_IType	= From.m_IType;
	To.m_PType	= From.m_PType;*/
}

void 
CLineState::SetValue(const char C, const boost::optional<double> Val)
{
	if(!Val)
	{
		return;
	}
	double val = Val.get();

	switch(C)
	{
	case 'X': m_X = Val; break;
	case 'Y': m_Y = Val; break;
	case 'Z': m_Z = Val; break;
	case 'F': m_F = Val; break;
	case 'I': m_I = Val; break;
	case 'J': m_J = Val; break;
	case 'K': m_K = Val; break;
	case 'G': 
		{
			if(fabs(val - 0.) < GCode::Eps)
			{
				m_IType.reset(G00);
				m_I = boost::none_t();
				m_J = boost::none_t();
				m_K = boost::none_t();
			}
			if(fabs(val - 1.) < GCode::Eps)
			{
				m_IType.reset(G01);
				m_I = boost::none_t();
				m_J = boost::none_t();
				m_K = boost::none_t();
			}
			if(fabs(val - 2.) < GCode::Eps) m_IType.reset(G02);
			if(fabs(val - 3.) < GCode::Eps) m_IType.reset(G03);
			if(fabs(val - 17.) < GCode::Eps) m_PType.reset(G17);
			if(fabs(val - 18.) < GCode::Eps) m_PType.reset(G18);
			if(fabs(val - 19.) < GCode::Eps) m_PType.reset(G19);
			break;
		}
	default:
		break;
	}
}

void 
CLineState::Dump(ofstream& fout) const
{			
	if(m_X) fout << " X=" << m_X.get();
	if(m_Y) fout << " Y=" << m_Y.get();
	if(m_Z) fout << " Z=" << m_Z.get();
	if(m_F) fout << " F=" << m_F.get();
	if(m_I) fout << " I=" << m_I.get();
	if(m_J) fout << " J=" << m_J.get();
	if(m_K) fout << " K=" << m_K.get();
	if(m_IType) 
	{
		switch(m_IType.get())
		{
		case G00: fout << " G=00"; break;
		case G01: fout << " G=01"; break;
		case G02: fout << " G=02"; break;
		case G03: fout << " G=03"; break;
		}
	}
	if(m_PType) 
	{
		switch(m_PType.get())
		{
		case G17: fout << " G=17"; break;
		case G18: fout << " G=18"; break;
		case G19: fout << " G=19"; break;
		}
	}
}

} // namespace GCode