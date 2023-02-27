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
	//김범중 에디터용
	float GetDetailLevel() const
	{
		return m_BufferData.DetailLevel;
	}
	int GetSplatCount() const
	{
		return m_BufferData.SplatCount;
	}
	//
	void SetDetailLevel(float Level)
	{
		m_BufferData.DetailLevel = Level;
	}

	void SetSplatCount(int Count)
	{
		m_BufferData.SplatCount = Count;
	}
};

