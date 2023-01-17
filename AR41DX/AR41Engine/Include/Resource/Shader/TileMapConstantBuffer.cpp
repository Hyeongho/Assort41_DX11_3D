#include "TileMapConstantBuffer.h"

CTileMapConstantBuffer::CTileMapConstantBuffer() :
	m_BufferData{}
{
}

CTileMapConstantBuffer::CTileMapConstantBuffer(const CTileMapConstantBuffer& Buffer) :
	CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CTileMapConstantBuffer::~CTileMapConstantBuffer()
{
}

bool CTileMapConstantBuffer::Init()
{
	SetConstantBuffer("TileMap");

	return true;
}

void CTileMapConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CTileMapConstantBuffer* CTileMapConstantBuffer::Clone()
{
	return new CTileMapConstantBuffer(*this);
}

