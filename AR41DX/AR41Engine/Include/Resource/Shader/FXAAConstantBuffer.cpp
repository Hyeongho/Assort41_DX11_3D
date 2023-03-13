#include "FXAAConstantBuffer.h"

CFXAAConstantBuffer::CFXAAConstantBuffer() : m_BufferData{}
{
}

CFXAAConstantBuffer::CFXAAConstantBuffer(const CFXAAConstantBuffer& Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CFXAAConstantBuffer::~CFXAAConstantBuffer()
{
}

bool CFXAAConstantBuffer::Init()
{
	SetConstantBuffer("FXAA");

	m_BufferData.rcpFrame =
		Vector4(1.0f / 1280.f, 1.0f / 720.f, 0.0f, 0.0f);

	return true;
}

void CFXAAConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CFXAAConstantBuffer* CFXAAConstantBuffer::Clone()
{
	return new CFXAAConstantBuffer(*this);
}
