#include "Duplicatotron.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Engine.h"

CDuplicatotron::CDuplicatotron() :
	m_SpawnOn(false)
	, m_DelayTime(0.f)
{
	SetTypeID<CDuplicatotron>();

	m_ObjectTypeName = "Duplicatotron";
}

CDuplicatotron::CDuplicatotron(const CDuplicatotron& Obj)
{
}

CDuplicatotron::~CDuplicatotron()
{
}

void CDuplicatotron::Start()
{
	CMonster::Start();

	m_Animation->SetCurrentEndFunction("Duplicatotron_SpawnEnemies", this, &CDuplicatotron::SpawnHammers);
}

bool CDuplicatotron::Init()
{
	if (!CMonster::Init())
	{
		return false;
	}

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Duplicatotron");
	m_Mesh->SetWorldPosition(500.f, 0.f, 100.f);
	m_Animation = m_Mesh->SetAnimation<CAnimation>("DuplicatotronAnimation");

	m_Animation->AddAnimation("Duplicatotron_Idle", "Duplicatotron_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Duplicatotron_SpawnEnemies", "Duplicatotron_SpawnEnemies", 1.f, 1.f, false);
	m_Animation->AddAnimation("Duplicatotron_Destroyed", "Duplicatotron_Destroyed", 1.f, 1.f, false);

	auto iter = m_Mesh->GetMaterials()->begin();
	auto iterEnd = m_Mesh->GetMaterials()->end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->SetEmissiveColor(1.f, 1.f, 1.f, 0.f);
	}

	m_Rigid->SetGround(true);

	return true;
}

void CDuplicatotron::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	if (m_SpawnOn)
	{
		int SpawnHammerCount = 0;

		m_DelayTime += DeltaTime;
		//
		if (m_DelayTime >= 1.3f && SpawnHammerCount < 1)
		{
			++SpawnHammerCount;

			SpawnHammers();

			m_DelayTime = 0.f;
		}

		if (m_DelayTime >= 1.4f && SpawnHammerCount < 3)
		{
			++SpawnHammerCount;

			SpawnHammers();

		}

		if (SpawnHammerCount > 2)
		{
			SpawnHammerCount = 0;

			m_SpawnOn = false;
		}
	}
}

void CDuplicatotron::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CDuplicatotron* CDuplicatotron::Clone() const
{
	return new CDuplicatotron(*this);
}

void CDuplicatotron::Save(FILE* File)
{
	CMonster::Save(File);
}

void CDuplicatotron::Load(FILE* File)
{
	CMonster::Load(File);
}

void CDuplicatotron::SpawnAnimation()
{
	m_Animation->ChangeAnimation("Duplicatotron_SpawnEnemies");
}

void CDuplicatotron::SpawnHammers()
{
	m_SpawnOn = true;

	m_Hammer = m_Scene->CreateObject<CHammer>("Hammer");
}