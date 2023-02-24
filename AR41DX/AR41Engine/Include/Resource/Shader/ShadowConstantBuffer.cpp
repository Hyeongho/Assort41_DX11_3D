#include "ShadowConstantBuffer.h"

CShadowConstantBuffer::CShadowConstantBuffer() : m_BufferData{}
{
	m_BufferData.Bias = 0.1f;
}

CShadowConstantBuffer::CShadowConstantBuffer(const CShadowConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CShadowConstantBuffer::~CShadowConstantBuffer()
{
}

bool CShadowConstantBuffer::Init()
{
	SetConstantBuffer("Shadow");

	return true;
}

void CShadowConstantBuffer::UpdateBuffer()
{
	m_BufferData.matShadowVP.Transpose();
	m_BufferData.matShadowInvVP.Transpose();

	m_Buffer->UpdateBuffer(&m_BufferData);
}

CShadowConstantBuffer* CShadowConstantBuffer::Clone()
{
	return new CShadowConstantBuffer(*this);
}
