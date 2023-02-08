#include "LightConstantBuffer.h"

CLightConstantBuffer::CLightConstantBuffer()
{
}

CLightConstantBuffer::CLightConstantBuffer(const CLightConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CLightConstantBuffer::~CLightConstantBuffer()
{
}

bool CLightConstantBuffer::Init()
{
	SetConstantBuffer("Light");

	return true;
}

void CLightConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CLightConstantBuffer* CLightConstantBuffer::Clone()
{
	return new CLightConstantBuffer(*this);
}
