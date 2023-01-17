#include "ThreadQueue.h"

CThreadQueue::CThreadQueue()	:
	m_Data{},
	m_Head(0),
	m_Tail(0),
	m_Size(0)
{
	InitializeCriticalSection(&m_Crt);
}

CThreadQueue::~CThreadQueue()
{
	DeleteCriticalSection(&m_Crt);
}
