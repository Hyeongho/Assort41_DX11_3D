#include "Tiki_Thunder.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "../Player.h"

CTiki_Thunder::CTiki_Thunder()
{
	SetTypeID<CTiki_Thunder>();

	m_ObjectTypeName = "Tiki_Thunder";
}

CTiki_Thunder::CTiki_Thunder(const CTiki_Thunder& Obj)
	: CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Animation = (CAnimation*)FindComponent("TikiThunderAnimation");
	m_Cube = (CColliderCube*)FindComponent("Cube");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CTiki_Thunder::~CTiki_Thunder()
{
}

void CTiki_Thunder::Start()
{
	CGameObject::Start();

#ifdef DEBUG
	CInput::GetInst()->AddBindFunction<CTiki_Thunder>("F1", Input_Type::Up, this, &CTiki_Thunder::ChangeAnim_Idle, m_Scene);
	CInput::GetInst()->AddBindFunction<CTiki_Thunder>("F2", Input_Type::Up, this, &CTiki_Thunder::ChangeAnim_Die, m_Scene);
#endif // DEBUG

}

bool CTiki_Thunder::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");
	m_Cube = CreateComponent<CColliderCube>("Cube");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Tiki_Thunder");

	m_Mesh->AddChild(m_Cube);

	m_Cube->SetCubeSize(m_Mesh->GetMeshSize());
	m_Cube->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Cube->SetCollisionProfile("Monster");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiThunderAnimation");

	m_Animation->AddAnimation("Tiki_Thunder_Idle", "Tiki_Thunder_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Tiki_Thunder_Die", "Tiki_Thunder_Die", 1.f, 1.f, false);

	m_Animation->SetCurrentEndFunction<CTiki_Thunder>("Tiki_Thunder_Die", this, &CTiki_Thunder::Tiki_Die);

	m_Animation->SetCurrentAnimation("Tiki_Thunder_Idle");

	return true;
}

void CTiki_Thunder::Update(float DeltaTime)
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

void CTiki_Thunder::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTiki_Thunder* CTiki_Thunder::Clone() const
{
	return new CTiki_Thunder(*this);
}

void CTiki_Thunder::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTiki_Thunder::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CTiki_Thunder::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Tiki_Thunder_Idle");
}

void CTiki_Thunder::ChangeAnim_Die()
{
	m_Animation->ChangeAnimation("Tiki_Thunder_Die");
}

void CTiki_Thunder::Tiki_Die()
{
	// 폭발 파티클

	
	// 피해처리
	m_Scene->GetPlayerObject();


	// 데칼 처리


	CreateFlowers();

	// 본인 삭제.
	Destroy();
}

void CTiki_Thunder::CreateFlowers()
{
}

void CTiki_Thunder::AttackedCollision(const CollisionResult& result)
{
	ChangeAnim_Die();
}
