#pragma once
#include "Thread/Core/IThreadFunctor.h"
#include "Thread/Functor/Timer.h"
#include <sstream>
#include <iostream>

namespace ThreadFunctor
{

class TimerManager : public IThreadFunctor
{
public:
	TimerManager(ThreadFunctor::Timer* func)
		:m_TimerFunctor(func)
	{}

	void Run()
	{
		while(m_TimerFunctor->IsAlive())
		{
			std::string Line;
			std::cin >> Line;

			char c = '\0';
			if(!Line.empty())
			{
				c = Line[0];
			}

			OnKeyDown(c);
		}
	}

private:
	void OnKeyDown(char eventCode)
	{
		switch(eventCode)
		{
		case 'r':
			m_TimerFunctor->ChangeSuspendState(false);
			break;
		case 's':
			m_TimerFunctor->ChangeSuspendState(true);
			break;
		case 'f':
			m_TimerFunctor->ChangeTimeMode(ThreadFunctor::Timer::FULL);
			break;
		case 'd':
			m_TimerFunctor->ChangeTimeMode(ThreadFunctor::Timer::DATE);
			break;
		case 't':
			m_TimerFunctor->ChangeTimeMode(ThreadFunctor::Timer::TIME);
			break;
		case 'q':
			m_TimerFunctor->End();
			break;
		default:
			{
				std::stringstream sst;
				sst << "使い方\n"
					<< "r (run)    : 時刻表示を開始します。\n"
					<< "s (suspend): 時刻表示を一時停止します。\n"
					<< "f (full)   : 時刻の表示形式を“日付＋時刻”にします。\n"
					<< "d (date)   : 時刻の表示形式を“日付のみ”にします。\n"
					<< "t (time)   : 時刻の表示形式を“時刻のみ”にします。\n"
					<< "q (quit)   : プログラムを終了します。\n";
				std::cout << sst.str();
			}
			break;
		}
	}

private:
	ThreadFunctor::Timer* m_TimerFunctor;
};

} // namespace ThreadFunctor
