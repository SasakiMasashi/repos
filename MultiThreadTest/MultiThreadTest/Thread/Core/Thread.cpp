#include "Thread.h"
#include "IThreadFunctor.h"
#include <process.h>

Thread::Thread(IThreadFunctor* funcObj)
	:m_Func(funcObj),
		m_Handle(0),
		m_ThreadId(0)
{
	// �X���b�h�Z�L�����e�B�̏�����
	m_SA.nLength = sizeof(SECURITY_ATTRIBUTES);
	m_SA.lpSecurityDescriptor = 0;
	m_SA.bInheritHandle = TRUE;
}

Thread::~Thread()
{
	Close();
	delete m_Func;
}

// �X���b�h�̃��C������
unsigned Thread::threadMain(void *arg)
{
	IThreadFunctor* func = (IThreadFunctor*)arg;
	if(!func)
	{
		return 1;
	}

	func->Run();

	return 0;
}

// �X���b�h�̊J�n
int Thread::Start()
{		
	m_Handle = (HANDLE)_beginthreadex(&m_SA,
									4096,
									Thread::threadMain,
									m_Func,
									0,
									&m_ThreadId);

	return 0;
}

// �X���b�h�̏I����҂�
void Thread::Wait()
{		
	WaitForSingleObject(m_Handle, INFINITE);
	Close();
}

// �X���b�h�����I��
void Thread::End()
{
	if(m_Handle != 0)
	{
		_endthreadex(0);
		Close();
	}
}

// �X���b�h�Z�L�����e�B���Z�b�g
void Thread::SetSecurityAttributes(const SECURITY_ATTRIBUTES& SA)
{
	m_SA = SA;
}

// ���ɏI�����Ă���΁A�n���h�������
void Thread::Close()
{
	if(0 == m_Handle)
	{
		return;
	}

	DWORD exitCode = 0;
	if(0 == GetExitCodeThread(m_Handle, &exitCode))
	{// �G���[
		return;
	}

	if(exitCode == STILL_ACTIVE)
	{
		return;
	}

	CloseHandle(m_Handle);
	m_Handle = 0;
}

bool Thread::IsAlive() const
{
	if(0 == m_Handle)
	{
		return false;
	}

	DWORD exitCode = 0;
	if(0 == GetExitCodeThread(m_Handle, &exitCode))
	{// �G���[
		return false;
	}

	return (exitCode == STILL_ACTIVE);
}
