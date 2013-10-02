#pragma once
#include <vector>
#include "CGCodePoint.h"
using namespace std;

namespace GCode{
class CSpeedPointSeq;

class CPointSeq
{
public:
	CPointSeq();
	explicit CPointSeq(const vector<CPoint*>& vPoint);

	bool	IsEmpty() const;
	void	Add(CPoint* Point);
	void	Add(const vector<CPoint*>& vPoint);
	void	Reserve(const size_t Size);
	double	CalcLength();
	CPoint*	GetFrontPoint();
	CPoint* GetBackPoint();
	const vector<CPoint*>& GetPoints() const { return m_vPoint; }
	//void	SepareteBySpeed(vector<CSpeedPointSeq*>& vSpeedPointSeq);

private:
	double			m_Length;
	vector<CPoint*> m_vPoint;
};

class CSpeedPointSeq
{
public:
	CSpeedPointSeq(const double Speed, vector<CPointSeq*>& vPointSeq);
	~CSpeedPointSeq();

	double GetSpeed() { return m_Speed; }
	double CalcLength();
	double CalcMillingTime();

private:
	double				m_Length;
	double				m_MillingTime;
	double				m_Speed;
	vector<CPointSeq*>	m_vPointSeq;
};

} // namespace GCode