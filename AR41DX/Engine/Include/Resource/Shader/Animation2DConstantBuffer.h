#pragma once

#include "ConstantBufferData.h"

class CAnimation2DConstantBuffer :
    public CConstantBufferData
{
public:
	CAnimation2DConstantBuffer();
	CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& Buffer);
	virtual ~CAnimation2DConstantBuffer();

private:
	Animation2DCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CAnimation2DConstantBuffer* Clone();

public:
	void SetImageSize(float Width, float Height)
	{
		m_BufferData.Anim2DImageWidth = Width;
		m_BufferData.Anim2DImageHeight = Height;
	}

	void SetImageFrame(const Vector2& Start, const Vector2& End)
	{
		m_BufferData.Anim2DFrameStart = Start;
		m_BufferData.Anim2DFrameEnd = End;
	}

	void SetImageType(EAnimation2DType Type)
	{
		m_BufferData.Anim2DType = (int)Type;
	}

	void SetAnim2DEnable(bool Enable)
	{
		m_BufferData.Anim2DEnable = Enable ? 1 : 0;
	}

	void SetFrame(int Frame)
	{
		m_BufferData.Anim2DFrame = Frame;
	}
};

