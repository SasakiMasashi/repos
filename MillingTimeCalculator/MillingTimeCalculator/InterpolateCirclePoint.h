#pragma once
#include "StatusType.h"
#include "CPoint3D.h"
#include "CLineState.h"
#include <vector>
using namespace std;

namespace GCode
{
	class CPoint;
}

GCode::StatusType
CreateInterpolationPoint(const CPoint3D<double>& StartP,
						 const CPoint3D<double>& EndP,
						 const CPoint3D<double>& Center,
						 const GCode::CLineState::InterpolationType IType,
						 const GCode::CLineState::PlaneType PType,
						 const double Speed,
						 const double Precision, // ï‚ä‘ê∏ìx
						 vector<GCode::CPoint*>& vPoint);