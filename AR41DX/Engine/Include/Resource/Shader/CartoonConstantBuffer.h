#pragma once
#include "ConstantBufferData.h"

class CCartoonConstantBuffer	:
	public CConstantBufferData
{
public:
	CCartoonConstantBuffer();
	CCartoonConstantBuffer(const CCartoonConstantBuffer& Buffer);
	virtual ~CCartoonConstantBuffer();

private:
	CartoonCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CCartoonConstantBuffer* Clone();
};
