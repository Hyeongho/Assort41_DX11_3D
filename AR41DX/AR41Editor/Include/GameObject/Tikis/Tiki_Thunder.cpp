#include "Tiki_Thunder.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "../Player.h"
#include "../Object/Common/Collectible/ShinyFlower.h"

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
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CTiki_Thunder::~CTiki_Thunder()
{
}

void CTiki_Thunder::Start()
{
	CGameObject::Start();
}

bool CTiki_Thunder::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);
	m_Mesh->SetMesh("Tiki_Thunder");
	m_Mesh->AddChild(m_Collider);


	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Monster");
	m_Collider->SetCollisionCallback<CTiki_Thunder>(ECollision_Result::Collision, this, &CTiki_Thunder::Collision_PlayerAttack);
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);


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

	// 꽃 생성
	CreateFlowers();

	// 플레이어가 가까이 있다면 피해 처리
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();
	// Player->SetDamage();


	// 사운드 처리
	int RandNum = rand() % 2 + 1;
	std::string TikiSound = "TikiThunderDie" + std::to_string(RandNum);
	CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

	if (Sound)
		Sound->Play();

	// 오브젝트 삭제 처리
	Destroy();
}

void CTiki_Thunder::CreateFlowers()
{
	// 5 ~ 12개의 플라워 생성
	int FlowerCount = rand() % 8 + 5;

	for (int i = 0; i < FlowerCount; i++) {
		CShinyFlower* Flower = m_Scene->CreateObject<CShinyFlower>("ShinyFlower_TikiThunder");

		// X, Y, Z -50 ~ 50 범위 내의 위치에서 꽃이 생겨날 수 있게 설정.
		int RandX = (float)(rand() % 100 - 50) + GetWorldPos().x;
		int RandY = (float)(rand() % 100 - 50) + GetWorldPos().y;
		int RandZ = (float)(rand() % 100 - 50) + GetWorldPos().z;

		Flower->SetWorldPosition(RandX, RandY, RandZ);
	}
}

void CTiki_Thunder::Collision_PlayerAttack(const CollisionResult& result)
{
	// 사운드 처리
	int RandNum = rand() % 3 + 1;
	std::string TikiSound = "TikiThunderHit" + std::to_string(RandNum);
	CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

	if (Sound)
		Sound->Play();

	// 사망 애니메이션 변경, 애니메이션 종료 시 사망 처리
	ChangeAnim_Die();
}
