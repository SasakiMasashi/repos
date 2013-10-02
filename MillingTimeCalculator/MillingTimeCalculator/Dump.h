#pragma once
#include <string>
#include <vector>
#include "MeasureResult.h"
using namespace std;

namespace GCode
{
	class GCodeHandler;
	class CPoint;
}

void
DumpGCodeStructure(const string NCFilePath,
				   const GCode::GCodeHandler& Handler);

void
DumpPoint(const string& NCFilePath,
		  const vector<GCode::CPoint*>& vPoint);

// ���葬�x��pnt�t�@�C���o�͋@�\
void
DumpEachSpeedPnt(const string& NCFilePath,
				const vector<GCode::CPoint*>& vPoint);


void
DisplaymeasureResult(const string& FolderPath, const vector<GCode::MeasureResult>& vResult);