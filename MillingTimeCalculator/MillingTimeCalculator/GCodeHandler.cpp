#include "GCodeHandler.h"
#include "GCodeLineParser.h"
#include "InterpolateCirclePoint.h"
#include "MachineFileSetting.h"

namespace GCode
{

GCodeHandler::GCodeHandler(const std::string& NCFilePath)
	:m_NCFilePath(NCFilePath), m_HeaderSt("NC_HEADER"), m_FooterSt("NC_FOOTER"), m_DecimalPlaceSt("NCDECIMALPLACE")
{}

void
GCodeHandler::GetAllGCodeLine(GCodeStructure::GCodeSection& vGCodeLine) const
{
	vGCodeLine.clear();
	const GCodeStructure::GCodeSection& vH = m_Structure.m_vHeaderLines;
	const GCodeStructure::GCodeSection& vC = m_Structure.m_vContentLines;
	const GCodeStructure::GCodeSection& vF = m_Structure.m_vFooterLines;

	size_t Size = vH.size() + vC.size() + vF.size();
	vGCodeLine.reserve(Size);
	vGCodeLine.insert(vGCodeLine.end(), vH.begin(), vH.end());
	vGCodeLine.insert(vGCodeLine.end(), vC.begin(), vC.end());
	vGCodeLine.insert(vGCodeLine.end(), vF.begin(), vF.end());
}

int 
GCodeHandler::getNumLine(const string& FilePath)
{
	ifstream fin(FilePath.c_str());
	if(!fin) 
		return 0;

	int num = 0;
	char buf[256];
	while(fin.getline(buf, 256)) 
		num++;

	return num;
}

int 
GCodeHandler::getDecimalPlaceNum(const string& Line, const string& DecimalPlaceSt)
{
	int decimalplace = 3;
	string::size_type pos = Line.find(")");
	if(string::npos != pos)
	{
		char num = (char)Line[pos - 1];
		decimalplace = atoi(&num);
	}
	return decimalplace;
}

GCode::StatusType 
GCodeHandler::Load(const string& NCFilePath, 
	 const string& HeaderSt, 
	 const string& FooterSt,
	 const string& DecimalPlaceSt,
	 vector<CGcodeLine>& vHeaderLines, 
	 vector<CGcodeLine>& vContentLine,
	 vector<CGcodeLine>& vFooterLine,
	 int& DecimalPlace)
{
	vHeaderLines.clear();
	vContentLine.clear();
	vFooterLine.clear();
	int num = getNumLine(NCFilePath);
	vContentLine.reserve(num);

	ifstream fin(NCFilePath.c_str());
	if(!fin)
	{
		return GCode::BAD_PARAM;
	}

	bool HeaderPart = true;
	bool FooterPart = false;

	CLineState Current;
	while(!fin.eof())
	{
		string stLine;
		getline(fin, stLine);
		if(stLine.empty())
		{
			continue;
		}

		if(string::npos != stLine.find(HeaderSt))
		{			
			vHeaderLines.push_back(CGcodeLine(stLine, CLineState(), Current));
			HeaderPart = false;
			continue;
		}
		if(string::npos != stLine.find(FooterSt))
		{
			FooterPart = true;  
			vFooterLine.push_back(CGcodeLine(stLine, CLineState(), Current));
			continue;
		}
		if(string::npos != stLine.find(DecimalPlaceSt))
		{
			DecimalPlace = getDecimalPlaceNum(stLine, DecimalPlaceSt);
			vContentLine.push_back(CGcodeLine(stLine, CLineState(), Current));
			continue;
		}

		CLineState RelState = GCodeLineParser::Parse(stLine);
		Current += RelState;
		CGcodeLine Line(stLine, RelState, Current);

		/*{
			ofstream fout("Line.txt", ios::app);
			Line.Dump(fout);
		}*/

		if(HeaderPart)					vHeaderLines.push_back(Line);
		if(!HeaderPart && !FooterPart)	vContentLine.push_back(Line);
		if(FooterPart)					vFooterLine.push_back(Line);
	}

	fin.close();

	return GCode::NOERROR;
}

GCode::StatusType 
GCodeHandler::CreateGCodeStructure()
{
	GetMachinePathSetting(m_NCFilePath, m_HeaderSt, m_FooterSt, string());

	GCode::StatusType Status = Load(m_NCFilePath, 
									 m_HeaderSt, 
									 m_FooterSt,
									 m_DecimalPlaceSt,
									 m_Structure.m_vHeaderLines,
									 m_Structure.m_vContentLines,
									 m_Structure.m_vFooterLines,
									 m_Structure.m_DecimalPlace);
	if(GCode::NOERROR != Status)
	{
		return Status;
	}

	return GCode::NOERROR;
}

//void 
//GCodeHandler::WriteOutHeader(ofstream& fout)
//{
//	for(size_t i = 0; i < m_Structure.m_vHeaderLines.size(); i++)
//	{
//		m_Structure.m_vHeaderLines[i].WriteLine(fout);
//	}
//}
//
//void 
//GCodeHandler::WriteOutContent(ofstream& fout)
//{
//	for(size_t i = 0; i < m_Structure.m_vContentLines.size(); i++)
//	{
//		m_Structure.m_vContentLines[i].WriteLine(fout);
//	}
//}
//
//void 
//GCodeHandler::WriteOutFooter(ofstream& fout)
//{
//	for(size_t i = 0; i < m_Structure.m_vFooterLines.size(); i++)
//	{
//		m_Structure.m_vFooterLines[i].WriteLine(fout);
//	}
//}
//
//void 
//GCodeHandler::SetSeparatorSt(const string& HeaderSt, const string& FooterSt)
//{
//	m_HeaderSt = HeaderSt;
//	m_FooterSt = FooterSt;
//}
//
//GCode::StatusType 
//GCodeHandler::ChangeFirstEvasionHeight(const double Height)
//{
//	// 空の場合は変更できない
//	if(m_Structure.m_vContentLines.empty())
//	{
//		return GCode::NOERROR;
//	}
//
//	// 早送りから始まらなければならない
//	if(CLineState::G00 != m_Structure.m_vContentLines.front().GetInterpolationType())
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	// contentの中からZの記述のある最初の行を探す
//	size_t pos = 0;
//	while((pos < m_Structure.m_vContentLines.size()) 
//		  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType())
//		  && !m_Structure.m_vContentLines[pos].HasZ())
//		pos++;
//
//	if(pos == m_Structure.m_vContentLines.size())
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	if(CLineState::G00 != m_Structure.m_vContentLines[pos].GetInterpolationType())
//	{
//		// ここにくるときは加工開始Z値と工具移動高さが同じでZが省略されているとき
//
//		pos = 0;
//		while(pos < m_Structure.m_vContentLines.size()
//			  && !m_Structure.m_vContentLines[pos].IsValid())
//			pos++;
//
//		CLineState ZState, XYState;
//		ZState.SetValue('Z', Height);
//		XYState.SetValue('X', m_Structure.m_vContentLines.front().GetX());
//		XYState.SetValue('Y', m_Structure.m_vContentLines.front().GetY());
//		CGcodeLine ZLine(string(), ZState, ZState);
//		CGcodeLine XYLine(string(), XYState, XYState);
//		ZLine.ChangeRelativeState(ZState, m_Structure.m_DecimalPlace);
//		XYLine.ChangeRelativeState(XYState, m_Structure.m_DecimalPlace);
//		m_Structure.m_vContentLines.insert(m_Structure.m_vContentLines.begin() + pos, ZLine);
//		m_Structure.m_vContentLines.insert(m_Structure.m_vContentLines.begin() + pos + 1, XYLine);
//	}
//	else
//	{
//		size_t zpos = pos;
//
//		CLineState HeightState;
//		HeightState.SetValue('Z', Height);
//		m_Structure.m_vContentLines[zpos].ChangeRelativeState(HeightState, m_Structure.m_DecimalPlace);
//	}
//
//	// 最初の早送り部分だけ絶対評価系を再構築
//	CLineState Current = m_Structure.m_vHeaderLines.back().GetAbsoluteState();
//	for(size_t i = 0; i < m_Structure.m_vContentLines.size(); i++)
//	{
//		if(CLineState::G00 != m_Structure.m_vContentLines[i].GetInterpolationType())
//		{
//			break;
//		}
//
//		Current += m_Structure.m_vContentLines[i].GetRelativeState();
//		m_Structure.m_vContentLines[i].SetAbsoluteState(Current);				
//	}
//
//	return GCode::NOERROR;
//}
//
//GCode::StatusType 
//GCodeHandler::ChangeLastEvasionHeight(const double Height)
//{
//	// 空の場合は変更できない
//	if(m_Structure.m_vContentLines.empty())
//	{
//		return GCode::NOERROR;
//	}
//
//	// G00で終わらなければならない
//	if(CLineState::G00 != m_Structure.m_vContentLines.back().GetInterpolationType())
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	int pos = m_Structure.m_vContentLines.size() - 1;
//	while(pos >= 0
//		  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType())
//		  && (!m_Structure.m_vContentLines[pos].HasZ()))
//		pos--;
//
//	if(pos < 0)
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	int zpos = pos;
//
//	CLineState HeightState;
//	HeightState.SetValue('Z', Height);
//	m_Structure.m_vContentLines[zpos].ChangeRelativeState(HeightState, m_Structure.m_DecimalPlace);
//
//	// pos以降の最初の早送り部分だけ絶対評価系を再構築
//	CLineState Current = m_Structure.m_vContentLines[zpos -1].GetAbsoluteState();
//	for(size_t i = zpos; i < m_Structure.m_vContentLines.size(); i++)
//	{
//		if(CLineState::G00 != m_Structure.m_vContentLines[i].GetInterpolationType())
//		{
//			break;
//		}
//
//		Current += m_Structure.m_vContentLines[i].GetRelativeState();
//		m_Structure.m_vContentLines[i].SetAbsoluteState(Current);				
//	}
//
//	return GCode::NOERROR;
//}
//
//GCode::StatusType 
//GCodeHandler::GetEndXYZOfFirstEvasion(double& X, double& Y, double& Z)
//{
//	// 空の場合は取得できない
//	if(m_Structure.m_vContentLines.empty())
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	// 早送りから始まらなければならない
//	if(CLineState::G00 != m_Structure.m_vContentLines.front().GetInterpolationType())
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	// G00でないところまで進める
//	size_t pos = 0;
//	while((pos < m_Structure.m_vContentLines.size()) 
//		  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType()))
//		pos++;
//
//	if(pos == m_Structure.m_vContentLines.size())
//	{
//		return GCode::BAD_PARAM;
//	}
//	
//	X = m_Structure.m_vContentLines[pos - 1].GetX();
//	Y = m_Structure.m_vContentLines[pos - 1].GetY();
//	Z = m_Structure.m_vContentLines[pos - 1].GetZ();
//
//	return GCode::NOERROR;
//}
//
//GCode::StatusType 
//GCodeHandler::GetStartXYZOfLastEvasion(double& X, double& Y, double& Z)
//{
//	// 空の場合は取得できない
//	if(m_Structure.m_vContentLines.empty())
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	// G00で終わらなければならない
//	if(CLineState::G00 != m_Structure.m_vContentLines.back().GetInterpolationType())
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	int pos = m_Structure.m_vContentLines.size() - 1;
//	while(pos >= 0
//		  && (CLineState::G00 == m_Structure.m_vContentLines[pos].GetInterpolationType()))
//		pos--;
//
//	if(pos < 0)
//	{
//		return GCode::BAD_PARAM;
//	}
//
//	X = m_Structure.m_vContentLines[pos].GetX();
//	Y = m_Structure.m_vContentLines[pos].GetY();
//	Z = m_Structure.m_vContentLines[pos].GetZ();
//
//	return GCode::NOERROR;
//}
//
//void 
//GCodeHandler::DeleteLastInitialXYLine()
//{
//	const double InitialX = m_Structure.m_vContentLines.front().GetX();
//	const double InitialY = m_Structure.m_vContentLines.front().GetY();
//
//	int i = m_Structure.m_vContentLines.size() - 1;
//
//	while((m_Structure.m_vContentLines[i].GetX() == InitialX) 
//		  && (m_Structure.m_vContentLines[i].GetY() == InitialY))
//		i--;
//	i++;
//
//	m_Structure.m_vContentLines.erase(m_Structure.m_vContentLines.begin() + i);			
//}

static bool
DeterminedXYZ(const GCode::CGcodeLine& Line)
{
	return (Line.HasAbsX() && Line.HasAbsY() && Line.HasAbsZ());
}

static bool
IsXYZChanged(const GCode::CGcodeLine& PreviousLine, 
		   const GCode::CGcodeLine& CurrentLine)
{
	// XYZが存在するようになった
	if(!DeterminedXYZ(PreviousLine) && DeterminedXYZ(CurrentLine))
	{
		return true;
	}

	// XYZが更新された
	if(PreviousLine.GetX() != CurrentLine.GetX()
		|| PreviousLine.GetY() != CurrentLine.GetY()
		|| PreviousLine.GetZ() != CurrentLine.GetZ())
	{
		return true;
	}

	return false;
}

static bool 
IsChangeed(const GCode::CGcodeLine& PreviousLine, 
		   const GCode::CGcodeLine& CurrentLine)
{
	// XYZが更新された
	if(IsXYZChanged(PreviousLine, CurrentLine))
	{
		return true;
	}

	// (G00/G01/G02/G03）が更新された
	if(PreviousLine.GetInterpolationType() != CurrentLine.GetInterpolationType())
	{
		return true;
	}

	// F値が見つかった
	if(!PreviousLine.HasAbsF() && CurrentLine.HasAbsF())
	{
		return true;
	}

	// F値が更新された
	if(PreviousLine.HasAbsF() && CurrentLine.HasF()
		&& (PreviousLine.GetF() != CurrentLine.GetF()))
	{
		return true;
	}

	return false;
}

static GCode::StatusType
InterpolateCirclePoint(const GCode::CGcodeLine& PreviousLine, 
					   const GCode::CGcodeLine& CurrentLine,
					   vector<GCode::CPoint*>& vPoint)
{
	if(!DeterminedXYZ(PreviousLine) || !DeterminedXYZ(CurrentLine))
	{
		return GCode::NOERROR;
	}

	const double Precision = 0.001;	// 取りあえず適当な値を入れる

	CPoint3D<double> PreviousPoint(PreviousLine.GetX(), PreviousLine.GetY(), PreviousLine.GetZ());
	CPoint3D<double> CurrentPoint(CurrentLine.GetX(), CurrentLine.GetY(), CurrentLine.GetZ());
	CPoint3D<double> Center = CurrentPoint;

	// XY平面
	if(GCode::CLineState::G17 == CurrentLine.GetPlaneType())
	{
		if(!CurrentLine.HasAbsI() || !CurrentLine.HasAbsJ())
		{
			return GCode::BAD_PARAM;
		}

		Center.m_X = PreviousPoint.m_X + CurrentLine.GetI();
		Center.m_Y = PreviousPoint.m_Y + CurrentLine.GetJ();
	}

	// XZ平面
	else if(GCode::CLineState::G18 == CurrentLine.GetPlaneType())
	{
		if(!CurrentLine.HasAbsI() || !CurrentLine.HasAbsK())
		{
			return GCode::BAD_PARAM;
		}

		Center.m_X = PreviousPoint.m_X + CurrentLine.GetI();
		Center.m_Z = PreviousPoint.m_Z + CurrentLine.GetK();
	}

	// YZ平面
	else if(GCode::CLineState::G19 == CurrentLine.GetPlaneType())
	{
		if(!CurrentLine.HasAbsJ() || !CurrentLine.HasAbsK())
		{
			return GCode::BAD_PARAM;
		}

		Center.m_Y = PreviousPoint.m_Y + CurrentLine.GetJ();
		Center.m_Z = PreviousPoint.m_Z + CurrentLine.GetK();
	}

	else
	{
		return GCode::BAD_PARAM;
	}

	GCode::StatusType Status = GCode::NOERROR;

	Status = CreateInterpolationPoint(PreviousPoint, 
									  CurrentPoint, 
									  Center, 
									  CurrentLine.GetInterpolationType(), 
									  CurrentLine.GetPlaneType(),
									  CurrentLine.GetF(),
									  Precision,
									  vPoint);
	if(GCode::NOERROR != Status)
	{
		return Status;
	}

	return GCode::NOERROR;
}

GCode::StatusType 
GCodeHandler::CreatePoint(const GCode::CGcodeLine& PreviousLine, 
						  const GCode::CGcodeLine& CurrentLine,
						  vector<GCode::CPoint*>& vPoint) const
{
	vPoint.clear();

	// X,Y,Zのどれかが定まっていなければ終了
	if(!DeterminedXYZ(CurrentLine))
	{
		return GCode::NOERROR;
	}

	// XYZが更新されていない
	if(!IsXYZChanged(PreviousLine, CurrentLine))
	{
		return GCode::NOERROR;
	}

	double F = GCode::RAPIDSPEED;
	if((GCode::CLineState::G00 != CurrentLine.GetInterpolationType())
		&& CurrentLine.HasAbsF())
	{
		F = CurrentLine.GetF();
	}

	// G02, G03の場合は点を補間して返す
	if(GCode::CLineState::G02 == CurrentLine.GetInterpolationType()
		|| GCode::CLineState::G03 == CurrentLine.GetInterpolationType())
	{
		return InterpolateCirclePoint(PreviousLine, CurrentLine, vPoint);
	}

	double X = CurrentLine.GetX();
	double Y = CurrentLine.GetY();
	double Z = CurrentLine.GetZ();
	GCode::CPoint* Point = new GCode::CPoint(X, Y, Z);
	Point->m_Speed = F;
	Point->m_IType = CurrentLine.GetInterpolationType();

	vPoint.push_back(Point);	

	return GCode::NOERROR;
}

void 
GCodeHandler::Dump(ofstream& fout) const
{
	for(size_t i = 0; i < m_Structure.m_vHeaderLines.size(); i++)
	{
		m_Structure.m_vHeaderLines[i].Dump(fout);
	}

	for(size_t i = 0; i < m_Structure.m_vContentLines.size(); i++)
	{
		m_Structure.m_vContentLines[i].Dump(fout);
	}

	for(size_t i = 0; i < m_Structure.m_vFooterLines.size(); i++)
	{
		m_Structure.m_vFooterLines[i].Dump(fout);
	}
}


} // namespace GCode