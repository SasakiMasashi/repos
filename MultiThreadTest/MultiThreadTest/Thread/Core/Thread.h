#pragma once
#include <Windows.h>

class IThreadFunctor;

class Thread 
{
public:
	Thread(IThreadFunctor* funcObj);

	virtual ~Thread();

	// スレッドのメイン処理
	static unsigned __stdcall threadMain(void *arg);

	// スレッドの開始
	int Start();

	// スレッドの終了を待つ
	void Wait();

	// スレッド強制終了
	void End();

	// スレッドセキュリティをセット
	void SetSecurityAttributes(const SECURITY_ATTRIBUTES& SA);

	// 既に終了していれば、ハンドルを閉じる
	void Close();

	bool IsAlive() const;

	// ハンドルを取得
	HANDLE GetHandle() const { return m_Handle; }

private:
	SECURITY_ATTRIBUTES	m_SA;
	IThreadFunctor*		m_Func;
	HANDLE				m_Handle;
	unsigned int			m_ThreadId;
};


