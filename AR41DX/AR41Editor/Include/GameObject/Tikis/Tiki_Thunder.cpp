#include "Tiki_Thunder.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "../Player.h"

CTiki_Thunder::CTiki_Thunder()
{
	SetTypeID<CTiki_Thunder>();

	m_ObjectTypeName = "Tiki_Thunder";
}

CTiki_Thunder::CTiki_Thunder(const CTiki_Thunder& Obj)
	: CTikiBase(Obj)
{
	m_Animation = (CAnimation*)FindComponent("TikiThunderAnimation");
}

CTiki_Thunder::~CTiki_Thunder()
{
}

void CTiki_Thunder::Start()
{
	CTikiBase::Start();

	CreateAnim();
}

bool CTiki_Thunder::Init()
{
	CTikiBase::Init();

	// 메쉬 세팅
	m_Mesh->SetMesh("Tiki_Thunder");

	// 충돌체 세팅
	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Monster");
	m_Collider->SetCollisionCallback<CTiki_Thunder>(ECollision_Result::Collision, this, &CTiki_Thunder::Collision_PlayerAttack);
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);


	Vector3 TBColSize = m_Mesh->GetMeshSize() / 2.f;
	TBColSize.y /= 10.f;

	// 바닥 충돌체 생성
	m_ColliderTop->SetBoxHalfSize(TBColSize);
	m_ColliderTop->SetRelativePositionY(TBColSize.y / 2.f);
	m_ColliderTop->SetCollisionProfile("TikiBottom");
	m_ColliderTop->SetInheritRotX(true);
	m_ColliderTop->SetInheritRotY(true);
	m_ColliderTop->SetInheritRotZ(true);

	// 위 충돌체 생성
	m_ColliderBottom->SetBoxHalfSize(TBColSize);
	m_ColliderBottom->SetRelativePositionY(m_Mesh->GetMeshSize().y - TBColSize.y / 2.f);
	m_ColliderBottom->SetCollisionProfile("Platform");
	m_ColliderBottom->SetCollisionCallback<CTiki_Thunder>(ECollision_Result::Collision, this, &CTiki_Thunder::Collision_Tikis);
	m_ColliderBottom->SetCollisionCallback<CTiki_Thunder>(ECollision_Result::Release, this, &CTiki_Thunder::Release_Tikis);
	m_ColliderBottom->SetInheritRotX(true);
	m_ColliderBottom->SetInheritRotY(true);
	m_ColliderBottom->SetInheritRotZ(true);

	// 애니메이션 세팅
	CreateAnim();

	return true;
}

void CTiki_Thunder::Update(float DeltaTime)
{
	CTikiBase::Update(DeltaTime);
}

void CTiki_Thunder::PostUpdate(float DeltaTime)
{
	CTikiBase::PostUpdate(DeltaTime);
}

CTiki_Thunder* CTiki_Thunder::Clone() const
{
	return new CTiki_Thunder(*this);
}

void CTiki_Thunder::Save(FILE* File)
{
	CTikiBase::Save(File);
}

void CTiki_Thunder::Load(FILE* File)
{
	CTikiBase::Load(File);
}

void CTiki_Thunder::Tiki_Die()
{
	// 폭발 파티클


	// 플레이어가 가까이 있다면 피해 처리
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	// 폭발 범위. 이 범위 내에 플레이어가 있다면 피해 처리.
	float BombRange = 500.f;

	if (BombRange >= Player->GetWorldPos().Distance(GetWorldPos()))
		Player->InflictDamage();


	// 사운드 처리
	int RandNum = rand() % 2 + 1;
	std::string TikiSound = "TikiThunderDie" + std::to_string(RandNum);
	CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

	if (Sound)
		Sound->Play();


	CTikiBase::Tiki_Die();
}


void CTiki_Thunder::CreateAnim()
{
	if (m_Animation)
		return;


	m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiThunderAnimation");
	m_Animation->AddAnimation("Tiki_Thunder_Idle", "Tiki_Thunder_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Tiki_Thunder_Die", "Tiki_Thunder_Die", 1.f, 1.f, false);
	m_Animation->SetCurrentEndFunction<CTiki_Thunder>("Tiki_Thunder_Die", this, &CTiki_Thunder::Tiki_Die);
	m_Animation->SetCurrentAnimation("Tiki_Thunder_Idle");
}

void CTiki_Thunder::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Tiki_Thunder_Idle");
}

void CTiki_Thunder::ChangeAnim_Die()
{
	m_Animation->ChangeAnimation("Tiki_Thunder_Die");
}

void CTiki_Thunder::Collision_PlayerAttack(const CollisionResult& result)
{
	//if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::PlayerAttack) {
	std::string ProfileName = result.Dest->GetCollisionProfile()->Name;

	if (strcmp("PlayerAttack", ProfileName.c_str()) == 0) {
		// 사운드 처리
		int RandNum = rand() % 3 + 1;
		std::string TikiSound = "TikiThunderHit" + std::to_string(RandNum);
		CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

		if (Sound)
			Sound->Play();

		// 사망 애니메이션 변경, 애니메이션 종료 시 사망 처리
		ChangeAnim_Die();
	}
}
