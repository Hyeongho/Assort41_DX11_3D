#pragma once
#include "SceneComponent.h"

class CRigidBody : public CSceneComponent
{
protected:
	friend class CGameObject;
	friend class CScene;
	Vector2		m_Force;		//ũ��,����
	Vector2		m_Accel;		//���ӵ�
	Vector2		m_AccelA;		//�߰� ���ӵ�
	Vector2		m_Velocity;		//�ӵ�(ũ��: �ӷ�, ����)
	Vector2		m_MaxVelocity;	//�ִ�ӵ�
	Vector2		m_MinVelocity;	//�ּҼӵ�
	float m_Mass;		//����
	float m_FricCoeff;	//�������
//�߷� �ɼ�
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

