#pragma once
#include <vector>
#include <string>
using namespace std;

namespace GCode
{
struct MeasureResult
{
	struct SpeedPart
	{
		SpeedPart()
			:m_Speed(0.0),
			 m_Length(0.0),
			 m_MillingTime(0.0)
		{}

		double m_Speed;
		double m_Length;
		double m_MillingTime;
	};

	MeasureResult()
		:m_Length(0.0),
		 m_MillingTime(0.0),
		 m_vSpeedPart()
	{}

	string m_NCFolderPath;
	string m_NCFileName;

	double m_Length;
	double m_MillingTime;
	vector<SpeedPart> m_vSpeedPart;
};

} // namespace GCode