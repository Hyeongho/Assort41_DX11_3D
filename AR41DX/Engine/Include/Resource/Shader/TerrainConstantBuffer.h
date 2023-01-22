#pragma once

#include "ConstantBufferData.h"

class CTerrainConstantBuffer :
    public CConstantBufferData
{
public:
	CTerrainConstantBuffer();
	CTerrainConstantBuffer(const CTerrainConstantBuffer& Buffer);
	virtual ~CTerrainConstantBuffer();

private:
	TerrainCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTerrainConstantBuffer* Clone();

public:
	void SetDetailLevel(float Level)
	{
		m_BufferData.DetailLevel = Level;
	}

	void SetSplatCount(int Count)
	{
		m_BufferData.SplatCount = Count;
	}
};

