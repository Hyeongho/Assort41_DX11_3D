#include "RigidBody.h"
#include "../GameObject/GameObject.h"

CRigidBody::CRigidBody()
	: m_Mass(1.f)
	, m_FricCoeff(100.f)
	, m_MaxVelocity(Vector2(1500.f, 1500.f))
	, m_MinVelocity(Vector2(-1500.f, -800.f))
	, m_Gravity(false)
	, m_Ground(false)
{
	SetTypeID<CRigidBody>();
	m_ComponentTypeName = "RigidBody";
}

CRigidBody::CRigidBody(const CRigidBody& component)
	: CSceneComponent(component)
	, m_Mass(component.m_Mass)
	, m_FricCoeff(component.m_FricCoeff)
	, m_MaxVelocity(component.m_MaxVelocity)
	, m_MinVelocity(component.m_MinVelocity)
	, m_Gravity(component.m_Gravity)
	, m_Ground(component.m_Ground)
{
}

CRigidBody::~CRigidBody()
{
}

void CRigidBody::Move()
{
	float fspeed = m_Velocity.Length();	//�̵� �ӷ�
	if (0.f != fspeed)
	{
		Vector3 rot=GetOwner()->GetWorldRot();
		if(isnan(rot.z))
		{
			GetOwner()->SetWorldRotationZ(0.f);
		}
		Vector2 vPos = m_Velocity * g_DeltaTime;
		GetOwner()->AddWorldPosition(vPos);
	}
}

void CRigidBody::SetGround(bool b)
{
	m_Ground = b;
	if (m_Ground&& m_Gravity)
	{
		m_Velocity=Vector2(m_Velocity.x, 0.f);
	}
}

void CRigidBody::Destroy()
{
	CSceneComponent::Destroy();
}

void CRigidBody::Start()
{
	CSceneComponent::Start();
}

bool CRigidBody::Init()
{
	CSceneComponent::Init();
	return true;
}

void CRigidBody::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CRigidBody::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
//�߷� �ɼ�
	if(m_Gravity)
	{
		m_AccelA = Vector2(0.f, -300.f);
	}
//
	float force = m_Force.Length();	//����ũ��
	if (0.f != force)
	{
		m_Force.Normalize();				//���ǹ���
		float accel = force / m_Mass;	//���ӵ���ũ��
		m_Accel = m_Force * accel;		//���ӵ� 
	}
	m_Accel += m_AccelA;				//�߰� ���ӵ� 
	m_Velocity += m_Accel * deltaTime;			//�ӵ�
	if (!m_Velocity.Iszero())
	{
		Vector2 fricDir = -m_Velocity;
		fricDir.Normalize();
		//�������� ���� �ݴ���������� ���ӵ�
		Vector2 friction = fricDir * m_FricCoeff * deltaTime;
		//�������ӵ��� �ӵ����� ��ũ��(length�� ���밪)
		if (m_Velocity.Length() <= friction.Length())
		{
			m_Velocity = Vector2(0.f, 0.f);
		}
		else
		{
			m_Velocity += friction;
		}
	}
	//�ӵ����� �˻�
	if (m_MaxVelocity.x < m_Velocity.x)
	{
		m_Velocity.x = m_MaxVelocity.x;
	}
	if (m_MinVelocity.x > m_Velocity.x)
	{
		m_Velocity.x = m_MinVelocity.x;
	}
	if (m_MaxVelocity.y < m_Velocity.y)
	{
		m_Velocity.y = m_MaxVelocity.y;
	}
	if (m_MinVelocity.y > m_Velocity.y)
	{
		m_Velocity.y = m_MinVelocity.y;
	}
	Move();								//�ӵ��� ���� �̵�
	m_Force = Vector2(0.f, 0.f);			//�� �ʱ�ȭ
	m_Accel = Vector2(0.f, 0.f);				//���ӵ� �ʱ�ȭ
	m_AccelA = Vector2(0.f, 0.f);				//�߰� ���ӵ� �ʱ�ȭ
}

CRigidBody* CRigidBody::Clone() const
{
	return new CRigidBody(*this);
}

void CRigidBody::Save(FILE* file)
{
	CSceneComponent::Save(file);
}

void CRigidBody::Load(FILE* file)
{
	CSceneComponent::Load(file);
}
