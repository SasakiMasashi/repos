#pragma once
#include "Thread/Core/IThreadFunctor.h"
#include <Windows.h>
#include <string>
#include <sstream>
#include <iostream>

namespace ThreadFunctor
{

class Timer : public IThreadFunctor
{
public:
	enum TimeMode
	{
		FULL,
		DATE,
		TIME
	};

	Timer()
		:m_IsAlive(true),
		 m_IsSuspend(false),
		 m_TimeMode(FULL)
	{}

	void Run()
	{
		while(m_IsAlive)
		{
			if(!m_IsSuspend)
			{
				std::cout << getCurrentTimeString() << "\n";
			}
			Sleep(1000);
		}
	}

	void End()
	{
		m_IsAlive = false;
	}

	void ChangeSuspendState(bool isSuspend)
	{
		m_IsSuspend = isSuspend;
	}

	void ChangeTimeMode(TimeMode mode)
	{
		m_TimeMode = mode;
	}

	bool IsAlive() { return m_IsAlive; }
	
private:
	std::string getCurrentTimeString()
	{
		switch(m_TimeMode)
		{
		case FULL: return getFullTimeString();
		case DATE: return getDateString();
		case TIME: return getTimeString();
		}

		return "";
	}

	std::string getFullTimeString()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		std::stringstream sst;
		sst << st.wYear << "." 
			<< st.wMonth << "."
			<< st.wDay << "-"
			<< st.wHour << "."
			<< st.wMinute << "."
			<< st.wSecond << "."
			<< st.wMilliseconds;

		return sst.str();
	}

	std::string getDateString()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		std::stringstream sst;
		sst << st.wYear << "/" 
			<< st.wMonth << "/"
			<< st.wDay;

		return sst.str();
	}

	std::string getTimeString()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		std::stringstream sst;
		sst << st.wHour << ":"
			<< st.wMinute << ":"
			<< st.wSecond << "."
			<< st.wMilliseconds;

		return sst.str();
	}

private:
	bool m_IsAlive;
	bool m_IsSuspend;
	TimeMode m_TimeMode;
};

} // namespace ThreadFunctor