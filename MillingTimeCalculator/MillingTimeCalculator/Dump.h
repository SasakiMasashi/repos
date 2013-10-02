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

// 送り速度別pntファイル出力機能
void
DumpEachSpeedPnt(const string& NCFilePath,
				const vector<GCode::CPoint*>& vPoint);


void
DisplaymeasureResult(const string& FolderPath, const vector<GCode::MeasureResult>& vResult);