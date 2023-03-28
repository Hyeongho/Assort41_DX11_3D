#include "TranslationConstantBuffer.h"

CTranslationConstantBuffer::CTranslationConstantBuffer() : m_BufferData{}
{
}

CTranslationConstantBuffer::CTranslationConstantBuffer(const CTranslationConstantBuffer& Buffer) : CConstantBufferData(Buffer)
{
    m_BufferData = Buffer.m_BufferData;
}

CTranslationConstantBuffer::~CTranslationConstantBuffer()
{
}

bool CTranslationConstantBuffer::Init()
{
    SetConstantBuffer("Translation");

    return true;
}

void CTranslationConstantBuffer::UpdateBuffer()
{
    m_Buffer->UpdateBuffer(&m_BufferData);
}

CTranslationConstantBuffer* CTranslationConstantBuffer::Clone()
{
    return new CTranslationConstantBuffer(*this);
}
