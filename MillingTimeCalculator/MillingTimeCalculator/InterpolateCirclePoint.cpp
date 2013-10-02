#include "InterpolateCirclePoint.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "CPoint2D.h"
#include "CGCodePoint.h"

const double Eps = 0.00005;

// Centerから見たPointの角度(ラジアン）を返す
static double
GetAngle(const CPoint2D<double>& Point,
		 const CPoint2D<double>& Center)
{
	CPoint2D<double> Diff(Point.m_X - Center.m_X, Point.m_Y - Center.m_Y);

	// 原点
	if(fabs(Diff.m_X) < Eps && fabs(Diff.m_Y) < Eps)
	{
		return 0.0;
	}

	// 0度
	if(Diff.m_X > Eps && fabs(Diff.m_Y) < Eps)
	{
		return 0.0;
	}

	// 90度
	if(fabs(Diff.m_X) < Eps && Diff.m_Y > Eps)
	{
		return M_PI / 2.0;
	}

	// 180度
	if(Diff.m_X < -Eps && fabs(Diff.m_Y) < Eps)
	{
		return M_PI;
	}

	// 270度
	if(fabs(Diff.m_X) < Eps && Diff.m_Y < -Eps)
	{
		return 3.0 * M_PI / 2.0;
	}

	const double Dist = sqrt(Diff.m_X * Diff.m_X + Diff.m_Y * Diff.m_Y);

	// 第一象限
	if(Diff.m_X > Eps && Diff.m_Y > Eps)
	{
		return acos(Diff.m_X / Dist);
	}

	// 第二象限
	if(Diff.m_X < -Eps && Diff.m_Y > Eps)
	{
		return acos(Diff.m_Y / Dist) + M_PI / 2.0;
	}

	// 第三象限
	if(Diff.m_X < -Eps && Diff.m_Y < -Eps)
	{
		return acos(fabs(Diff.m_X) / Dist) + M_PI;
	}

	// 第四象限
	if(Diff.m_X > Eps && Diff.m_Y < -Eps)
	{
		return acos(fabs(Diff.m_Y) / Dist) + 3.0 * M_PI / 2.0;
	}

	// ここに来ることはない
	return 0.0;
}

static double
CalcDistance(const CPoint3D<double>& P1,
			 const CPoint3D<double>& P2)
{
	return sqrt((P2.m_X - P1.m_X) * (P2.m_X - P1.m_X) 
			  + (P2.m_Y - P1.m_Y) * (P2.m_Y - P1.m_Y) 
			  + (P2.m_Z - P1.m_Z) * (P2.m_Z - P1.m_Z));
}

static double 
GetRadius(const double StartRadius,
		  const double EndRadius,
		  const double StartAngle,
		  const double EndAngle,
		  const double Angle)
{
	if(fabs(EndRadius - StartRadius) < Eps)
	{
		return StartRadius;
	}

	if(EndAngle > StartAngle)
	{
		const double TotalDiffAngle = EndAngle - StartAngle;
		const double DiffAngle = Angle - StartAngle;
		const double Ratio = DiffAngle / TotalDiffAngle;
		return Ratio *( EndRadius - StartRadius) + StartRadius;
	}
	else
	{
		const double TotalDiffAngle = StartAngle - EndAngle;
		const double DiffAngle = Angle - EndAngle;
		const double Ratio = DiffAngle / TotalDiffAngle;
		return Ratio *( StartRadius - EndRadius) + EndRadius;
	}
}

static void
CreateInterpolationPoint_XY(const CPoint3D<double>& StartP,
							const CPoint3D<double>& EndP,
							const CPoint3D<double>& Center,
							const double StartRadius,
							const double EndRadius,
							const bool IsClockwise,
							const double Precision,
							vector<GCode::CPoint*>& vPoint)
{
	CPoint2D<double> Start2D(	StartP.m_X, StartP.m_Y);
	CPoint2D<double> End2D(		EndP.m_X,	EndP.m_Y);
	CPoint2D<double> Center2D(	Center.m_X, Center.m_Y);

	const double StartAngle = GetAngle(Start2D, Center2D);
	double EndAngle	= GetAngle(End2D,	Center2D);
	if(!IsClockwise)
	{
		if(StartAngle > EndAngle)
		{
			EndAngle += 2.0 * M_PI;
		}
	}
	else
	{
		if(StartAngle < EndAngle)
		{
			EndAngle -= 2. * M_PI;
		}
	}

	GCode::CPoint* StartPoint	= new GCode::CPoint(StartP.m_X, StartP.m_Y, StartP.m_Z);
	GCode::CPoint* EndPoint		= new GCode::CPoint(EndP.m_X, EndP.m_Y, EndP.m_Z);
	vPoint.push_back(StartPoint);

	const double MaxRadius = max(StartRadius, EndRadius);
	const double DivAngle = 2 * acos(1 - Precision / MaxRadius);
	if(!IsClockwise)
	{
		for(double Angle = StartAngle + DivAngle; 
			Angle < EndAngle, EndAngle - Angle > DivAngle;
			Angle += DivAngle)
		{
			const double Radius = GetRadius(StartRadius, EndRadius, StartAngle, EndAngle, Angle);
			double X = Radius * cos(Angle) + Center.m_X;
			double Y = Radius * sin(Angle) + Center.m_Y;
			GCode::CPoint* Point = new GCode::CPoint(X, Y, Center.m_Z);
			vPoint.push_back(Point);
		}
	}
	else
	{
		for(double Angle = StartAngle - DivAngle; 
			Angle > EndAngle, Angle - EndAngle > DivAngle;
			Angle -= DivAngle)
		{
			const double Radius = GetRadius(StartRadius, EndRadius, StartAngle, EndAngle, Angle);
			double X = Radius * cos(Angle) + Center.m_X;
			double Y = Radius * sin(Angle) + Center.m_Y;
			GCode::CPoint* Point = new GCode::CPoint(X, Y, Center.m_Z);
			vPoint.push_back(Point);
		}
	}
	vPoint.push_back(EndPoint);
}

static void
CreateInterpolationPoint_XZ(const CPoint3D<double>& StartP,
							const CPoint3D<double>& EndP,
							const CPoint3D<double>& Center,
							const double StartRadius,
							const double EndRadius,
							const bool IsClockwise,
							const double Precision,
							vector<GCode::CPoint*>& vPoint)
{
	CPoint2D<double> Start2D(	StartP.m_X, StartP.m_Z);
	CPoint2D<double> End2D(		EndP.m_X,	EndP.m_Z);
	CPoint2D<double> Center2D(	Center.m_X, Center.m_Z);

	const double StartAngle = GetAngle(Start2D, Center2D);
	double EndAngle	= GetAngle(End2D,	Center2D);
	if(IsClockwise)
	{
		if(StartAngle > EndAngle)
		{
			EndAngle += 2.0 * M_PI;
		}
	}
	else
	{
		if(StartAngle < EndAngle)
		{
			EndAngle -= 2. * M_PI;
		}
	}

	GCode::CPoint* StartPoint	= new GCode::CPoint(StartP.m_X, StartP.m_Y, StartP.m_Z);
	GCode::CPoint* EndPoint		= new GCode::CPoint(EndP.m_X, EndP.m_Y, EndP.m_Z);
	vPoint.push_back(StartPoint);

	const double MaxRadius = max(StartRadius, EndRadius);
	const double DivAngle = 2 * acos(1 - Precision / MaxRadius);
	if(IsClockwise)
	{
		for(double Angle = StartAngle + DivAngle;
			Angle < EndAngle, EndAngle - Angle > DivAngle;
			Angle += DivAngle)
		{
			const double Radius = GetRadius(StartRadius, EndRadius, StartAngle, EndAngle, Angle);
			double X = Radius * cos(Angle) + Center.m_X;
			double Z = Radius * sin(Angle) + Center.m_Z;
			GCode::CPoint* Point = new GCode::CPoint(X, Center.m_Y, Z);
			vPoint.push_back(Point);
		}
	}
	else
	{
		for(double Angle = StartAngle - DivAngle; 
			Angle > EndAngle, Angle - EndAngle > DivAngle;
			Angle -= DivAngle)
		{
			const double Radius = GetRadius(StartRadius, EndRadius, StartAngle, EndAngle, Angle);
			double X = Radius * cos(Angle) + Center.m_X;
			double Z = Radius * sin(Angle) + Center.m_Z;
			GCode::CPoint* Point = new GCode::CPoint(X, Center.m_Y, Z);
			vPoint.push_back(Point);
		}
	}
	vPoint.push_back(EndPoint);
}

static void
CreateInterpolationPoint_YZ(const CPoint3D<double>& StartP,
							const CPoint3D<double>& EndP,
							const CPoint3D<double>& Center,
							const double StartRadius,
							const double EndRadius,
							const bool IsClockwise,
							const double Precision,
							vector<GCode::CPoint*>& vPoint)
{
	CPoint2D<double> Start2D(	StartP.m_Y, StartP.m_Z);
	CPoint2D<double> End2D(		EndP.m_Y,	EndP.m_Z);
	CPoint2D<double> Center2D(	Center.m_Y, Center.m_Z);

	const double StartAngle = GetAngle(Start2D, Center2D);
	double EndAngle	= GetAngle(End2D,	Center2D);
	if(!IsClockwise)
	{
		if(StartAngle > EndAngle)
		{
			EndAngle += 2.0 * M_PI;
		}
	}
	else
	{
		if(StartAngle < EndAngle)
		{
			EndAngle -= 2. * M_PI;
		}
	}

	GCode::CPoint* StartPoint	= new GCode::CPoint(StartP.m_X, StartP.m_Y, StartP.m_Z);
	GCode::CPoint* EndPoint		= new GCode::CPoint(EndP.m_X, EndP.m_Y, EndP.m_Z);
	vPoint.push_back(StartPoint);

	const double MaxRadius = max(StartRadius, EndRadius);
	const double DivAngle = 2 * acos(1 - Precision / MaxRadius);
	if(!IsClockwise)
	{
		for(double Angle = StartAngle + DivAngle; 
			Angle < EndAngle, EndAngle - Angle > DivAngle;
			Angle += DivAngle)
		{
			const double Radius = GetRadius(StartRadius, EndRadius, StartAngle, EndAngle, Angle);
			double Y = Radius * cos(Angle) + Center.m_Y;
			double Z = Radius * sin(Angle) + Center.m_Z;
			GCode::CPoint* Point = new GCode::CPoint(Center.m_X, Y, Z);
			vPoint.push_back(Point);
		}
	}
	else
	{
		for(double Angle = StartAngle - DivAngle; 
			Angle > EndAngle, Angle - EndAngle > DivAngle;
			Angle -= DivAngle)
		{
			const double Radius = GetRadius(StartRadius, EndRadius, StartAngle, EndAngle, Angle);
			double Y = Radius * cos(Angle) + Center.m_Y;
			double Z = Radius * sin(Angle) + Center.m_Z;
			GCode::CPoint* Point = new GCode::CPoint(Center.m_X, Y, Z);
			vPoint.push_back(Point);
		}
	}

	vPoint.push_back(EndPoint);
}

GCode::StatusType
CreateInterpolationPoint(const CPoint3D<double>& StartP,
						 const CPoint3D<double>& EndP,
						 const CPoint3D<double>& Center,
						 const GCode::CLineState::InterpolationType IType,
						 const GCode::CLineState::PlaneType PType,
						 const double Speed,
						 const double Precision, // 補間精度
						 vector<GCode::CPoint*>& vPoint)
{
	const double StartRadius = CalcDistance(StartP, Center);
	const double EndRadius = CalcDistance(EndP, Center);
	/*if(fabs(Radius1 - Radius2) > Precision)
	{
		return GCode::BAD_PARAM;
	}*/

	const bool IsClockwise = (GCode::CLineState::G02 == IType);
	if(GCode::CLineState::G17 == PType)
	{
		CreateInterpolationPoint_XY(StartP, EndP, Center, StartRadius, EndRadius, IsClockwise, Precision, vPoint);
	}
	if(GCode::CLineState::G18 == PType)
	{
		CreateInterpolationPoint_XZ(StartP, EndP, Center, StartRadius, EndRadius, IsClockwise, Precision, vPoint);
	}
	if(GCode::CLineState::G19 == PType)
	{
		CreateInterpolationPoint_YZ(StartP, EndP, Center, StartRadius, EndRadius, IsClockwise, Precision, vPoint);
	}

	for(size_t i = 0; i < vPoint.size(); ++i)
	{
		vPoint[i]->m_Speed = Speed;
		vPoint[i]->m_IType = IType;
	}

	return GCode::NOERROR;
}