#include "Dump.h"
#include "CGCodePoint.h"
#include "FileUtility.h"
#include <sstream>
#include "MachineFileSetting.h"
#include <map>
#include <math.h>
#include "GCodeHandler.h"

static string
GetOutputFilePath(const string& NCFilePath)
{
	string FolderPath, FileName;
	SeparateFilePath(NCFilePath, FolderPath, FileName);
	if(FileName.empty())
	{
		return string();
	}

	string NCExtension;
	GetMachinePathSetting(NCFilePath, string(), string(), NCExtension);

	stringstream sst;
	sst << FolderPath
		<< "\\Analyze-"
		<< FileName.substr(0, FileName.size() - NCExtension.size() - 1)
		<< ".txt";

	return sst.str();
}

void
DumpGCodeStructure(const string NCFilePath,
				   const GCode::GCodeHandler& Handler)
{
	const string OutputFilePath = GetOutputFilePath(NCFilePath);
	if(OutputFilePath.empty())
	{
		return;
	}

	ofstream fout(OutputFilePath.c_str());
	Handler.Dump(fout);
}

static string
GetDumpPntFilePath(const string& NCFilePath)
{
	string FolderPath, FileName;
	SeparateFilePath(NCFilePath, FolderPath, FileName);
	if(FileName.empty())
	{
		return string();
	}

	string NCExtension;
	GetMachinePathSetting(NCFilePath, string(), string(), NCExtension);

	stringstream sst;
	sst << FolderPath
		<< "\\"
		<< FileName.substr(0, FileName.size() - NCExtension.size() - 1)
		<< ".pnt";

	return sst.str();
}

void
DumpPoint(const string& NCFilePath,
		  const vector<GCode::CPoint*>& vPoint)
{
	const string FilePath = GetDumpPntFilePath(NCFilePath);
	ofstream fout(FilePath.c_str(), ios::app);
	if(vPoint.size() > 1)
	{
		fout << "begin\n";
		for(size_t i = 0; i < vPoint.size(); ++i)
		{
			fout << vPoint[i]->m_X << "," << vPoint[i]->m_Y << "," << vPoint[i]->m_Z << "\n";
		}
		fout << "end\n";
	}
}

// 送り速度別pntファイル出力機能
void
DumpEachSpeedPnt(const string& NCFilePath,
				const vector<GCode::CPoint*>& vPoint)
{
	map<double, vector<vector<GCode::CPoint*> > > SpeedPathMap;
	map<double, vector<vector<GCode::CPoint*> > >::iterator it = SpeedPathMap.begin();

	// 点を送り速度毎に分類
	vector<GCode::CPoint*> vtmpPoint;
	for(size_t i = 0; i < vPoint.size(); ++i)
	{
		if(i != 0 && vPoint[i - 1]->m_Speed != vPoint[i]->m_Speed)
		{
			if(SpeedPathMap.end() != (it = SpeedPathMap.find(vPoint[i - 1]->m_Speed)))
			{
				(*it).second.push_back(vtmpPoint);
			}
			else
			{
				SpeedPathMap[vPoint[i - 1]->m_Speed].push_back(vtmpPoint);
			}
			vtmpPoint.clear();
			vtmpPoint.push_back(vPoint[i - 1]);
		}
		
		vtmpPoint.push_back(vPoint[i]);
	}

	if(!vtmpPoint.empty())
	{
		SpeedPathMap[vtmpPoint.back()->m_Speed].push_back(vtmpPoint);
	}

	string FolderPath, FileName;
	SeparateFilePath(NCFilePath, FolderPath, FileName);
	if(FileName.empty())
	{
		return;
	}

	string NCExtension;
	GetMachinePathSetting(NCFilePath, string(), string(), NCExtension);

	string FileNameWithoutExt = FileName.substr(0, FileName.size() - NCExtension.size() - 1);

	// 送り速度別にファイル出力
	/*for(it = SpeedPathMap.begin();
		it != SpeedPathMap.end();
		++it)
	{
		vector<vector<GCode::CPoint*>>& vvPoint = it->second;

		stringstream sst;
		sst << FolderPath
			<< "\\"
			<< FileNameWithoutExt
			<< "-"
			<< it->first
			<< ".pnt";

		ofstream fout(sst.str().c_str(), ios::app);
		for(size_t i = 0; i < vvPoint.size(); ++i)
		{
			fout << "begin\n";
			for(size_t j = 0; j < vvPoint[i].size(); ++j)
			{
				fout << vvPoint[i][j]->m_X << "," << vvPoint[i][j]->m_Y << "," << vvPoint[i][j]->m_Z << "\n";
			}
			fout << "end\n";
		}
	}*/

	// 送り速度毎に別の色をつけて一つのファイルに出力
	{
		stringstream sst;
		sst << FolderPath
			<< "\\"
			<< FileNameWithoutExt
			<< "-SpeedColor.pnt";
		ofstream fout(sst.str().c_str(), ios::app);

		// あらかじめ色を登録しておく
		vector<double*> vColor;
		double Color1[4]	= { 0.0, 1.0, 0.0, 1.0 }; // 緑
		double Color2[4]	= { 1.0, 1.0, 0.0, 1.0 }; // 黄
		double Color3[4]	= { 0.0, 1.0, 1.0, 1.0 }; // 水
		double Color4[4]	= { 1.0, 0.5, 0.0, 1.0 }; // オレンジ
		double Color5[4]	= { 1.0, 0.5, 1.0, 1.0 }; // ピンク
		double Color6[4]	= { 0.0, 0.5, 1.0, 1.0 }; // 紺
		double Color7[4]	= { 1.0, 0.0, 0.5, 1.0 }; // 赤紫
		double Color8[4]	= { 1.0, 0.0, 0.0, 1.0 }; // 赤
		double Color9[4]	= { 0.5, 0.25, 0.0, 1.0 }; // 茶
		vColor.push_back(Color1);
		vColor.push_back(Color2);
		vColor.push_back(Color3);
		vColor.push_back(Color4);
		vColor.push_back(Color5);
		vColor.push_back(Color6);
		vColor.push_back(Color7);
		vColor.push_back(Color8);
		vColor.push_back(Color9);
		size_t ColorIndex = 0;

		map<double, vector<vector<GCode::CPoint*> > >::reverse_iterator rit = SpeedPathMap.rbegin();
		for(;rit != SpeedPathMap.rend(); ++rit)
		{
			vector<vector<GCode::CPoint*>>& vvPoint = rit->second;
			double* Color = vColor[ColorIndex];
			fout << "color\t" << Color[0] << ", " << Color[1] << ", " << Color[2] << ", " << Color[3] << "\n";

			for(size_t i = 0; i < vvPoint.size(); ++i)
			{
				fout << "begin\n";
				for(size_t j = 0; j < vvPoint[i].size(); ++j)
				{
					fout << vvPoint[i][j]->m_X << "," << vvPoint[i][j]->m_Y << "," << vvPoint[i][j]->m_Z << "\n";
				}
				fout << "end\n";
			}

			++ColorIndex;
			if(ColorIndex >= vColor.size())
			{
				ColorIndex -= vColor.size();
			}
		}

	}
}

static size_t 
GetMaxPartSize(const vector<GCode::MeasureResult>& vResult)
{
	size_t size = 0;
	for(size_t i = 0; i < vResult.size(); i++)
	{
		if(size < vResult[i].m_vSpeedPart.size())
		{
			size = vResult[i].m_vSpeedPart.size();
		}
	}
	return size;
}

static string
GeMillingTimeString(const double MillingTimeMin)
{
	const double MillingTimeSec = 60 * MillingTimeMin;
	const int h = (int)(MillingTimeSec / 3600.0);
	const int m = (int)((MillingTimeSec - h * 3600.0)/ 60.0);
	const int s = (int)(MillingTimeSec - h * 3600.0 - m * 60.0);

	stringstream sst;
	sst << h << ":" << m << ":" << s;
	return sst.str();
}

static double 
GetSpeed(const double Speed, const double UseRapidSpeed)
{
	if(fabs(Speed - GCode::RAPIDSPEED) < GCode::Eps)
	{
		return UseRapidSpeed;
	}

	return Speed;
}

void
DisplaymeasureResult(const string& FolderPath, 
					 const vector<GCode::MeasureResult>& vResult)
{
	const string OutputFilePath = GetFilePath(FolderPath, "Result.html");
	ofstream fout(OutputFilePath.c_str());
	if(!fout)
	{
		return;
	}

	const double UseRapidSpeed = 1000.0;
	const double RapidSpeedConvertRatio = GCode::RAPIDSPEED / UseRapidSpeed;
	const size_t MaxPartSize = GetMaxPartSize(vResult);

	fout << "<!DOCTYPE html>\n";
	fout << "<html>\n";
	fout << "<head>\n";
	fout << "<meta chatset=\"sjis\">\n";
	fout << "<title>加工時間</title>\n";
	fout << "<style type=\"text/css\">\n";
	fout << "body {\n";
	fout << "	background-color: #ADD8E6;\n";
	fout << "}\n";
	fout << "thead{\n";
	fout << "	background-color: #4682B4;\n";
	fout << "   text-align: center;\n";
	fout << "   color: White;\n";
	fout << "   font-weight: bold;\n";
	fout << "}\n";
	fout << "table{\n";
	fout << "   border: 1px solid #191970;\n";
	fout << "   border-collapse: collapse;\n";
	fout << "   border-spacing: 0;\n";
	fout << "}\n";
	fout << "tbody {\n";
	fout << "	background-color: #FFFFFF;\n";
	fout << "}\n";
	fout << "td {\n";
	fout << "	border: 1px solid #000000;\n";
	fout << "}\n";
	fout << "td#attrib_speed {\n";
	fout << "	background-color: #FFD700;\n";
	fout << "}\n";
	fout << "td#attrib_length {\n";
	fout << "	background-color: #FFE4E1;\n";
	fout << "}\n";
	fout << "td#attrib_time {\n";
	fout << "	background-color: #98FB98;\n";
	fout << "}\n";
	fout << "</style>\n";
	fout << "</head>\n";
	fout << "<body>\n";
	fout << "<table border=\"1px solid\">\n";
	fout << "<thead>\n";
	fout << "<tr>\n";
	fout << "<td>NCファイル名</td><td>属性</td><td>全体</td>";
	fout << "<td>速度1(早送り)</td>";
	for(size_t i = 1; i < MaxPartSize; ++i)
	{
		fout << "<td>速度" << i + 1 << "</td>";
	}
	fout << "\n";
	fout << "</tr>\n";
	fout << "</thead>\n";
	fout << "<tbody>\n";
	for(size_t i = 0; i < vResult.size(); ++i)
	{
		fout << "<tr>";
		fout << "<td rowspan=\"3\">" << vResult[i].m_NCFileName << "</td>";
		fout << "<td id=\"attrib_speed\">速度(mm/min)</td><td></td>";
		for(size_t j = 0; j < vResult[i].m_vSpeedPart.size(); j++)
		{
			fout << "<td>" << GetSpeed(vResult[i].m_vSpeedPart[j].m_Speed, UseRapidSpeed) << "</td>";
		}
		fout << "</tr>\n";

		fout << "<tr>";
		fout << "<td id=\"attrib_length\">距離(mm)</td>";
		fout << "<td>" << vResult[i].m_Length << "</td>";
		for(size_t j = 0; j < vResult[i].m_vSpeedPart.size(); j++)
		{
			fout << "<td>" << vResult[i].m_vSpeedPart[j].m_Length << "</td>";
		}
		fout << "</tr>\n";

		fout << "<tr>";
		fout << "<td id=\"attrib_time\">加工時間(h:m:s)</td>";
		fout << "<td>" << GeMillingTimeString(vResult[i].m_MillingTime + (RapidSpeedConvertRatio - 1) * vResult[i].m_vSpeedPart[0].m_MillingTime) << "</td>";
		fout << "<td>" << GeMillingTimeString(RapidSpeedConvertRatio * vResult[i].m_vSpeedPart[0].m_MillingTime) << "</td>";
		for(size_t j = 1; j < vResult[i].m_vSpeedPart.size(); j++)
		{
			fout << "<td>" << GeMillingTimeString(vResult[i].m_vSpeedPart[j].m_MillingTime) << "</td>";
		}
		fout << "</tr>\n";
	}
	fout << "</tbody>\n";
	fout << "</table>\n";
	fout << "</body>\n";
	fout << "</html>\n";
}