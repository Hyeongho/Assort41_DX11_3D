#pragma once
#include "ConstantBufferData.h"
class CColliderConstantBuffer :
    public CConstantBufferData
{
public:
	CColliderConstantBuffer();
	CColliderConstantBuffer(const CColliderConstantBuffer& Buffer);
	virtual ~CColliderConstantBuffer();

private:
	ColliderCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CColliderConstantBuffer* Clone();

public:
	void SetColor(const Vector4& Color)
	{
		m_BufferData.Color = Color;
	}

	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
		m_BufferData.matWVP.Transpose();
	}
};

