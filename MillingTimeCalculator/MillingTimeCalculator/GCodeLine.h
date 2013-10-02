#pragma once
#include <string>
#include "CLineState.h"
using namespace std;

namespace GCode
{

class CGcodeLine
{
public:
	CGcodeLine(const string& Line, const CLineState& RelativeState, const CLineState& AbsoluteState);

	/*explicit CGcodeLine(const string& Line)
		:m_Line(Line),
		 m_AbsoluteState()
	{
		m_RelativeState = GCodeLineParser::Parse(Line);
	}*/

	/*explicit CGcodeLine( const CLineState& RelativeState)
		:m_RelativeState(RelativeState),
		 m_AbsoluteState()
	{
		m_Line = CreateLine();
	}*/

public:
	// 1行の中に各文字が出現するかどうか
	bool HasX() const { return (!!m_RelativeState.m_X); }
	bool HasY() const { return (!!m_RelativeState.m_Y); }
	bool HasZ() const { return (!!m_RelativeState.m_Z); }
	bool HasF() const { return (!!m_RelativeState.m_F); }
	bool HasI() const { return (!!m_RelativeState.m_I); }
	bool HasJ() const { return (!!m_RelativeState.m_J); }
	bool HasK() const { return (!!m_RelativeState.m_K); }
	bool HasG00() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G00); }
	bool HasG01() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G01); }
	bool HasG02() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G02); }
	bool HasG03() const { return (!!m_RelativeState.m_IType) && (m_RelativeState.m_IType.get() == CLineState::G03); }
	bool HasG17() const { return (!!m_RelativeState.m_PType) && (m_RelativeState.m_IType.get() == CLineState::G17); }
	bool HasG18() const { return (!!m_RelativeState.m_PType) && (m_RelativeState.m_IType.get() == CLineState::G18); }
	bool HasG19() const { return (!!m_RelativeState.m_PType) && (m_RelativeState.m_IType.get() == CLineState::G19); }
	bool HasInterplationCommand() const { return (!!m_RelativeState.m_IType);}
	bool HasPlaneSpecifyCommand() const { return (!!m_RelativeState.m_PType);}

	// その行までに定まった値
	bool HasAbsX() const { return (!!m_AbsoluteState.m_X); }
	bool HasAbsY() const { return (!!m_AbsoluteState.m_Y); }
	bool HasAbsZ() const { return (!!m_AbsoluteState.m_Z); }
	bool HasAbsF() const { return (!!m_AbsoluteState.m_F); }
	bool HasAbsI() const { return (!!m_AbsoluteState.m_I); }
	bool HasAbsJ() const { return (!!m_AbsoluteState.m_J); }
	bool HasAbsK() const { return (!!m_AbsoluteState.m_K); }
	bool HasAbsG00() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G00); }
	bool HasAbsG01() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G01); }
	bool HasAbsG02() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G02); }
	bool HasAbsG03() const { return (!!m_AbsoluteState.m_IType) && (m_AbsoluteState.m_IType.get() == CLineState::G03); }
	bool HasAbsG17() const { return (!!m_AbsoluteState.m_PType) && (m_AbsoluteState.m_IType.get() == CLineState::G17); }
	bool HasAbsG18() const { return (!!m_AbsoluteState.m_PType) && (m_AbsoluteState.m_IType.get() == CLineState::G18); }
	bool HasAbsG19() const { return (!!m_AbsoluteState.m_PType) && (m_AbsoluteState.m_IType.get() == CLineState::G19); }
	bool HasAbsInterplationCommand() const { return (!!m_AbsoluteState.m_IType);}
	bool HasAbsPlaneSpecifyCommand() const { return (!!m_AbsoluteState.m_PType);}

	// 以下値が取れることを確認してから使用してください
	double GetX() const { return m_AbsoluteState.m_X.get(); }
	double GetY() const { return m_AbsoluteState.m_Y.get(); }
	double GetZ() const { return m_AbsoluteState.m_Z.get(); }
	double GetI() const { return m_AbsoluteState.m_I.get(); }
	double GetJ() const { return m_AbsoluteState.m_J.get(); }
	double GetK() const { return m_AbsoluteState.m_K.get(); }
	double GetF() const { return m_AbsoluteState.m_F.get(); }

	CLineState::InterpolationType GetInterpolationType() const { return m_AbsoluteState.m_IType.get(); }
	std::string GetInterpolationTypeString() const;
	CLineState::PlaneType GetPlaneType() const { return m_AbsoluteState.m_PType.get(); }
	std::string GetPlaneTypeString() const;
	string CreateLine(const int DecimalPlace) const;
	void ChangeRelativeState(const CLineState& AddState, const int DecimalPlace);
	void SetValue(const char ch, const double value, const int DecimalPlace);

	bool IsValid() const;

	void ClearAbsoluteState() {	m_AbsoluteState = CLineState(); }

	std::string GetLineSt() const { return m_Line; }
	const CLineState& GetRelativeState() const { return m_RelativeState; }
	const CLineState& GetAbsoluteState() const { return m_AbsoluteState; }
	void SetRelativeState(const CLineState& State) { m_RelativeState = State; }
	void SetAbsoluteState(const CLineState& State) { m_AbsoluteState = State; }

	void WriteLine(ofstream& fout);

	void Dump(ofstream& fout) const;
private:
	std::string m_Line;
	CLineState	m_RelativeState;
	CLineState	m_AbsoluteState;
};

} // namespace GCode