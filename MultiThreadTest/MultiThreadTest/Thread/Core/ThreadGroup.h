#pragma once
#include <vector>

class Thread;

class ThreadGroup
{
public:
	ThreadGroup();
	explicit ThreadGroup(const size_t Size);

	void Add(Thread* thread);
	void Start();
	bool WaitAll();
	bool WaitOneOfAll();

private:
	bool WaitImple(bool isWaitAll);

private:
	std::vector<Thread*> m_vThread;
};