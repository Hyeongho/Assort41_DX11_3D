#include "Pufferfish.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ColliderSphere3D.h"

CPufferfish::CPufferfish()
{
}

CPufferfish::CPufferfish(const CPufferfish& Obj)
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

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Pufferfish");

	m_Mesh->AddChild(m_Sphere);

	m_Sphere->SetRadius(m_Mesh->GetMeshSize().x);

	m_Animation = m_Mesh->SetAnimation<CAnimation>("Pufferfish");
	m_Animation->AddAnimation("Pufferfish", "Pufferfish", 1.f, 1.f, true);

	return true;
}

void CPufferfish::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPufferfish::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
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
}
