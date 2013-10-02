#pragma once
#include <vector>
using namespace std;
namespace GCode
{
class CPoint;
class CPointSeq;
class CSpeedPointSeq;
struct MeasureResult;

class CPointSeqHandler
{
public:
	explicit CPointSeqHandler(const CPointSeq* PointSeq);

	void	SepareteBySpeed();
	double	CalcLength();
	double	CalcMillingTime();	
	void	GetDisplayResult(MeasureResult& Result);

private:
	double					m_Length;
	double					m_MillingTime;
	const CPointSeq*		m_PointSeq;
	vector<CSpeedPointSeq*> m_vSpeedPointSeq;
};

} // namespace GCode