#pragma once

#include "ConstantBufferData.h"

class CTranslationConstantBuffer :
	public CConstantBufferData
{
public:
	CTranslationConstantBuffer();
	CTranslationConstantBuffer(const CTranslationConstantBuffer& Buffer);
	virtual ~CTranslationConstantBuffer();

private:
	TranslationCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTranslationConstantBuffer* Clone();

public:
	void SetTextureTranslation(float TextureTranslation)
	{
		m_BufferData.TextureTranslation = TextureTranslation;
	}
};

