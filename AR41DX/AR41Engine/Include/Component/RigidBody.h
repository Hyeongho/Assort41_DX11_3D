#pragma once
#include "SceneComponent.h"

class CRigidBody : public CSceneComponent
{
protected:
	friend class CGameObject;
	friend class CScene;
	Vector3		m_Force;		//크기,방향
	Vector3		m_Accel;		//가속도
	Vector3		m_AccelA;		//추가 가속도
	Vector3		m_Velocity;		//속도(크기: 속력, 방향)
	Vector3		m_MaxVelocity;	//최대속도
	Vector3		m_MinVelocity;	//최소속도
	float m_Mass;		//질량
	float m_FricCoeff;	//마찰계수
	//중력 옵션
	bool m_Gravity;
	bool m_Ground;
	CRigidBody();
	CRigidBody(const CRigidBody& component);
	virtual ~CRigidBody();
	//F=M*A
	//V+=A*DT
	void Move();
public:
	Vector3 GetForce()
	{
		return m_Force;
	}
	void SetForce(Vector3 v)
	{
		m_Force = v;
	}
	void SetForce(float x, float y, float z)
	{
		m_Force.x = x;
		m_Force.y = y;
		m_Force.z = z;
	}
	void AddForce(Vector3 vf)
	{
		m_Force += vf;
	}
	void AddForce(float x, float y)
	{
		m_Force.x = x;
		m_Force.y = y;
	}
	void AddForce(float x, float y, float z)
	{
		m_Force.x = x;
		m_Force.y = y;
		m_Force.z = z;
	}
	void MulForce(float f)
	{
		m_Force.x *= f;
		m_Force.y *= f;
		m_Force.z *= f;
	}
	Vector3 GetAccel()
	{
		return m_Accel;
	}
	void SetAccel(Vector3 v)
	{
		m_Accel = v;
	}
	void SetAccel(float x, float y, float z)
	{
		m_Accel.x = x;
		m_Accel.y = y;
		m_Accel.z = z;
	}
	Vector3 GetAccelAlpha()
	{
		return m_AccelA;
	}
	void SetAccelAlpha(Vector3 accel)
	{
		m_AccelA = accel;
	}
	void SetAccelAlpha(float x, float y, float z)
	{
		m_AccelA.x = x;
		m_AccelA.y = y;
		m_AccelA.z = z;
	}
	float GetSpeed()
	{
		return m_Velocity.Length();
	}
	Vector3 GetVelocity()
	{
		return m_Velocity;
	}
	void SetVelocity(Vector3 v)
	{
		m_Velocity = v;
	}
	void SetVelocity(float x, float y, float z)
	{
		m_Velocity.x = x;
		m_Velocity.y = y;
		m_Velocity.z = z;
	}
	void SetVelocity(float x, float y)
	{
		m_Velocity.x = x;
		m_Velocity.y = y;
	}
	void SetVelocityY(float y)
	{
		m_Velocity.y = y;
	}
	void AddVelocity(Vector3 v)
	{
		m_Velocity += v;
	}
	void MulVelocity(float velocity)
	{
		m_Velocity *= velocity;
	}
	Vector3 GetMaxVelocity()
	{
		return m_MaxVelocity;
	}
	void SetMaxVelocity(Vector3 v)
	{
		m_MaxVelocity = v;
	}
	void SetMaxVelocity(float x, float y, float z)
	{
		m_MaxVelocity.x = x;
		m_MaxVelocity.y = y;
		m_MaxVelocity.z = z;
	}
	Vector3 GetMinVelocity()
	{
		return m_MinVelocity;
	}
	void SetMinVelocity(Vector3 v)
	{
		m_MinVelocity = v;
	}
	void SetMinVelocity(float x, float y, float z)
	{
		m_MinVelocity.x = x;
		m_MinVelocity.y = y;
		m_MinVelocity.z = z;
	}
	float GetMass()
	{
		return m_Mass;
	}
	void SetMass(float mass)
	{
		m_Mass = mass;
	}
	void MulMass(float mass)
	{
		m_Mass *= mass;
	}
	float GetFricCoeff()
	{
		return m_FricCoeff;
	}
	void SetFricCoeff(float f)
	{
		m_FricCoeff = f;
	}
	bool GetGravity()
	{
		return m_Gravity;
	}
	void SetGravity(bool b)
	{
		m_Gravity = b;
	}
	bool GetGround()
	{
		return m_Ground;
	}
	void SetGround(bool b);
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CRigidBody* Clone()    const;
	virtual void Save(FILE* file);
	virtual void Load(FILE* file);
};

