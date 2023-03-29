#include "Tiki_Stone.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "../Player.h"

CTiki_Stone::CTiki_Stone()
{
	SetTypeID<CTiki_Stone>();

	m_ObjectTypeName = "Tiki_Stone";
}

CTiki_Stone::CTiki_Stone(const CTiki_Stone& Obj)
	: CTikiBase(Obj)
{
	m_Animation = (CAnimation*)FindComponent("TikiStoneAnimation");
}

CTiki_Stone::~CTiki_Stone()
{
}

void CTiki_Stone::Start()
{
	CTikiBase::Start();

	CreateAnim();

	m_Collider->SetCollisionCallback<CTiki_Stone>(ECollision_Result::Collision, this, &CTiki_Stone::Collision_PlayerAttack);
	m_ColliderBottom->SetCollisionCallback<CTiki_Stone>(ECollision_Result::Collision, this, &CTiki_Stone::Collision_Tikis);
	m_ColliderBottom->SetCollisionCallback<CTiki_Stone>(ECollision_Result::Release, this, &CTiki_Stone::Release_Tikis);
}

bool CTiki_Stone::Init()
{
	CTikiBase::Init();

	// 메쉬 세팅
	m_Mesh->SetMesh("Tiki_Stone");

	// 충돌체 세팅
	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Monster");
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);


	Vector3 TBColSize = m_Mesh->GetMeshSize() / 2.f;
	TBColSize.y /= 10.f;

	// 바닥 충돌체 생성
	m_ColliderBottom->SetBoxHalfSize(TBColSize);
	m_ColliderBottom->SetRelativePositionY(TBColSize.y / 2.f);
	m_ColliderBottom->SetCollisionProfile("TikiBottom");
	m_ColliderBottom->SetInheritRotX(true);
	m_ColliderBottom->SetInheritRotY(true);
	m_ColliderBottom->SetInheritRotZ(true);

	// 위 충돌체 생성
	m_ColliderTop->SetBoxHalfSize(TBColSize);
	m_ColliderTop->SetRelativePositionY(m_Mesh->GetMeshSize().y - TBColSize.y / 2.f);
	m_ColliderTop->SetCollisionProfile("Platform");
	m_ColliderTop->SetInheritRotX(true);
	m_ColliderTop->SetInheritRotY(true);
	m_ColliderTop->SetInheritRotZ(true);

	// 애니메이션 세팅
	CreateAnim();

    return true;
}

void CTiki_Stone::Update(float DeltaTime)
{
	CTikiBase::Update(DeltaTime);
}

void CTiki_Stone::PostUpdate(float DeltaTime)
{
	CTikiBase::PostUpdate(DeltaTime);
}

CTiki_Stone* CTiki_Stone::Clone() const
{
	return new CTiki_Stone(*this);
}

void CTiki_Stone::Save(FILE* File)
{
	CTikiBase::Save(File);
}

void CTiki_Stone::Load(FILE* File)
{
	CTikiBase::Load(File);
}

void CTiki_Stone::Tiki_Die()
{
	CTikiBase::Tiki_Die();
}

void CTiki_Stone::CreateAnim()
{
	if (m_Animation)
		return;


	m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiStoneAnimation");
	m_Animation->AddAnimation("Tiki_Stone_Idle", "Tiki_Stone_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Tiki_Stone_Die", "Tiki_Stone_Die", 1.f, 1.f, false);
	m_Animation->SetCurrentEndFunction<CTiki_Stone>("Tiki_Stone_Die", this, &CTiki_Stone::Tiki_Die);
	m_Animation->SetCurrentAnimation("Tiki_Stone_Idle");
}

void CTiki_Stone::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Tiki_Stone_Idle");
}

void CTiki_Stone::ChangeAnim_Die()
{
	m_Animation->ChangeAnimation("Tiki_Stone_Die");
}

void CTiki_Stone::Collision_PlayerAttack(const CollisionResult& result)
{
	//if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::PlayerAttack) {
	std::string ProfileName = result.Dest->GetCollisionProfile()->Name;

	if (strcmp("PlayerAttack", ProfileName.c_str()) == 0) {
		// 사운드 처리
		int RandNum = rand() % 4 + 1;
		std::string TikiSound = "TikiStoneHit" + std::to_string(RandNum);
		CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

		if (Sound)
			Sound->Play();
	}
}
