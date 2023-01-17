
#include "GlobalConstantBuffer.h"

CGlobalConstantBuffer::CGlobalConstantBuffer() :
	m_BufferData{}
{
}

CGlobalConstantBuffer::CGlobalConstantBuffer(const CGlobalConstantBuffer& Buffer) :
	CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CGlobalConstantBuffer::~CGlobalConstantBuffer()
{
}

bool CGlobalConstantBuffer::Init()
{
	SetConstantBuffer("Global");

	return true;
}

void CGlobalConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CGlobalConstantBuffer* CGlobalConstantBuffer::Clone()
{
	return new CGlobalConstantBuffer(*this);
}
