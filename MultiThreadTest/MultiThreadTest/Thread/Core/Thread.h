#pragma once
#include <Windows.h>

class IThreadFunctor;

class Thread 
{
public:
	Thread(IThreadFunctor* funcObj);

	virtual ~Thread();

	// �X���b�h�̃��C������
	static unsigned __stdcall threadMain(void *arg);

	// �X���b�h�̊J�n
	int Start();

	// �X���b�h�̏I����҂�
	void Wait();

	// �X���b�h�����I��
	void End();

	// �X���b�h�Z�L�����e�B���Z�b�g
	void SetSecurityAttributes(const SECURITY_ATTRIBUTES& SA);

	// ���ɏI�����Ă���΁A�n���h�������
	void Close();

	bool IsAlive() const;

	// �n���h�����擾
	HANDLE GetHandle() const { return m_Handle; }

private:
	SECURITY_ATTRIBUTES	m_SA;
	IThreadFunctor*		m_Func;
	HANDLE				m_Handle;
	unsigned int			m_ThreadId;
};


