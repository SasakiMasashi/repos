#include "CGCodePonitCreator.h"
#include "CGCodePointSeq.h"
namespace GCode
{

CPointCreator::CPointCreator(const GCode::GCodeHandler& Handler)
:m_Handler(Handler)
{
}

GCode::CPointSeq*
CPointCreator::Create()
{
	GCode::GCodeHandler::GCodeStructure::GCodeSection vGCodeLine;
	m_Handler.GetAllGCodeLine(vGCodeLine);
	if(vGCodeLine.size() <= 2)
	{
		return NULL;
	}

	GCode::StatusType Status = GCode::NOERROR;
	
	GCode::CPointSeq* PointSeq = new GCode::CPointSeq();
	PointSeq->Reserve(vGCodeLine.size() * 3);

	// äeçsñàÇ…ì_Çê∂ê¨
	for(size_t i = 0; i < vGCodeLine.size() - 1; ++i)
	{
		vector<GCode::CPoint*> vtmpPoint;
		Status = m_Handler.CreatePoint(vGCodeLine[i], vGCodeLine[i + 1], vtmpPoint);
		if(GCode::NOERROR != Status)
		{
			return NULL;
		}

		PointSeq->Add(vtmpPoint);
	}

	return PointSeq;
}

}; // namespace GCode