#include "ParticleConstantBuffer.h"


CParticleConstantBuffer::CParticleConstantBuffer() : m_BufferData{}
{
}

CParticleConstantBuffer::CParticleConstantBuffer(const CParticleConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CParticleConstantBuffer::~CParticleConstantBuffer()
{
}

bool CParticleConstantBuffer::Init()
{
	SetConstantBuffer("ParticleUpdate");

	return true;
}

void CParticleConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CParticleConstantBuffer* CParticleConstantBuffer::Clone()
{
	return new CParticleConstantBuffer(*this);
}
