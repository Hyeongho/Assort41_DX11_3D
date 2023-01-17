#pragma once
#include "ConstantBufferData.h"
class CInstancingConstantBuffer :
    public CConstantBufferData
{
public:
	CInstancingConstantBuffer();
	CInstancingConstantBuffer(const CInstancingConstantBuffer& Buffer);
	virtual ~CInstancingConstantBuffer();

private:
	InstancingCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CInstancingConstantBuffer* Clone();

public:
	void SetBoneCount(int Count)
	{
		m_BufferData.BoneCount = Count;
	}
};

