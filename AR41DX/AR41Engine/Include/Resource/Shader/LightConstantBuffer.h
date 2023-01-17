#pragma once
#include "ConstantBufferData.h"
class CLightConstantBuffer :
    public CConstantBufferData
{
public:
	CLightConstantBuffer();
	CLightConstantBuffer(const CLightConstantBuffer& Buffer);
	virtual ~CLightConstantBuffer();

private:
	LightCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateBuffer();
	virtual CLightConstantBuffer* Clone();

public:
	void SetLightInfo(const LightCBuffer& Info)
	{
		m_BufferData = Info;
	}

	void SetLightType(ELightType Type)
	{
		m_BufferData.LightType = (int)Type;
	}

	void SetLightColor(const Vector4& Color)
	{
		m_BufferData.Color = Color;
	}

	void SetLightPos(const Vector3& Pos)
	{
		m_BufferData.Pos = Pos;
	}

	void SetLightDir(const Vector3& Dir)
	{
		m_BufferData.Dir = Dir;
	}

	void SetLightDistance(float Distance)
	{
		m_BufferData.Distance = Distance;
	}

	void SetLightAngleIn(float AngleIn)
	{
		m_BufferData.AngleIn = AngleIn;
	}

	void SetLightAngleOut(float AngleOut)
	{
		m_BufferData.AngleOut = AngleOut;
	}

	void SetLightAtt1(float Att1)
	{
		m_BufferData.Att1 = Att1;
	}

	void SetLightAtt2(float Att2)
	{
		m_BufferData.Att2 = Att2;
	}

	void SetLightAtt3(float Att3)
	{
		m_BufferData.Att3 = Att3;
	}

	void SetLightIntensity(float Intensity)
	{
		m_BufferData.Intensity = Intensity;
	}
};

