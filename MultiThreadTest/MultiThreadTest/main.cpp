
#include <io.h>
#include <conio.h>
#include "Thread/Core/Thread.h"
#include "Thread/Core/ThreadGroup.h"
#include "Thread/Functor/Counter.h"
#include "Thread/Functor/Timer.h"
#include "Thread/Functor/TimerManager.h"
#include "Communication/Unit.h"

void Test_Counter()
{
	ThreadGroup threadGr;
	threadGr.Add(new Thread(new ThreadFunctor::Counter(9999, "thread 1 ")));
	threadGr.Add(new Thread(new ThreadFunctor::Counter(5000, "thread 2 ")));
	threadGr.Add(new Thread(new ThreadFunctor::Counter(7000, "thread 3 ")));
	threadGr.Add(new Thread(new ThreadFunctor::Counter(4000, "thread 4 ")));
	threadGr.Add(new Thread(new ThreadFunctor::Counter(8000, "thread 5 ")));

	threadGr.Start();
	threadGr.WaitAll();

	_getch();
}

void Test_Timer()
{
	ThreadGroup threadGr;
	ThreadFunctor::Timer* timerFunc = new ThreadFunctor::Timer();
	threadGr.Add(new Thread(timerFunc));									// 時刻スレッド
	threadGr.Add(new Thread(new ThreadFunctor::TimerManager(timerFunc)));	// 時刻管理スレッド

	threadGr.Start();
	threadGr.WaitAll();
}

int main(int argc, char* argv[])
{
	//Test_Counter();
	Test_Timer();
	
	return 0;
}