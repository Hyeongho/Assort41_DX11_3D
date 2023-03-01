#include "FXAAConstantBuffer.h"

CFXAAConstantBuffer::CFXAAConstantBuffer()
{
}

CFXAAConstantBuffer::CFXAAConstantBuffer(const CFXAAConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
}

CFXAAConstantBuffer::~CFXAAConstantBuffer()
{
}

bool CFXAAConstantBuffer::Init()
{
	SetConstantBuffer("FXAA");

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
