#include "DropConstantBuffer.h"

CDropConstantBuffer::CDropConstantBuffer()
{
}

CDropConstantBuffer::CDropConstantBuffer(const CDropConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CDropConstantBuffer::~CDropConstantBuffer()
{
}

bool CDropConstantBuffer::Init()
{
	SetConstantBuffer("Drop");

	return true;
}

void CDropConstantBuffer::UpdateBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CDropConstantBuffer* CDropConstantBuffer::Clone()
{
	return new CDropConstantBuffer(*this);
}
