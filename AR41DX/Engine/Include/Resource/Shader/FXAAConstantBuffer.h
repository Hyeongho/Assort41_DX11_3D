
#pragma once
#include "ConstantBufferData.h"

class CFXAAConstantBuffer :
	public CConstantBufferData
{
public:
	CFXAAConstantBuffer();
	CFXAAConstantBuffer(const CFXAAConstantBuffer& Buffer);
	virtual ~CFXAAConstantBuffer();

private:
	FXAACBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CFXAAConstantBuffer* Clone();
};

