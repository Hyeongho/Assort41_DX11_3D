#include "Fodder.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "Engine.h"

CFodder::CFodder()
{
	SetTypeID<CFodder>();

	m_ObjectTypeName = "Fodder";
}

CFodder::CFodder(const CFodder& Obj)
{
}

CFodder::~CFodder()
{
}

void CFodder::Start()
{
	CMonster::Start();

	// 테스트용 키세팅
	CInput::GetInst()->AddBindFunction<CFodder>("Q", Input_Type::Down, this, &CFodder::Chase, m_Scene);
	CInput::GetInst()->AddBindFunction<CFodder>("W", Input_Type::Down, this, &CFodder::Attack, m_Scene);
	CInput::GetInst()->AddBindFunction<CFodder>("E", Input_Type::Down, this, &CFodder::Dead, m_Scene);


	// 탐지범위에 플레이어가 들어올 시 Notice 애니메이션 후 Walk 로 변경.
	m_Animation->SetCurrentEndFunction("Fodder_Notice", this, &CFodder::Walk);
	m_Animation->SetCurrentEndFunction("Fodder_Dead", this, &CFodder::Walk);
}

bool CFodder::Init()
{
	if (!CMonster::Init())
	{
		return false;
	}

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Fodder");
	m_Mesh->SetWorldPosition(100.f, 50.f, 0.f);
	m_Animation = m_Mesh->SetAnimation<CAnimation>("FodderAnimation");

	m_Animation->AddAnimation("Fodder_Walk", "Fodder_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("Fodder_Attack", "Fodder_Attack", 1.f, 1.f, false);
	m_Animation->AddAnimation("Fodder_Notice", "Fodder_Notice", 1.f, 1.f, false);
	m_Animation->AddAnimation("Fodder_Dead", "Fodder_Dead", 1.f, 1.f, false);

	return true;
}

void CFodder::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	if (m_DetectRange) // 탐지 범위내에 있으면
	{
		if (m_Scene->FindObject("Player"))
			Chase();

		if (m_AttackRange) // 공격 범위 내에 있으면
		{
			if (m_Scene->FindObject("Player"))
				Attack();
		}
	}
}

void CFodder::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CFodder* CFodder::Clone() const
{
	return new CFodder(*this);
}

void CFodder::Save(FILE* File)
{
	CMonster::Save(File);
}

void CFodder::Load(FILE* File)
{
	CMonster::Load(File);
}

void CFodder::Walk()
{
	m_Animation->ChangeAnimation("Fodder_Walk");
}

void CFodder::Chase()
{
	m_Animation->ChangeAnimation("Fodder_Notice");

	Vector3 FodderPos = m_Mesh->GetWorldPos();
	Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

	Vector3 Dir = FodderPos - PlayerPos;

	Dir.y = 0.f; // Y축으로 이동 불가. 

	Dir.Normalize();

	// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 1.5배 빠르게.
	m_Mesh->AddWorldPosition(Dir * m_MoveSpeed * 1.5 * g_DeltaTime);
}

void CFodder::Attack()
{
	m_Animation->ChangeAnimation("Fodder_Attack");

	Vector3 FodderPos = m_Mesh->GetWorldPos();
	Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

	Vector3 Dir = FodderPos - PlayerPos;

	Dir.y = 0.f; // Y축으로 이동 불가. 

	Dir.Normalize();

	// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 2배 빠르게.
	m_Mesh->AddWorldPosition(Dir * 2 * m_MoveSpeed * g_DeltaTime);
}

void CFodder::Dead()
{
	m_Animation->ChangeAnimation("Fodder_Dead");

	SetMoveSpeed(0.f);
}

void CFodder::Debris()
{/* 일단보류
	m_Mesh->SetAnimation("DebrisAnimation");
	m_Mesh->ClearMaterial();
	m_Mesh->SetMesh("Debris");
	m_Anim[(int)m_MainCharacter]->Start();*/
}
