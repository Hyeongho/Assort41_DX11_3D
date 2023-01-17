#include "ColliderConstantBuffer.h"

CColliderConstantBuffer::CColliderConstantBuffer() :
	m_BufferData{}
{
}

CColliderConstantBuffer::CColliderConstantBuffer(const CColliderConstantBuffer& Buffer) :
	CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CColliderConstantBuffer::~CColliderConstantBuffer()
{
}

bool CColliderConstantBuffer::Init()
{
	SetConstantBuffer("Collider");

	return true;
}

void CColliderConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CColliderConstantBuffer* CColliderConstantBuffer::Clone()
{
	return new CColliderConstantBuffer(*this);
}
