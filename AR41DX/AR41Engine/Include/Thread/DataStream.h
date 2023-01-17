#pragma once

#include "../EngineInfo.h"

class CDataStream
{
public:
	CDataStream();
	~CDataStream();

private:
	unsigned char* m_Buffer;
	int			m_Size;

public:
	void SetBuffer(unsigned char* Buffer)
	{
		m_Buffer = Buffer;
	}

public:
	int GetSize()	const
	{
		return m_Size;
	}

public:
	template <typename T>
	void AddData(const T* Data, int Size)
	{
		memcpy(m_Buffer + m_Size, Data, Size);
		m_Size += Size;
	}

	template <typename T>
	void GetData(T* Data, int Size)
	{
		memcpy(Data, m_Buffer + m_Size, Size);
		m_Size += Size;
	}
};

