#pragma once
#include <string>
#include <vector>
#include "GCodeLine.h"
#include "StatusType.h"
#include "CGCodePoint.h"
using namespace std;

namespace GCode
{

class GCodeHandler
{
public:
	explicit GCodeHandler(const std::string& NCFilePath);

public:

	struct GCodeStructure
	{
		typedef vector<CGcodeLine> GCodeSection;

		GCodeStructure() : m_DecimalPlace(3)
		{}

		GCodeSection m_vHeaderLines;
		GCodeSection m_vContentLines;
		GCodeSection m_vFooterLines;
		int m_DecimalPlace;
	};

	GCodeStructure::GCodeSection& GetHeader()	{ return m_Structure.m_vHeaderLines; }
	GCodeStructure::GCodeSection& GetContent()	{ return m_Structure.m_vContentLines; }
	GCodeStructure::GCodeSection& GetFooter()	{ return m_Structure.m_vFooterLines; }
	void GetAllGCodeLine(GCodeStructure::GCodeSection& vGCodeLine) const;

	int GetDecimalPlace() const { return m_Structure.m_DecimalPlace; }

private:

	int getNumLine(const string& FilePath);
	int getDecimalPlaceNum(const string& Line, const string& DecimalPlaceSt);

	GCode::StatusType Load(const string& NCFilePath, 
							 const string& HeaderSt, 
							 const string& FooterSt,
							 const string& DecimalPlaceSt,
							 vector<CGcodeLine>& vHeaderLines, 
							 vector<CGcodeLine>& vContentLine,
							 vector<CGcodeLine>& vFooterLine,
							 int& DecimalPlace);

public:
	GCode::StatusType CreateGCodeStructure();
	/*void WriteOutHeader(ofstream& fout);
	void WriteOutContent(ofstream& fout);
	void WriteOutFooter(ofstream& fout);
	void SetSeparatorSt(const string& HeaderSt, const string& FooterSt);
	GCode::StatusType ChangeFirstEvasionHeight(const double Height);
	GCode::StatusType ChangeLastEvasionHeight(const double Height);
	GCode::StatusType GetEndXYZOfFirstEvasion(double& X, double& Y, double& Z);
	GCode::StatusType GetStartXYZOfLastEvasion(double& X, double& Y, double& Z);
	void DeleteLastInitialXYLine();*/

	GCode::StatusType CreatePoint(const GCode::CGcodeLine& PreviousLine, 
								  const GCode::CGcodeLine& CurrentLine,
								  vector<GCode::CPoint*>& vPoint) const;
	void Dump(ofstream& fout) const;

private:
	const std::string m_NCFilePath;
	std::string m_HeaderSt;
	std::string m_FooterSt;
	std::string m_DecimalPlaceSt;

	GCodeStructure m_Structure;
};

} // namespace GCode