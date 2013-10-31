#include "Thread.h"
#include "IThreadFunctor.h"
#include <process.h>

Thread::Thread(IThreadFunctor* funcObj)
	:m_Func(funcObj),
		m_Handle(0),
		m_ThreadId(0)
{
	// スレッドセキュリティの初期化
	m_SA.nLength = sizeof(SECURITY_ATTRIBUTES);
	m_SA.lpSecurityDescriptor = 0;
	m_SA.bInheritHandle = TRUE;
}

Thread::~Thread()
{
	Close();
	delete m_Func;
}

// スレッドのメイン処理
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

// スレッドの開始
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

// スレッドの終了を待つ
void Thread::Wait()
{		
	WaitForSingleObject(m_Handle, INFINITE);
	Close();
}

// スレッド強制終了
void Thread::End()
{
	if(m_Handle != 0)
	{
		_endthreadex(0);
		Close();
	}
}

// スレッドセキュリティをセット
void Thread::SetSecurityAttributes(const SECURITY_ATTRIBUTES& SA)
{
	m_SA = SA;
}

// 既に終了していれば、ハンドルを閉じる
void Thread::Close()
{
	if(0 == m_Handle)
	{
		return;
	}

	DWORD exitCode = 0;
	if(0 == GetExitCodeThread(m_Handle, &exitCode))
	{// エラー
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
	{// エラー
		return false;
	}

	return (exitCode == STILL_ACTIVE);
}
