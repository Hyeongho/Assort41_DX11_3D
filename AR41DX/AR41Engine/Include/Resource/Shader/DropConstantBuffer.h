#pragma once

#include "ConstantBufferData.h"

class CDropConstantBuffer :
    public CConstantBufferData
{
public:
	CDropConstantBuffer();
	CDropConstantBuffer(const CDropConstantBuffer& Buffer);
	virtual ~CDropConstantBuffer();

private:
	DropCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CDropConstantBuffer* Clone();

public:
	void SetDropTexture(float Drop)
	{
		m_BufferData.Drop = Drop;
	}
};


