#pragma once
#include "ConstantBufferData.h"
class CAnimationUpdateConstantBuffer :
    public CConstantBufferData
{
public:
	CAnimationUpdateConstantBuffer();
	CAnimationUpdateConstantBuffer(const CAnimationUpdateConstantBuffer& Buffer);
	virtual ~CAnimationUpdateConstantBuffer();

private:
	AnimationCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CAnimationUpdateConstantBuffer* Clone();

public:
	void SetBoneCount(int BoneCount)
	{
		m_BufferData.BoneCount = BoneCount;
	}

	void SetCurrentFrame(int CurrentFrame)
	{
		m_BufferData.CurrentFrame = CurrentFrame;
	}

	void SetNextFrame(int NextFrame)
	{
		m_BufferData.NextFrame = NextFrame;
	}

	void SetRatio(float Ratio)
	{
		m_BufferData.Ratio = Ratio;
	}

	void SetFrameCount(int FrameCount)
	{
		m_BufferData.FrameCount = FrameCount;
	}

	void SetRowIndex(int RowIndex)
	{
		m_BufferData.RowIndex = RowIndex;
	}

	void SetChangeAnimation(bool Change)
	{
		m_BufferData.ChangeAnimation = Change ? 1 : 0;
	}

	void SetChangeRatio(float ChangeRatio)
	{
		m_BufferData.ChangeRatio = ChangeRatio;
	}

	void SetChangeFrameCount(int ChangeFrameCount)
	{
		m_BufferData.ChangeFrameCount = ChangeFrameCount;
	}
};

