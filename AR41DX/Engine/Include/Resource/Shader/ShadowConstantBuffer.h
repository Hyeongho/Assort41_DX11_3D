#pragma once

#include "ConstantBufferData.h"

class CShadowConstantBuffer :
    public CConstantBufferData
{
public:
	CShadowConstantBuffer();
	CShadowConstantBuffer(const CShadowConstantBuffer& Buffer);
	virtual ~CShadowConstantBuffer();

private:
	ShadowCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CShadowConstantBuffer* Clone();

public:
	void SetShadowVP(const Matrix& matShadowVP)
	{
		m_BufferData.matShadowVP = matShadowVP;

		m_BufferData.matShadowInvVP = matShadowVP;
		m_BufferData.matShadowInvVP.Inverse();
	}

	void SetShadowResolution(const Vector2& RS)
	{
		m_BufferData.Resolution = RS;
	}

	void SetShadowBias(float Bias)
	{
		m_BufferData.Bias = Bias;
	}
};

