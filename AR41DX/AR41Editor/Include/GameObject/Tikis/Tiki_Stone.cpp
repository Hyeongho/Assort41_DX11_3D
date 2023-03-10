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
	: CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Animation = (CAnimation*)FindComponent("Tiki_Stone");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CTiki_Stone::~CTiki_Stone()
{
}

void CTiki_Stone::Start()
{
	CGameObject::Start();
}

bool CTiki_Stone::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);
	m_Mesh->SetMesh("Tiki_Stone");
	m_Mesh->AddChild(m_Collider);

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Monster");
	m_Collider->SetInheritRotY(true);

	m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiStoneAnimation");
	m_Animation->AddAnimation("Tiki_Stone_Idle", "Tiki_Stone_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Tiki_Stone_Die", "Tiki_Stone_Die", 1.f, 1.f, false);
	m_Animation->SetCurrentEndFunction<CTiki_Stone>("Tiki_Stone_Die", this, &CTiki_Stone::Tiki_Die);
	m_Animation->SetCurrentAnimation("Tiki_Stone_Idle");

    return true;
}

void CTiki_Stone::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


	// 항상 플레이어를 바라보게 한다.
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (!Player)
		return;

	Vector3 PlayerPos = Player->GetWorldPos();

	float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);
}

void CTiki_Stone::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTiki_Stone* CTiki_Stone::Clone() const
{
	return new CTiki_Stone(*this);
}

void CTiki_Stone::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTiki_Stone::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CTiki_Stone::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Tiki_Stone_Idle");
}

void CTiki_Stone::ChangeAnim_Die()
{
	m_Animation->ChangeAnimation("Tiki_Stone_Die");
}

void CTiki_Stone::Tiki_Die()
{
	CreateFlowers();
	Destroy();
}

void CTiki_Stone::CreateFlowers()
{
}

void CTiki_Stone::AttackedCollision(const CollisionResult& result)
{
	// 튕겨나가는 사운드 재생

}
