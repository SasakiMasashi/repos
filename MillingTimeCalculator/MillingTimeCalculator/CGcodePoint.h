#pragma once
#include "CPoint3D.h"
#include "CLineState.h"

namespace GCode
{
class CPoint : public CPoint3D<double>
{
public:
	CPoint(const double X, const double Y, const double Z)
		:CPoint3D(X, Y, Z), m_Speed(0.0), m_Distance(0.0),
		 m_IType(CLineState::G00)
	{}

	double m_Speed;
	double m_Distance;
	CLineState::InterpolationType m_IType;
};

}; // namespace GCode