#include "Pufferfish.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Scene/NavigationManager3D.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ColliderSphere3D.h"
#include "Component/RigidBody.h"

CPufferfish::CPufferfish()
{
}

CPufferfish::CPufferfish(const CPufferfish& Obj) : CGameObject(Obj)
{
}

CPufferfish::~CPufferfish()
{
}

void CPufferfish::Start()
{
	CGameObject::Start();

	m_Sphere->SetCollisionCallback<CPufferfish>(ECollision_Result::Collision, this, &CPufferfish::CollisionTest);
}

bool CPufferfish::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Sphere = CreateComponent<CColliderSphere3D>("Mesh");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Pufferfish");

	m_Mesh->AddChild(m_Sphere);
	m_Mesh->AddChild(m_Rigid);

	m_Sphere->SetRadius(m_Mesh->GetMeshSize().x / 2.f);

	m_Animation = m_Mesh->SetAnimation<CAnimation>("Pufferfish");
	m_Animation->AddAnimation("Pufferfish", "Pufferfish", 1.f, 1.f, true);

	SetPivot(0.5f, 1.f, 0.5f);

	m_Rigid->SetGravity(true);

	return true;
}

void CPufferfish::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();

	if (m_Rigid->GetGround())
	{
		m_Rigid->SetGround(false);
		m_Rigid->AddForce(0, 1000.f, 0.f);
		m_Rigid->SetVelocityY(1000.f);

		/*float Y = Nav->GetHeight(GetWorldPos());
		if (Y != FLT_MAX)
		{
			SetWorldPositionY(Y);
		}*/
	}

	if (m_Rigid->GetVelocity().y < 0.f)
	{
		float Y = Nav->GetHeight(GetWorldPos());

		if (Y != FLT_MAX && GetWorldPos().y - Y < m_Mesh->GetMeshSize().y / 2.f)
		{
			SetWorldPositionY(Y + m_Mesh->GetMeshSize().y / 2.f);
			m_Rigid->SetGround(true);
		}
	}
}

void CPufferfish::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	AddWorldPositionX(300.f * DeltaTime);
}

CPufferfish* CPufferfish::Clone() const
{
	return new CPufferfish(*this);
}

void CPufferfish::Save(FILE* File)
{
}

void CPufferfish::Load(FILE* File)
{
}

void CPufferfish::CollisionTest(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		return;
	}

	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Ground)
	{
		return;
	}
}
