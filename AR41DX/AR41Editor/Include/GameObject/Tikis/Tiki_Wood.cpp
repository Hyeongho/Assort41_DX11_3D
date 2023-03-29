#include "Tiki_Wood.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"

CTiki_Wood::CTiki_Wood()
{
	SetTypeID<CTiki_Wood>();

	m_ObjectTypeName = "Tiki_Wood";
}

CTiki_Wood::CTiki_Wood(const CTiki_Wood& Obj)
	: CTikiBase(Obj)
{
	//m_Animation = (CAnimation*)FindComponent("TikiWoodAnimation");
	m_Animation = Obj.m_Animation;
}

CTiki_Wood::~CTiki_Wood()
{
}

void CTiki_Wood::Start()
{
	CTikiBase::Start();

	if(!m_Animation)
		CreateAnim();
}

bool CTiki_Wood::Init()
{
	CTikiBase::Init();

	// 메쉬 세팅
	m_Mesh->SetMesh("Tiki_Woods");
	m_Mesh->AddChild(m_Collider);
	m_Mesh->AddChild(m_Rigid);

	// 충돌체 세팅
	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Monster");
	m_Collider->SetCollisionCallback<CTiki_Wood>(ECollision_Result::Collision, this, &CTiki_Wood::Collision_PlayerAttack);
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);
	
	// 애니메이션 세팅
	CreateAnim();


	return true;
}

void CTiki_Wood::Update(float DeltaTime)
{
	CTikiBase::Update(DeltaTime);
}

void CTiki_Wood::PostUpdate(float DeltaTime)
{
	CTikiBase::PostUpdate(DeltaTime);
}

CTiki_Wood* CTiki_Wood::Clone() const
{
	return new CTiki_Wood(*this);
}

void CTiki_Wood::Save(FILE* File)
{
	CTikiBase::Save(File);
}

void CTiki_Wood::Load(FILE* File)
{
	CTikiBase::Load(File);
}

void CTiki_Wood::Tiki_Die()
{
	// 사운드 처리
	int RandNum = rand() % 2 + 1;
	std::string TikiSound = "TikiWoodDie" + std::to_string(RandNum);
	CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

	if (Sound)
		Sound->Play();


	CTikiBase::Tiki_Die();
}

void CTiki_Wood::CreateAnim()
{
	m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiWoodsAnimation");
	m_Animation->AddAnimation("Tiki_Woods_Idle", "Tiki_Woods_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Tiki_Woods_Die", "Tiki_Woods_Die", 1.f, 1.f, false);
	m_Animation->SetCurrentEndFunction<CTiki_Wood>("Tiki_Woods_Die", this, &CTiki_Wood::Tiki_Die);
	m_Animation->SetCurrentAnimation("Tiki_Woods_Idle");
}

void CTiki_Wood::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Tiki_Woods_Idle");
}

void CTiki_Wood::ChangeAnim_Die()
{
	m_Animation->ChangeAnimation("Tiki_Woods_Die");
}

void CTiki_Wood::Collision_PlayerAttack(const CollisionResult& result)
{
	//if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::PlayerAttack) {
	std::string ProfileName = result.Dest->GetCollisionProfile()->Name;

	if (strcmp("PlayerAttack", ProfileName.c_str()) == 0) {
		// 사운드 처리
		int RandNum = rand() % 3 + 1;
		std::string TikiSound = "TikiWoodHit" + std::to_string(RandNum);
		CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

		if (Sound)
			Sound->Play();

		// 사망 애니메이션 변경, 애니메이션 종료 시 사망 처리
		ChangeAnim_Die();
	}
}

