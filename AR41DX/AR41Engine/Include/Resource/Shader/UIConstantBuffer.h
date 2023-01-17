#pragma once
#include "ConstantBufferData.h"
class CUIConstantBuffer :
    public CConstantBufferData
{
public:
	CUIConstantBuffer();
	CUIConstantBuffer(const CUIConstantBuffer& Buffer);
	virtual ~CUIConstantBuffer();

private:
	UICBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CUIConstantBuffer* Clone();

public:
	void SetTint(const Vector4& Color)
	{
		m_BufferData.UITint = Color;
	}

	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.UIWVP = matWVP;
		m_BufferData.UIWVP.Transpose();
	}

	void SetMeshSize(const Vector2& Size)
	{
		m_BufferData.UIMeshSize = Size;
	}

	void SetPivot(const Vector2& Pivot)
	{
		m_BufferData.UIPivot = Pivot;
	}

	void SetTextureEnable(bool Enable)
	{
		m_BufferData.UITextureEnable = Enable ? 1 : 0;
	}

	void SetOpacity(float Opacity)
	{
		m_BufferData.UIOpacity = Opacity;
	}
};

