#include "RigidBody.h"
#include "../GameObject/GameObject.h"

CRigidBody::CRigidBody()
	: m_Mass(1.f)
	, m_FricCoeff(100.f)
	, m_MaxVelocity(Vector3(1500.f, 1500.f, 1500.f))
	, m_MinVelocity(Vector3(-1500.f, -800.f, 1500.f))
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
	float fspeed = m_Velocity.Length();	//이동 속력
	if (0.f != fspeed)
	{
		Vector3 rot = GetOwner()->GetWorldRot();
		if (isnan(rot.z))
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
	if (m_Ground && m_Gravity)
	{
		m_Velocity = Vector3(m_Velocity.x, 0.f, m_Velocity.z);
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
	//중력 옵션
	if (m_Gravity)
	{
		m_AccelA = Vector3(0.f, -300.f, 0.f);
	}
	//
	float force = m_Force.Length();	//힘의크기
	if (0.f != force)
	{
		m_Force.Normalize();				//힘의방향
		float accel = force / m_Mass;	//가속도의크기
		m_Accel = m_Force * accel;		//가속도 
	}
	m_Accel += m_AccelA;				//추가 가속도 
	m_Velocity += m_Accel * deltaTime;			//속도
	if (!m_Velocity.Iszero())
	{
		Vector3 fricDir = -m_Velocity;
		fricDir.Normalize();
		//마찰력의 의한 반대방향으로의 가속도
		Vector3 friction = fricDir * m_FricCoeff * deltaTime;
		//마찰가속도가 속도보다 더크면(length는 절대값)
		if (m_Velocity.Length() <= friction.Length())
		{
			m_Velocity = Vector3(0.f, 0.f, 0.f);
		}
		else
		{
			m_Velocity += friction;
		}
	}
	//속도제한 검사
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
	Move();								//속도에 따른 이동
	m_Force = Vector3(0.f, 0.f, 0.f);	//힘 초기화
	m_Accel = Vector3(0.f, 0.f, 0.f);	//가속도 초기화
	m_AccelA = Vector3(0.f, 0.f, 0.f); 	//추가 가속도 초기화
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
