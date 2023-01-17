#pragma once
#include "ConstantBufferData.h"
class CTileMapConstantBuffer :
    public CConstantBufferData
{
public:
	CTileMapConstantBuffer();
	CTileMapConstantBuffer(const CTileMapConstantBuffer& Buffer);
	virtual ~CTileMapConstantBuffer();

private:
	TileMapCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CTileMapConstantBuffer* Clone();

public:
	const Vector2& GetStartFrame()	const
	{
		return m_BufferData.Start;
	}

	const Vector2& GetEndFrame()	const
	{
		return m_BufferData.End;
	}

public:
	void SetImageSize(const Vector2& Size)
	{
		m_BufferData.ImageSize = Size;
	}

	void SetStart(const Vector2& Start)
	{
		m_BufferData.Start = Start;
	}

	void SetEnd(const Vector2& End)
	{
		m_BufferData.End = End;
	}

	void SetTileSize(const Vector2& Size)
	{
		m_BufferData.TileSize = Size;
	}

	void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
		m_BufferData.matWVP.Transpose();
	}

	void SetFrame(int Frame)
	{
		m_BufferData.Frame = Frame;
	}
};

