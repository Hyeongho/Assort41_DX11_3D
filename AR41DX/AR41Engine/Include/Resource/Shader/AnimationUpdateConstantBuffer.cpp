
#include "AnimationUpdateConstantBuffer.h"

CAnimationUpdateConstantBuffer::CAnimationUpdateConstantBuffer() :
	m_BufferData{}
{
}

CAnimationUpdateConstantBuffer::CAnimationUpdateConstantBuffer(const CAnimationUpdateConstantBuffer& Buffer) :
	CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CAnimationUpdateConstantBuffer::~CAnimationUpdateConstantBuffer()
{
}

bool CAnimationUpdateConstantBuffer::Init()
{
	SetConstantBuffer("Animation");

	return true;
}

void CAnimationUpdateConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CAnimationUpdateConstantBuffer* CAnimationUpdateConstantBuffer::Clone()
{
	return new CAnimationUpdateConstantBuffer(*this);
}
