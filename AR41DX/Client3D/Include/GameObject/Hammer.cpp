#include "Hammer.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Engine.h"

CHammer::CHammer()
{
	SetTypeID<CHammer>();

	m_ObjectTypeName = "Hammer";
}

CHammer::CHammer(const CHammer& Obj)
{
}

CHammer::~CHammer()
{
}

void CHammer::Start()
{
	CMonster::Start();
}

bool CHammer::Init()
{
	if (!CMonster::Init())
	{
		return false;
	}

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Hammer");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("HammerAnimation");

	m_Animation->AddAnimation("Hammer_Attack", "Hammer_Attack", 1.f, 1.f, true);
	m_Animation->AddAnimation("Hammer_Notice", "Hammer_Notice", 1.f, 1.f, true);
	m_Animation->AddAnimation("Hammer_Walk", "Hammer_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("Hammer_Dead", "Hammer_Dead", 1.f, 1.f, true);
	m_Animation->AddAnimation("Hammer_Stunned", "Hammer_Stunned", 1.f, 1.f, true);

	return true;
}

void CHammer::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CHammer::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CHammer* CHammer::Clone() const
{
	return new CHammer(*this);
}

void CHammer::Save(FILE* File)
{
	CMonster::Save(File);
}

void CHammer::Load(FILE* File)
{
	CMonster::Load(File);
}
