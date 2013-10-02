#pragma once

template<class T>
class CPoint2D
{
public:
	T m_X;
	T m_Y;
};

// “ÁŽê‰»
template<>
class CPoint2D<double>
{
public:
	CPoint2D()
		:m_X(0.0), m_Y(0.0)
	{}
	CPoint2D(double X, double Y)
		:m_X(X), m_Y(Y)
	{}

public:
	double m_X;
	double m_Y;
};

