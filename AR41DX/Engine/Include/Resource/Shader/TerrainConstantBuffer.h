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
	//����� �����Ϳ�
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

