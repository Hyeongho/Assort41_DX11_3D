#pragma once
#include "ConstantBufferData.h"
class CUIProgressBarConstantBuffer :
    public CConstantBufferData
{
public:
	CUIProgressBarConstantBuffer();
	CUIProgressBarConstantBuffer(const CUIProgressBarConstantBuffer& Buffer);
	virtual ~CUIProgressBarConstantBuffer();

private:
	UIProgressBarCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CUIProgressBarConstantBuffer* Clone();

public:
	void SetBarDir(EProgressBarDir Dir)
	{
		m_BufferData.BarDir = (int)Dir;
	}

	void SetPercent(float Percent)
	{
		m_BufferData.Percent = Percent;
	}
};

