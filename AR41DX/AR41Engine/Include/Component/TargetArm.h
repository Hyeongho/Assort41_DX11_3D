#pragma once

#include "SceneComponent.h"

class CTargetArm :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CScene;

protected:
	CTargetArm();
	CTargetArm(const CTargetArm& component);
	virtual ~CTargetArm();

protected:
	Vector3		m_TargetOffset;
	float		m_TargetDistance;
	AXIS		m_TargetDistanceAxis;
	bool		m_WheelZoomInOutEnable;
	float		m_WheelSensitivity;
	float		m_WheelTickMove;
	float		m_WheelZoomMin;
	float		m_WheelZoomMax;

public:
	void SetWheelTickMove(float Move)
	{
		m_WheelTickMove = Move;
	}

	void SetZoomMin(float Min)
	{
		if (Min > m_WheelZoomMax)
			Min = m_WheelZoomMax - 100.f;

		m_WheelZoomMin = Min < 50.f ? 50.f : Min;
	}

	void SetZoomMax(float Max)
	{
		if (Max < m_WheelZoomMin)
			Max = m_WheelZoomMin + 100.f;

		m_WheelZoomMax = Max > 5000.f ? 5000.f : Max;
	}

	void SetMouseWheelSensitivity(float Sensitivity)
	{
		m_WheelSensitivity = Sensitivity;
	}

	void SetWheelZoomInOutEnable(bool Enable)
	{
		m_WheelZoomInOutEnable = Enable;
	}

	void SetTargetDistanceAxis(AXIS Axis)
	{
		m_TargetDistanceAxis = Axis;
	}

	void SetTargetDistance(float Distance)
	{
		m_TargetDistance = Distance;
	}

	void SetTargetOffset(const Vector3& Offset)
	{
		m_TargetOffset = Offset;
	}

	void SetTargetOffset(float x, float y, float z)
	{
		m_TargetOffset = Vector3(x, y, z);
	}

public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CTargetArm* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

