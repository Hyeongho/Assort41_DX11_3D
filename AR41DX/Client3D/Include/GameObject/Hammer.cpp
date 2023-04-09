#include "Hammer.h"
#include "Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Engine.h"
#include "HammerDebris.h"
#include "Duplicatotron.h"

CHammer::CHammer() :
	//m_Stunned(false)
	m_DetectOn(false),
	m_AttackOn(false),
	m_WeaponAttack(false),
	m_DebrisOn(false),
	m_Dead(false)
{
	SetTypeID<CHammer>();

	m_ObjectTypeName = "Hammer";
}

CHammer::CHammer(const CHammer& Obj)
{
	m_DetectArea = (CColliderOBB3D*)FindComponent("DetectArea");
	m_AttackArea = (CColliderOBB3D*)FindComponent("AttackArea");
	m_BodyCube = (CColliderOBB3D*)FindComponent("BodyCube");
	m_WeaponCube = (CColliderOBB3D*)FindComponent("WeaponCube");

	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CHammer::~CHammer()
{
}

void CHammer::Start()
{
	CMonster::Start();

	// �׽�Ʈ�� Ű����
	CInput::GetInst()->AddBindFunction<CHammer>("F", Input_Type::Down, this, &CHammer::Debris, m_Scene);

	// �ð��� ���� ���ҽ�, Hammer_LassoedStart -> Hammer_Lassoed �� ����
	//m_Animation->SetCurrentEndFunction("Hammer_LassoedStart", this, &CHammer::Lassoed);

	// Ž�������� �÷��̾ ���� �� Notice �ִϸ��̼� �� Walk �� ����.
	//m_Animation->SetCurrentEndFunction("Hammer_Notice", this, &CHammer::Walk);
	m_Animation->SetCurrentEndFunction("Hammer_Dead", this, &CHammer::Debris);
	m_Animation->SetCurrentEndFunction("Hammer_Attack", this, &CHammer::WeaponAttackOn);
	m_Animation->AddCurrentNotify<CHammer>("Hammer_Attack", "Hammer_Attack", 14, this, &CHammer::AttackSound);


	m_DetectArea->SetCollisionCallback<CHammer>(ECollision_Result::Collision, this, &CHammer::Collision_Detect_ChaseOn);
	m_DetectArea->SetCollisionCallback<CHammer>(ECollision_Result::Release, this, &CHammer::Release_Detect_ChaseOff);

	m_AttackArea->SetCollisionCallback<CHammer>(ECollision_Result::Collision, this, &CHammer::Collision_AttackOn);
	m_AttackArea->SetCollisionCallback<CHammer>(ECollision_Result::Release, this, &CHammer::Release_AttackOff);

	m_BodyCube->SetCollisionCallback<CHammer>(ECollision_Result::Collision, this, &CHammer::Collision_Body);

	m_WeaponCube->SetCollisionCallback<CHammer>(ECollision_Result::Collision, this, &CHammer::Collision_WeaponAttack);
	m_WeaponCube->SetCollisionCallback<CHammer>(ECollision_Result::Release, this, &CHammer::Release_WeaponAttackOff);

}

bool CHammer::Init()
{
	if (!CMonster::Init())
	{
		return false;
	}

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	m_DebrisMesh1 = CreateComponent <CStaticMeshComponent>("DebrisMesh1");
	m_DebrisMesh2 = CreateComponent <CStaticMeshComponent>("DebrisMesh2");
	m_DebrisMesh3 = CreateComponent <CStaticMeshComponent>("DebrisMesh3");
	m_DebrisMesh4 = CreateComponent <CStaticMeshComponent>("DebrisMesh4");

	m_DetectArea = CreateComponent<CColliderOBB3D>("DetectArea");
	m_AttackArea = CreateComponent<CColliderOBB3D>("AttackArea");
	m_BodyCube = CreateComponent<CColliderOBB3D>("BodyCube");
	m_WeaponCube = CreateComponent<CColliderOBB3D>("WeaponCube");



	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	SetRootComponent(m_Mesh);
	
	m_Mesh->SetMesh("Hammer");
	m_Mesh->SetWorldPosition(130.f, 50.f, 150.f);
	m_Mesh->AddChild(m_DetectArea);
	m_Mesh->AddChild(m_AttackArea);
	m_Mesh->AddChild(m_BodyCube);
	m_Mesh->AddChild(m_WeaponCube);

	//m_DebrisMesh1->SetMesh("FodderDebris1");
	//m_DebrisMesh2->SetMesh("FodderDebris2");
	//m_DebrisMesh3->SetMesh("FodderDebris3");
	//m_DebrisMesh4->SetMesh("FodderDebris4");

	m_DetectArea->SetCollisionProfile("DetectArea");
	m_DetectArea->SetBoxHalfSize(800.f, 400.f, 800.f);
	m_DetectArea->SetRelativePosition(0.f, 400.f);

	m_AttackArea->SetCollisionProfile("DetectArea");
	m_AttackArea->SetBoxHalfSize(300.f, 50.f, 300.f);
	m_AttackArea->SetRelativePosition(0.f, 70.f);

	m_BodyCube->SetCollisionProfile("Monster");
	m_BodyCube->SetRelativePosition(-30.f, 130.f, -70.f);
	m_BodyCube->SetBoxHalfSize(135.f, 130.f, 50.f);

	m_WeaponCube->SetCollisionProfile("MonsterAttack");
	m_WeaponCube->SetRelativePosition(-80.f, 300.f, 10.f);
	m_WeaponCube->SetBoxHalfSize(100.f, 130.f, 100.f);
	m_WeaponCube->SetInheritRotX(true);
	m_WeaponCube->SetInheritRotY(true);
	m_WeaponCube->SetInheritRotZ(true);


	m_Animation = m_Mesh->SetAnimation<CAnimation>("HammerAnimation");

	m_Animation->AddAnimation("Hammer_Walk", "Hammer_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("Hammer_Attack", "Hammer_Attack", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Notice", "Hammer_Notice", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Dead", "Hammer_Dead", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Stunned", "Hammer_Stunned", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_LassoedStart", "Hammer_LassoedStart", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Lassoed", "Hammer_Lassoed", 1.f, 1.f, false);

	return true;
}

void CHammer::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	// Ž���������� ������ 
	if (m_DetectOn)
	{
		// m_Animation->ChangeAnimation("Fodder_Notice");

		Detect_Chase();

		// ���ݹ������� ������
		if (m_AttackOn)
		{
			m_Animation->ChangeAnimation("Hammer_Attack");

			if (m_Animation->GetCurrentAnimationName() == "Hammer_Attack")
			{
				float HammerSpeed = GetMoveSpeed();

				HammerSpeed = 0.f;
			}
		}
	}


	//AddWorldPositionX(300.f * DeltaTime);
	 
	 
	//if (m_Stunned)
	//{
	//	SetMoveSpeed(0.f);

	//	m_DeltaTime += g_DeltaTime;

	//	// 3�� ������ ���� ����
	//	if (m_DeltaTime >= 3.f)
	//	{
	//		SetMoveSpeed(200.f);

	//		m_DeltaTime = 0.f;

	//		m_Stunned = false;
	//	}
	//}
	



	
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

	m_DetectArea = (CColliderOBB3D*)FindComponent("DetectArea");
	m_AttackArea = (CColliderOBB3D*)FindComponent("AttackArea");
	m_BodyCube = (CColliderOBB3D*)FindComponent("BodyCube");
	m_WeaponCube = (CColliderOBB3D*)FindComponent("WeaponCube");

	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

void CHammer::Walk()
{
}

void CHammer::Detect_Chase()
{
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (!Player)
		return;

	Vector3 HammerPos = CSceneManager::GetInst()->GetScene()->FindObject("Hammer")->GetWorldPos();
	Vector3 PlayerPos = CSceneManager::GetInst()->GetScene()->GetPlayerObject()->GetWorldPos();

	Vector3 Dir = PlayerPos - HammerPos;

	Dir.y = 0.f; // Y������ �̵� �Ұ�. 

	Dir.Normalize();

	float HammerSpeed = GetMoveSpeed();

	// m_MoveSpeed�� Monster Ŭ�������� 100.f�� �Ǿ��ִ�. �ӵ� 2�� ������.
	AddWorldPosition(Dir * 2 * HammerSpeed * g_DeltaTime);

	// �÷��̾� �ٶ󺸰Բ�.
	float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);
}

void CHammer::Attack()
{
	m_Animation->ChangeAnimation("Hammer_Attack");
}

//void CHammer::Lassoed()
//{
//	// if() �ð��̿� �浹��
//	m_Animation->ChangeAnimation("Hammer_Lassoed");
//}
//
//void CHammer::Stunned()
//{
//	m_Stunned = true;
//
//	m_Animation->ChangeAnimation("Hammer_Stunned");
//}

void CHammer::WeaponAttackOn()
{
	m_WeaponAttack = true;
}

void CHammer::Dead()
{
	m_Mesh->AddWorldPositionZ(150.f);
	m_Rigid->SetVelocity(0.f, 250.f, 200.f);
	m_Rigid->AddForce(0.f, 100.f, 100.f);
	m_Animation->ChangeAnimation("Hammer_Dead");

	SetMoveSpeed(0.f);
}

void CHammer::Debris()
{
	//if (!m_Scene->FindObject("HammerDebris"))
	//{
		Vector3 HammerPos = GetWorldPos();

		m_Mesh->Destroy();
		m_Mesh->ClearMaterial();

		m_Dead = true;

		CHammerDebris* Debris = m_Scene->CreateObject<CHammerDebris>("HammerDebris");
		Debris->SetWorldPosition(HammerPos.x, HammerPos.y + 500.f, HammerPos.z);

		// --CountCan & SpawnHammers in Duplicatotron class.
		CDuplicatotron* Duplicatotron = (CDuplicatotron*)m_Scene->FindObject("Duplicatotron");
		if (Duplicatotron)
		{
			Duplicatotron->AddCountCan(-1);
			Duplicatotron->SetSpawnOn(true);
		}
		
	//}
}

void CHammer::AttackSound()
{
	CResourceManager::GetInst()->SoundPlay("Ham_Hit");
	CResourceManager::GetInst()->SetVolume(30.f);
}

void CHammer::Collision_Detect_ChaseOn(const CollisionResult& result)
{
	std::string Name = result.Dest->GetCollisionProfile()->Name;

	if (Name == "Player")
	{
		m_DetectOn = true;
	}
}

void CHammer::Release_Detect_ChaseOff(const CollisionResult& result)
{
	if (result.Dest != nullptr)
	{
		std::string Name = result.Dest->GetCollisionProfile()->Name;

		if (Name == "Player")
		{
			m_DetectOn = false;
		}
	}
}

void CHammer::Collision_AttackOn(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		m_AttackOn = true;
	}
}

void CHammer::Release_AttackOff(const CollisionResult& result)
{
	// �÷��̾ ������� �� ���
	if (result.Dest != nullptr)
	{
		std::string Name = result.Dest->GetCollisionProfile()->Name;

		if (Name == "Player")
		{
			m_AttackOn = false;
			m_Animation->ChangeAnimation("Hammer_Walk");
		}
	}
}

void CHammer::Collision_Body(const CollisionResult& result)
{
	// ������ �޴� ��.
	std::string Name = result.Dest->GetCollisionProfile()->Name;

	if (Name == "Player")
	{
		result.Dest->GetOwner()->InflictDamage(1);
	}

	if (Name == "PlayerAttack")
	{
		CResourceManager::GetInst()->SoundPlay("Robot_Explode");
		//CResourceManager::GetInst()->SetVolume(2.f);
		Debris();
	}
}

void CHammer::Collision_WeaponAttack(const CollisionResult& result)
{
	std::string Name = result.Dest->GetCollisionProfile()->Name;

	if (Name == "Player" && m_WeaponAttack)
	{
		result.Dest->GetOwner()->InflictDamage(1);
	}
}

void CHammer::Release_WeaponAttackOff(const CollisionResult& result)
{
	m_WeaponAttack = false;
}
