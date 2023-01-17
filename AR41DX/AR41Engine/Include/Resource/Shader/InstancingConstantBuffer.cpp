
#include "InstancingConstantBuffer.h"

CInstancingConstantBuffer::CInstancingConstantBuffer() :
	m_BufferData{}
{
}

CInstancingConstantBuffer::CInstancingConstantBuffer(const CInstancingConstantBuffer& Buffer) :
	CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CInstancingConstantBuffer::~CInstancingConstantBuffer()
{
}

bool CInstancingConstantBuffer::Init()
{
	SetConstantBuffer("Instancing");

	return true;
}

void CInstancingConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CInstancingConstantBuffer* CInstancingConstantBuffer::Clone()
{
	return new CInstancingConstantBuffer(*this);
}
