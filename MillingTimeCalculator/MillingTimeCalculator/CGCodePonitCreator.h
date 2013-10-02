#pragma once
#include "GCodeHandler.h"
#include "StatusType.h"
#include "CGCodePoint.h"

namespace GCode
{
class CPointSeq;


class CPointCreator
{
public:
	explicit CPointCreator(const GCode::GCodeHandler& Handler);
	GCode::CPointSeq* Create();

private:
	const GCode::GCodeHandler& m_Handler;
};

}; // namespace GCode