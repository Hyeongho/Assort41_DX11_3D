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
	, m_CountCan(0)
	, m_DelayTime(0.f)
{
	SetTypeID<CDuplicatotron>();

	m_ObjectTypeName = "Duplicatotron";
}

CDuplicatotron::CDuplicatotron(const CDuplicatotron& Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_DetectArea = (CColliderOBB3D*)FindComponent("DetectArea");
	m_BodyCube = (CColliderOBB3D*)FindComponent("Body");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CDuplicatotron::~CDuplicatotron()
{
}

void CDuplicatotron::Start()
{
	CMonster::Start();

	m_DetectArea->SetCollisionCallback<CDuplicatotron>(ECollision_Result::Collision, this, &CDuplicatotron::Collision_Detect);
	m_DetectArea->SetCollisionCallback<CDuplicatotron>(ECollision_Result::Release, this, &CDuplicatotron::Release_DetectOff);
	m_BodyCube->SetCollisionCallback<CDuplicatotron>(ECollision_Result::Collision, this, &CDuplicatotron::Collision_Dead);

	//m_Animation->SetCurrentEndFunction("Duplicatotron_SpawnEnemies", this, &CDuplicatotron::SpawnCan);
	m_Animation->SetCurrentEndFunction("Duplicatotron_Destroyed", this, &CDuplicatotron::Destroyed);
}

bool CDuplicatotron::Init()
{
	if (!CMonster::Init())
	{
		return false;
	}

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	m_DetectArea = CreateComponent<CColliderOBB3D>("DetectArea");
	m_BodyCube = CreateComponent<CColliderOBB3D>("Body");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("HammerAnimation");

	m_Mesh->SetMesh("Duplicatotron");

	SetRootComponent(m_Mesh);

	//m_Mesh->SetMesh("Hammer");
	//m_Mesh->AddChild(m_DetectArea);
	//m_Mesh->AddChild(m_BodyCube);

	m_Mesh->AddChild(m_DetectArea);
	m_Mesh->AddChild(m_BodyCube);
	//m_Mesh->AddChild(m_Rigid);
	m_Mesh->SetWorldPosition(500.f, 0.f, 100.f);

	//auto iter = m_Mesh->GetMaterials()->begin();
	//auto iterEnd = m_Mesh->GetMaterials()->end();

	//for (; iter != iterEnd; iter++)
	//{
	//	(*iter)->SetEmissiveColor(1.f, 1.f, 1.f, 0.f);
	//}

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

	CHammer* Hammer = m_Scene->CreateObject<CHammer>("Hammer");

	if (Hammer->GetDead() == true)
	{
		--m_CountCan; 
		m_SpawnOn = true;
	}

	if (m_SpawnOn)
	{
		//m_CountCan = 0;

		m_DelayTime += DeltaTime;
		
		if (m_DelayTime >= 3.f && m_CountCan < 1)
		{
			++m_CountCan;

			SpawnCan();

			//m_DelayTime = 0.f;
		}

		if (m_DelayTime >= 5.f && m_CountCan < 2)
		{
			++m_CountCan;

			SpawnCan();

			//m_SpawnOn = false;

		}

		if (m_DelayTime >= 7.f && m_CountCan < 3)
		{
			++m_CountCan;

			SpawnCan();

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

void CDuplicatotron::Collision_Dead(const CollisionResult& result)
{
	std::string Name = result.Dest->GetCollisionProfile()->Name;

	if (Name == "PlayerAttack")
	{
		m_Animation->ChangeAnimation("Duplicatotron_Destroyed");
	}
}


void CDuplicatotron::Destroyed()
{
	m_Mesh->Destroy();
}

void CDuplicatotron::SpawnAnimation()
{
	m_Animation->ChangeAnimation("Duplicatotron_SpawnEnemies");
}

void CDuplicatotron::SpawnCan()
{
	//m_SpawnOn = true;

	Vector3 Pos = GetWorldPos();

	CDupli_Can* DupliCan = m_Scene->CreateObject<CDupli_Can>("Dupli_Can");
}

