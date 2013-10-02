#pragma once
#include <boost/optional.hpp>
#include <fstream>
using namespace std;

namespace GCode
{
class CLineState
{
public:
	enum InterpolationType
	{
		G00,
		G01,
		G02,
		G03
	};

	enum PlaneType
	{
		G17,
		G18,
		G19
	};

	CLineState();
	CLineState(const CLineState& State);

	~CLineState();

	const CLineState& operator=(const CLineState& State);
	const CLineState& operator+=(const CLineState& State);
	CLineState operator+(const CLineState& State) const;

private:
	void AddState(const CLineState& From, CLineState& To) const;
	void SetState(const CLineState& From, CLineState& To) const;

public:
	void SetValue(const char C, const boost::optional<double> Val);
	void Dump(ofstream& fout) const;

public:
	boost::optional<double> m_X;
	boost::optional<double> m_Y;
	boost::optional<double> m_Z;
	boost::optional<double> m_F;
	boost::optional<double> m_I;
	boost::optional<double> m_J;
	boost::optional<double> m_K;
	boost::optional<InterpolationType> m_IType;
	boost::optional<PlaneType> m_PType;
};

} // namespace GCode