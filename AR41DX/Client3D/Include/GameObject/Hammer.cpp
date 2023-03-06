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

CHammer::CHammer() :
	m_Stunned(false)
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

	m_HammerBT->Start();

	// 올가미 공격 당할시, Hammer_LassoedStart -> Hammer_Lassoed 로 변경
	m_Animation->SetCurrentEndFunction("Hammer_LassoedStart", this, &CHammer::Lassoed);

	// 탐지범위에 플레이어가 들어올 시 Notice 애니메이션 후 Walk 로 변경.
	m_Animation->SetCurrentEndFunction("Hammer_Notice", this, &CHammer::Walk);
	m_Animation->SetCurrentEndFunction("Hammer_Dead", this, &CHammer::Walk);
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

	m_Animation->AddAnimation("Hammer_Walk", "Hammer_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("Hammer_Attack", "Hammer_Attack", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Notice", "Hammer_Notice", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Dead", "Hammer_Dead", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Stunned", "Hammer_Stunned", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_LassoedStart", "Hammer_LassoedStart", 1.f, 1.f, false);
	m_Animation->AddAnimation("Hammer_Lassoed", "Hammer_Lassoed", 1.f, 1.f, false);


	m_HammerBT = new CHammerBT;

	return true;
}

void CHammer::Update(float DeltaTime)
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

	if (m_Stunned)
	{
		SetMoveSpeed(0.f);

		m_DeltaTime += g_DeltaTime;

		// 3초 지나면 스턴 해제
		if (m_DeltaTime >= 3.f)
		{
			SetMoveSpeed(200.f);

			m_DeltaTime = 0.f;

			m_Stunned = false;
		}
	}

	m_HammerBT->Run(this);

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

void CHammer::Walk()
{
}

void CHammer::Chase()
{
	m_Animation->ChangeAnimation("Hammer_Notice");

	Vector3 HammerPos = m_Mesh->GetWorldPos();
	Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

	Vector3 Dir = HammerPos - PlayerPos;

	Dir.y = 0.f; // Y축으로 이동 불가. 

	Dir.Normalize();

	// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 1.5배 빠르게.
	m_Mesh->AddWorldPosition(Dir * m_MoveSpeed * 1.5 * g_DeltaTime);
}

void CHammer::Attack()
{
	m_Animation->ChangeAnimation("Fodder_Attack");

	Vector3 HammerPos = m_Mesh->GetWorldPos();
	Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

	Vector3 Dir = HammerPos - PlayerPos;

	Dir.y = 0.f; // Y축으로 이동 불가. 

	Dir.Normalize();

	// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 2배 빠르게.
	m_Mesh->AddWorldPosition(Dir * 2 * m_MoveSpeed * g_DeltaTime);
}

void CHammer::Lassoed()
{
	// if() 올가미와 충돌시
	m_Animation->ChangeAnimation("Hammer_Lassoed");
}

void CHammer::Stunned()
{
	m_Stunned = true;

	m_Animation->ChangeAnimation("Hammer_Stunned");
}

void CHammer::Dead()
{
	m_Animation->ChangeAnimation("Hammer_Dead");

	SetMoveSpeed(0.f);

}

void CHammer::Debris()
{
}