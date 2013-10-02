#pragma once

template<class T>
class CPoint3D
{
public:
	T m_X;
	T m_Y;
	T m_Z;
};

// “ÁŽê‰»
template<>
class CPoint3D<double>
{
public:
	CPoint3D() 
		:m_X(0.0), m_Y(0.0), m_Z(0.0)
	{}

	CPoint3D(double X, double Y, double Z)
		:m_X(X), m_Y(Y), m_Z(Z)
	{}

public:
	double m_X;
	double m_Y;
	double m_Z;
};