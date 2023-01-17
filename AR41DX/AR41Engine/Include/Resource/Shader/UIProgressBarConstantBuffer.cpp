#include "UIProgressBarConstantBuffer.h"

CUIProgressBarConstantBuffer::CUIProgressBarConstantBuffer() :
	m_BufferData{}
{
}

CUIProgressBarConstantBuffer::CUIProgressBarConstantBuffer(const CUIProgressBarConstantBuffer& Buffer) :
	CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CUIProgressBarConstantBuffer::~CUIProgressBarConstantBuffer()
{
}

bool CUIProgressBarConstantBuffer::Init()
{
	SetConstantBuffer("UIProgressBar");

	return true;
}

void CUIProgressBarConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CUIProgressBarConstantBuffer* CUIProgressBarConstantBuffer::Clone()
{
	return new CUIProgressBarConstantBuffer(*this);
}


