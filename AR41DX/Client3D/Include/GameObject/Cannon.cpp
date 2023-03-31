#include "Cannon.h"
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
#include "Pufferfish.h"

CCannon::CCannon() : m_Time(0.f)
{
	SetTypeID<CCannon>();

	m_ObjectTypeName = "Cannon";
}

CCannon::CCannon(const CCannon& Obj) : CGameObject(Obj)
{
	m_Mesh = dynamic_cast<CAnimationMeshComponent*>(FindComponent("Mesh"));
}

CCannon::~CCannon()
{
}

void CCannon::Start()
{
	CGameObject::Start();
}

bool CCannon::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Cannon");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("Cannon");
	m_Animation->AddAnimation("Cannon", "Cannon_Shoot", 1.f, 1.f, true);

	m_Animation->AddCurrentNotify<CCannon>("Cannon", "CannonFire", 0.1f, this, &CCannon::Fire);
	m_Animation->SetCurrentEndFunction<CCannon>("Cannon", this, &CCannon::Reload);

	return true;
}

void CCannon::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_Time += DeltaTime;

	if (m_Time >= 5.f)
	{
		m_Time = 0.f;

		m_Animation->Play();
	}
}

void CCannon::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CCannon* CCannon::Clone() const
{
	return new CCannon(*this);
}

void CCannon::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CCannon::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CCannon::Fire()
{
	CPufferfish* Pufferfish = m_Scene->CreateObject<CPufferfish>("Cannon");
	Pufferfish->SetWorldPosition(GetWorldPos());

	Pufferfish->AddWorldRotation(GetWorldRot());
}

void CCannon::Reload()
{
	m_Animation->Stop();
	//m_Animation->SetPlayTime("Cannon", 0.f);
}
