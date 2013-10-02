#include "CGCodePointSeq.h"
#include <math.h>
#include <map>
#include "StatusType.h"
#include "Utility.h"

namespace GCode{

CPointSeq::CPointSeq()
	:m_vPoint(),
	 m_Length(0.0)
{}

CPointSeq::CPointSeq(const vector<CPoint*>& vPoint)
	:m_vPoint(vPoint),
	 m_Length(0.0)
{}

bool 
CPointSeq::IsEmpty() const
{
	return m_vPoint.empty();
}

void 
CPointSeq::Add(CPoint* Point) 
{
	if(!Point)
	{
		return;
	}

	m_vPoint.push_back(Point);
}

void	
CPointSeq::Add(const vector<CPoint*>& vPoint)
{
	if(vPoint.empty())
	{
		return;
	}

	m_vPoint.insert(m_vPoint.end(), vPoint.begin(), vPoint.end());
}

void
CPointSeq::Reserve(const size_t Size)
{
	m_vPoint.reserve(Size);
}

static double
_CalcDistance(const CPoint* P1, const CPoint* P2)
{
	double v[3] = { P2->m_X - P1->m_X, 
					P2->m_Y - P1->m_Y, 
					P2->m_Z - P1->m_Z};

	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

double 
CPointSeq::CalcLength()
{
	if(m_vPoint.empty())
	{
		return 0.0;
	}

	if(m_Length > GCode::Eps)
	{
		return m_Length;
	}

	double Dist = 0.0;
	m_vPoint.front()->m_Distance = 0.0;
	for(size_t i = 0; i < m_vPoint.size() - 1; ++i)
	{
		const CPoint* P1 = m_vPoint[i];
		CPoint* P2 = m_vPoint[i + 1];
		Dist += P2->m_Distance = _CalcDistance(P1, P2);
	}

	m_Length = Dist;
	return Dist;
}

CPoint*	
CPointSeq::GetFrontPoint()
{
	if(m_vPoint.empty())
	{
		return NULL;
	}

	return m_vPoint.front();
}

CPoint* 
CPointSeq::GetBackPoint()
{
	if(m_vPoint.empty())
	{
		return NULL;
	}

	return m_vPoint.back();
}

//void CPointSeq::SepareteBySpeed(vector<CSpeedPointSeq*>& vSpeedPointSeq)
//{
//	vSpeedPointSeq.clear();
//
//	map<double, vector<CPointSeq*> > SpeedPathMap;
//	map<double, vector<CPointSeq*> >::iterator it = SpeedPathMap.begin();
//
//	// “_‚ð‘—‚è‘¬“x–ˆ‚É•ª—Þ
//	{
//		CPointSeq* Seq = new CPointSeq();
//		for(size_t i = 0; i < m_vPoint.size(); ++i)
//		{
//			if(i != 0 && m_vPoint[i - 1]->m_Speed != m_vPoint[i]->m_Speed)
//			{
//				if(SpeedPathMap.end() != (it = SpeedPathMap.find(m_vPoint[i - 1]->m_Speed)))
//				{
//					(*it).second.push_back(Seq);
//				}
//				else
//				{
//					SpeedPathMap[m_vPoint[i - 1]->m_Speed].push_back(Seq);
//				}
//				Seq = new CPointSeq();
//				Seq->Add(m_vPoint[i - 1]);
//			}
//
//			Seq->Add(m_vPoint[i]);
//		}
//
//		if(!Seq->IsEmpty())
//		{
//			SpeedPathMap[Seq->GetBackPoint()->m_Speed].push_back(Seq);
//		}
//		else
//		{
//			delete Seq;
//		}
//	}
//
//	// “_—ñ‚ðSpeedPointSeq‚Ö‚Æ•ÏŠ·
//	for(map<double, vector<CPointSeq*> >::reverse_iterator rit = SpeedPathMap.rbegin();
//		rit != SpeedPathMap.rend();
//		++rit)
//	{
//		CSpeedPointSeq* Seq = new CSpeedPointSeq(rit->first, it->second);
//		vSpeedPointSeq.push_back(Seq);
//	}
//}

CSpeedPointSeq::CSpeedPointSeq(const double Speed, vector<CPointSeq*>& vPointSeq)
	:m_Speed(Speed),
	 m_Length(0.0),
	 m_MillingTime(0.0)
{
	m_vPointSeq.swap(vPointSeq);
}

CSpeedPointSeq::~CSpeedPointSeq()
{
	for_each(m_vPointSeq.begin(), m_vPointSeq.end(), Deletion());
}

double 
CSpeedPointSeq::CalcLength()
{
	if(m_Length > GCode::Eps)
	{
		return m_Length;
	}

	double Length = 0.0;
	for(size_t i = 0; i < m_vPointSeq.size(); ++i)
	{
		Length += m_vPointSeq[i]->CalcLength();
	}
	m_Length = Length;

	return Length;
}

double 
CSpeedPointSeq::CalcMillingTime()
{
	if(fabs(m_Speed) < GCode::Eps)
	{
		return 0.0;
	}

	if(m_MillingTime > GCode::Eps)
	{
		return m_MillingTime;
	}

	const double Length = CalcLength();
	m_MillingTime = Length / m_Speed;

	return m_MillingTime;
}

} // namespace GCode