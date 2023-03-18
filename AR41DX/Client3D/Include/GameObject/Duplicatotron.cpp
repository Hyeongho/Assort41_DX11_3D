#include "Duplicatotron.h"
#include "Hammer.h"
#include "Dupli_Can.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
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
	, m_CountHammer(0)
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

	m_DetectArea->SetCollisionCallback<CDuplicatotron>(ECollision_Result::Collision, this, &CDuplicatotron::Collision_Detect);
	m_DetectArea->SetCollisionCallback<CDuplicatotron>(ECollision_Result::Release, this, &CDuplicatotron::Release_DetectOff);

	
	m_Animation->SetCurrentEndFunction("Duplicatotron_SpawnEnemies", this, &CDuplicatotron::SpawnHammers);
}

bool CDuplicatotron::Init()
{
	if (!CMonster::Init())
	{
		return false;
	}

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	m_DetectArea = CreateComponent<CColliderOBB3D>("DetectArea");
	m_BodyCube = CreateComponent<CColliderOBB3D>("AttackArea");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("HammerAnimation");

	SetRootComponent(m_Mesh);

	//m_Mesh->SetMesh("Hammer");
	//m_Mesh->AddChild(m_DetectArea);
	//m_Mesh->AddChild(m_BodyCube);

	m_Mesh->SetMesh("Duplicatotron");
	m_Mesh->AddChild(m_DetectArea);
	m_Mesh->AddChild(m_BodyCube);
	m_Mesh->SetWorldPosition(500.f, 0.f, 100.f);

	auto iter = m_Mesh->GetMaterials()->begin();
	auto iterEnd = m_Mesh->GetMaterials()->end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->SetEmissiveColor(1.f, 1.f, 1.f, 0.f);
	}

	m_DetectArea->SetCollisionProfile("DetectArea");
	m_DetectArea->SetBoxHalfSize(800.f, 400.f, 800.f);
	m_DetectArea->SetRelativePosition(0.f, 400.f);

	m_BodyCube->SetCollisionProfile("Monster");
	m_BodyCube->SetBoxHalfSize(90.f, 220.f, 165.f);
	m_BodyCube->SetRelativePosition(0.f, 220.f);


	m_Animation->AddAnimation("Duplicatotron_Idle", "Duplicatotron_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Duplicatotron_SpawnEnemies", "Duplicatotron_SpawnEnemies", 1.f, 1.f, true);
	m_Animation->AddAnimation("Duplicatotron_Destroyed", "Duplicatotron_Destroyed", 1.f, 1.f, false);

	m_Rigid->SetGround(true);

	return true;
}

void CDuplicatotron::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	
}

void CDuplicatotron::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);

	if (m_SpawnOn)
	{
		m_CountHammer = 0;

		m_DelayTime += DeltaTime;
		//
		if (m_DelayTime >= 1.3f && m_CountHammer < 1)
		{
			++m_CountHammer;

			SpawnHammers();

			m_DelayTime = 0.f;
		}

		if (m_DelayTime >= 1.4f && m_CountHammer < 3)
		{
			++m_CountHammer;

			SpawnHammers();

		}

		if (m_CountHammer > 2)
		{
			m_CountHammer = 0;

			m_SpawnOn = false;
		}
	}
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

void CDuplicatotron::Collision_Detect(const CollisionResult& result)
{
	std::string Name = result.Dest->GetCollisionProfile()->Name;

	if (Name == "Player")
	{
		m_SpawnOn = true;

		SpawnAnimation();
	}
}

void CDuplicatotron::Release_DetectOff(const CollisionResult& result)
{
	std::string Name = result.Dest->GetCollisionProfile()->Name;

	if (Name == "Player")
	{
		m_SpawnOn = false;

		m_Animation->ChangeAnimation("Duplicatotron_Idle");
	}
}

void CDuplicatotron::SpawnAnimation()
{
	m_Animation->ChangeAnimation("Duplicatotron_SpawnEnemies");
}

void CDuplicatotron::SpawnHammers()
{
	m_SpawnOn = true;

	Vector3 Pos = GetWorldPos();

	CDupli_Can* DupliCan = m_Scene->CreateObject<CDupli_Can>("DupliCan");
	DupliCan->SetWorldPosition(Pos.x + 10.f, Pos.y + 10.f, Pos.z + 10.f);
	//m_Hammer = m_Scene->CreateObject<CHammer>("Hammer");
}