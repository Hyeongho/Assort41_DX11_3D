#pragma once

#include "ConstantBufferData.h"

class CMaterialConstantBuffer :
    public CConstantBufferData
{
public:
	CMaterialConstantBuffer();
	CMaterialConstantBuffer(const CMaterialConstantBuffer& Buffer);
	virtual ~CMaterialConstantBuffer();

private:
	MaterialCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CMaterialConstantBuffer* Clone();

public:
//김범중 매터리얼 에디터 값 받아오는 함수
	bool GetIsBump()
	{
		return m_BufferData.AmbientColor.w;
	}
	bool GetIsSpecular()
	{
		return m_BufferData.BaseColor.w;
	}
	bool GetIsEmissive()
	{
		return m_BufferData.EmissiveColor.w;
	}
//
	void SetBaseColor(const Vector4& Color)
	{
		m_BufferData.BaseColor = Color;
	}

	void SetAmbientColor(const Vector4& Color)
	{
		m_BufferData.AmbientColor = Color;
	}

	void SetSpecularColor(const Vector4& Color)
	{
		m_BufferData.SpecularColor = Color;
	}

	void SetEmissiveColor(const Vector4& Color)
	{
		m_BufferData.EmissiveColor = Color;
	}

	void SetOpacity(float Opacity)
	{
		m_BufferData.Opacity = Opacity;
	}

	void SetEnableBump(bool Enable)
	{
		m_BufferData.AmbientColor.w = Enable ? 1.f : 0.f;
	}

	void SetEnableSpecular(bool Enable)
	{
		m_BufferData.BaseColor.w = Enable ? 1.f : 0.f;
	}

	void SetEnableEmissive(bool Enable)
	{
		m_BufferData.EmissiveColor.w = Enable ? 1.f : 0.f;
	}

	void SetAnimation3D(bool Enable)
	{
		m_BufferData.Animation3DEnable = Enable ? 1 : 0;
	}

	void SetImageType(EImageType Type)
	{
		m_BufferData.TextureType = (int)Type;
	}

	void SetTextureWidth(float Width)
	{
		m_BufferData.TextureWidth = Width;
	}

	void SetTextureHeight(float Height)
	{
		m_BufferData.TextureHeight = Height;
	}

	void SetReceiveDecal(bool Receive)
	{
		m_BufferData.ReceiveDecal = Receive ? 1 : 0;
	}
};

