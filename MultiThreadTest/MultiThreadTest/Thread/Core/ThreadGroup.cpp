#include "ThreadGroup.h"
#include "Thread.h"

ThreadGroup::ThreadGroup()
	: m_vThread()
{
	m_vThread.reserve(10);
}

ThreadGroup::ThreadGroup(const size_t Size)
	: m_vThread()
{
	m_vThread.reserve(Size);
}

void ThreadGroup::Add(Thread* thread)
{
	if(!thread)
	{
		return;
	}

	m_vThread.push_back(thread);
}

void ThreadGroup::Start()
{
	for(std::vector<Thread*>::const_iterator it = m_vThread.begin();
		it != m_vThread.end();
		++it)
	{
		(*it)->Start();
	}
}

bool ThreadGroup::WaitAll()
{
	return WaitImple(true);
}

bool ThreadGroup::WaitOneOfAll()
{
	return WaitImple(false);
}

bool ThreadGroup::WaitImple(bool isWaitAll)
{
	if(m_vThread.empty())
	{
		return true;
	}

	// �n���h�����W�߂�
	HANDLE* handle = new HANDLE[m_vThread.size()];
	int counter = 0;
	for(std::vector<Thread*>::const_iterator it = m_vThread.begin();
		it != m_vThread.end();
		++it)
	{
		handle[counter] = (*it)->GetHandle();
		++counter;
	}

	BOOL isAll = isWaitAll ? TRUE : FALSE;

	// �X���b�h�̏I����҂�
	WaitForMultipleObjects(m_vThread.size(), handle, isAll, INFINITE);

	// �I�������X���b�h�̃n���h�������
	for(std::vector<Thread*>::const_iterator it = m_vThread.begin();
		it != m_vThread.end();
		++it)
	{
		(*it)->Close();
		delete (*it);
	}

	delete [] handle;

	return true;
}
