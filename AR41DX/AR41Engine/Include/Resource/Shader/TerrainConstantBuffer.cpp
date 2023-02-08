#include "TerrainConstantBuffer.h"

CTerrainConstantBuffer::CTerrainConstantBuffer() : m_BufferData{}
{
}

CTerrainConstantBuffer::CTerrainConstantBuffer(const CTerrainConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CTerrainConstantBuffer::~CTerrainConstantBuffer()
{
}

bool CTerrainConstantBuffer::Init()
{
	SetConstantBuffer("Terrain");

	m_BufferData.DetailLevel = 80.f;
	m_BufferData.SplatCount = 1;

	return true;
}

void CTerrainConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CTerrainConstantBuffer* CTerrainConstantBuffer::Clone()
{
	return new CTerrainConstantBuffer(*this);
}
