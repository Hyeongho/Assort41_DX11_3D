#pragma once

#include "SceneComponent.h"
#include "../Resource/Shader/LightConstantBuffer.h"

class CLightComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CScene;

protected:
	CLightComponent();
	CLightComponent(const CLightComponent& component);
	virtual ~CLightComponent();

protected:
	CLightConstantBuffer*	m_CBuffer;
	LightCBuffer			m_LightInfo;

public:
	const LightCBuffer& GetLightInfo()	const
	{
		return m_LightInfo;
	}

	ELightType GetLightType()	const
	{
		return (ELightType)m_LightInfo.LightType;
	}

	float GetLightDistance()	const
	{
		return m_LightInfo.Distance;
	}

	float GetLightIntensity()	const
	{
		return m_LightInfo.Intensity;
	}

	void SetLightType(ELightType Type)
	{
		m_LightInfo.LightType = (int)Type;

		m_CBuffer->SetLightType(Type);
	}

	void SetLightColor(const Vector4& Color)
	{
		m_LightInfo.Color = Color;

		m_CBuffer->SetLightColor(Color);
	}

	void SetLightPos(const Vector3& Pos)
	{
		m_LightInfo.Pos = Pos;

		m_CBuffer->SetLightPos(Pos);
	}

	void SetLightDir(const Vector3& Dir)
	{
		m_LightInfo.Dir = Dir;

		m_CBuffer->SetLightDir(Dir);
	}

	void SetLightDistance(float Distance)
	{
		m_LightInfo.Distance = Distance;

		m_CBuffer->SetLightDistance(Distance);
	}

	void SetLightAngleIn(float AngleIn)
	{
		m_LightInfo.AngleIn = AngleIn;

		m_CBuffer->SetLightAngleIn(AngleIn);
	}

	void SetLightAngleOut(float AngleOut)
	{
		m_LightInfo.AngleOut = AngleOut;

		m_CBuffer->SetLightAngleOut(AngleOut);
	}

	void SetLightAtt1(float Att1)
	{
		m_LightInfo.Att1 = Att1;

		m_CBuffer->SetLightAtt1(Att1);
	}

	void SetLightAtt2(float Att2)
	{
		m_LightInfo.Att2 = Att2;

		m_CBuffer->SetLightAtt2(Att2);
	}

	void SetLightAtt3(float Att3)
	{
		m_LightInfo.Att3 = Att3;

		m_CBuffer->SetLightAtt3(Att3);
	}

	void SetLightIntensity(float Intensity)
	{
		m_LightInfo.Intensity = Intensity;

		m_CBuffer->SetLightIntensity(Intensity);
	}

public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CLightComponent* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void SetShader();
};

