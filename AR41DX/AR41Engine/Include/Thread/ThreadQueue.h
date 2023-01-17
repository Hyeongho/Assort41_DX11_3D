#pragma once

#include "../EngineInfo.h"
#include "Sync.h"

class CThreadQueue
{
public:
	CThreadQueue();
	~CThreadQueue();

private:
	ThreadSyncData	m_Data[201];
	int		m_Head;
	int		m_Tail;
	int		m_Size;
	CRITICAL_SECTION	m_Crt;

public:
	void push(int Header, int Size, unsigned char* Data)
	{
		if (full())
			return;

		CSync	sync(&m_Crt);

		m_Tail = (m_Tail + 1) % 201;

		m_Data[m_Tail].Header = Header;
		m_Data[m_Tail].Size = Size;
		memcpy(m_Data[m_Tail].Data, Data, Size);

		++m_Size;
	}

	void pop(int* Header, int* Size, unsigned char* Data)
	{
		if (empty())
			return;

		CSync	sync(&m_Crt);

		m_Head = (m_Head + 1) % 201;

		*Header = m_Data[m_Head].Header;
		*Size = m_Data[m_Head].Size;
		memcpy(Data, m_Data[m_Head].Data, m_Data[m_Head].Size);

		--m_Size;
	}

	int size()
	{
		CSync	sync(&m_Crt);

		return m_Size;
	}

	bool full()
	{
		CSync	sync(&m_Crt);

		return m_Size == 200;
	}

	bool empty()
	{
		CSync	sync(&m_Crt);

		return m_Size == 0;
	}
};

