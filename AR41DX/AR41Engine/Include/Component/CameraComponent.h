#pragma once

#include "SceneComponent.h"

enum EFrustumPlane
{
	Frustum_Left,
	Frustum_Right,
	Frustum_Top,
	Frustum_Bottom,
	Frustum_Near,
	Frustum_Far,
	Frustum_Max
};

class CCameraComponent :
    public CSceneComponent
{
    friend class CCameraManager;
	friend class CGameObject;
	friend class CScene;

protected:
	CCameraComponent();
	CCameraComponent(const CCameraComponent& component);
	virtual ~CCameraComponent();

protected:
	ECameraType	m_CameraType;
	Matrix		m_matView;
	Matrix		m_matProj;
	Matrix		m_matShadowView;
	Matrix		m_matShadowProj;

	float		m_CameraViewDistance;

	Vector3		m_FrustumPos[8];
	Vector4		m_FrustumPlane[Frustum_Max];

public:
	const Matrix& GetShadowViewMatrix()	const
	{
		return m_matShadowView;
	}

	const Matrix& GetShadowProjMatrix()	const
	{
		return m_matShadowProj;
	}

	const Matrix& GetViewMatrix()	const
	{
		return m_matView;
	}

	const Matrix& GetProjMatrix()	const
	{
		return m_matProj;
	}

	//김범중 에디터에서 컴포넌트값 가져오는 용도
	ECameraType GetCameraType()	const
	{
		return m_CameraType;
	}

	float GetCameraViewDistance()	const
	{
		return m_CameraViewDistance;
	}


public:
	void SetCameraType(ECameraType Type)
	{
		m_CameraType = Type;

		ComputeProjectionMatrix();
	}

	void SetCameraViewDistance(float Distance)
	{
		m_CameraViewDistance = Distance;

		ComputeProjectionMatrix();
	}

	void ComputeProjectionMatrix();

public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CCameraComponent* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);


public:
	bool FrustumInPoint(const Vector3& Point);
	bool FrustumInSphere(const Vector3& Center,
		float Radius);
};

