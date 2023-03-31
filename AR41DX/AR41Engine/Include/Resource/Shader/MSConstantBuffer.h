#pragma once

#include "ConstantBufferData.h"

class CMSConstantBuffer :
    public CConstantBufferData
{
public:
	CMSConstantBuffer();
	CMSConstantBuffer(const CMSConstantBuffer& Buffer);
	virtual ~CMSConstantBuffer();

private:
	MSCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CMSConstantBuffer* Clone();
};

