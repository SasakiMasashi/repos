#include "CGCodePointSeqHandler.h"
#include "CGCodePoint.h"
#include "CGCodePointSeq.h"
#include <map>
#include "MeasureResult.h"

namespace GCode
{

CPointSeqHandler::CPointSeqHandler(const CPointSeq* PointSeq)
	:m_PointSeq(PointSeq),
	 m_vSpeedPointSeq()
{}

void 
CPointSeqHandler::SepareteBySpeed()
{
	m_vSpeedPointSeq.clear();

	map<double, vector<CPointSeq*> > SpeedPathMap;
	map<double, vector<CPointSeq*> >::iterator it = SpeedPathMap.begin();

	// “_‚ð‘—‚è‘¬“x–ˆ‚É•ª—Þ
	{
		CPointSeq* Seq = new CPointSeq();
		const vector<CPoint*>& vPoint = m_PointSeq->GetPoints();

		for(size_t i = 0; i < vPoint.size(); ++i)
		{
			if(i != 0 && vPoint[i - 1]->m_Speed != vPoint[i]->m_Speed)
			{
				if(SpeedPathMap.end() != (it = SpeedPathMap.find(vPoint[i - 1]->m_Speed)))
				{
					(*it).second.push_back(Seq);
				}
				else
				{
					SpeedPathMap[vPoint[i - 1]->m_Speed].push_back(Seq);
				}
				Seq = new CPointSeq();
				Seq->Add(vPoint[i - 1]);
			}

			Seq->Add(const_cast<CPoint*>(vPoint[i]));
		}

		if(!Seq->IsEmpty())
		{
			SpeedPathMap[Seq->GetBackPoint()->m_Speed].push_back(Seq);
		}
		else
		{
			delete Seq;
		}
	}

	// “_—ñ‚ðSpeedPointSeq‚Ö‚Æ•ÏŠ·
	for(map<double, vector<CPointSeq*> >::reverse_iterator rit = SpeedPathMap.rbegin();
		rit != SpeedPathMap.rend();
		++rit)
	{
		CSpeedPointSeq* Seq = new CSpeedPointSeq(rit->first, rit->second);
		m_vSpeedPointSeq.push_back(Seq);
	}
}

double 
CPointSeqHandler::CalcLength()
{
	double Dist = 0.0;
	for(size_t i = 0; i < m_vSpeedPointSeq.size(); ++i)
	{
		Dist += m_vSpeedPointSeq[i]->CalcLength();
	}
	m_Length = Dist;

	return Dist;
}

double 
CPointSeqHandler::CalcMillingTime()
{
	double Time = 0.0;
	for(size_t i = 0; i < m_vSpeedPointSeq.size(); ++i)
	{
		Time += m_vSpeedPointSeq[i]->CalcMillingTime();
	}
	m_MillingTime = Time;

	return Time;
}

void
CPointSeqHandler::GetDisplayResult(MeasureResult& Result)
{
	Result.m_Length = m_Length;
	Result.m_MillingTime = m_MillingTime;

	for(size_t i = 0; i < m_vSpeedPointSeq.size(); ++i)
	{
		MeasureResult::SpeedPart Part;
		Part.m_Length		= m_vSpeedPointSeq[i]->CalcLength();
		Part.m_MillingTime	= m_vSpeedPointSeq[i]->CalcMillingTime();
		Part.m_Speed		= m_vSpeedPointSeq[i]->GetSpeed();

		Result.m_vSpeedPart.push_back(Part);
	}
}

} // namespace GCode