#pragma once
#include "SceneComponent.h"

class CRigidBody : public CSceneComponent
{
protected:
	friend class CGameObject;
	friend class CScene;
	Vector2		m_Force;		//크기,방향
	Vector2		m_Accel;		//가속도
	Vector2		m_AccelA;		//추가 가속도
	Vector2		m_Velocity;		//속도(크기: 속력, 방향)
	Vector2		m_MaxVelocity;	//최대속도
	Vector2		m_MinVelocity;	//최소속도
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
	void AddForce(Vector2 vf)
	{
		m_Force += vf;
	}
	void AddForce(float x, float y)
	{
		m_Force.x = x;
		m_Force.y = y;
	}
	float GetSpeed()
	{
		return m_Velocity.Length();
	}
	void SetAccelAlpha(Vector2 accel)
	{
		m_AccelA = accel;
	}
	void SetAccelAlpha(float x,float y)
	{
		m_AccelA.x = x;
		m_AccelA.y = y;
	}
	Vector2 GetVelocity()
	{
		return m_Velocity;
	}
	void SetVelocity(Vector2 v)
	{
		m_Velocity = v;
	}
	void SetVelocity(float x,float y)
	{
		m_Velocity.x = x;
		m_Velocity.y = y;
	}
	void SetVelocityY(float y)
	{
		m_Velocity.y = y;
	}
	void AddVelocity(Vector2 v)
	{
		m_Velocity += v;
	}
	void MulVelocity(float velocity)
	{
		m_Velocity *= velocity;
	}
	void SetMaxVelocity(Vector2 v)
	{
		m_MaxVelocity = v;
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
	bool GetGravity()
	{
		return m_Gravity;
	}
	void SetGravity(bool b)
	{
		m_Gravity = b;
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

