#include "Tiki_Wood.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "../Player.h"
#include "../Object/Common/Collectible/ShinyFlower.h"

CTiki_Wood::CTiki_Wood()
{
	SetTypeID<CTiki_Wood>();

	m_ObjectTypeName = "Tiki_Wood";
}

CTiki_Wood::CTiki_Wood(const CTiki_Wood& Obj)
	: CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Animation = (CAnimation*)FindComponent("TikiWoodsAnimation");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CTiki_Wood::~CTiki_Wood()
{
}

void CTiki_Wood::Start()
{
	CGameObject::Start();
}

bool CTiki_Wood::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);
	m_Mesh->SetMesh("Tiki_Woods");
	m_Mesh->AddChild(m_Collider);

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Monster");
	m_Collider->SetCollisionCallback<CTiki_Wood>(ECollision_Result::Collision, this, &CTiki_Wood::Collision_PlayerAttack);
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

	m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiWoodsAnimation");
	m_Animation->AddAnimation("Tiki_Woods_Idle", "Tiki_Woods_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Tiki_Woods_Die", "Tiki_Woods_Die", 1.f, 1.f, false);
	m_Animation->SetCurrentEndFunction<CTiki_Wood>("Tiki_Woods_Die", this, &CTiki_Wood::Tiki_Die);
	m_Animation->SetCurrentAnimation("Tiki_Woods_Idle");

	return true;
}

void CTiki_Wood::Update(float DeltaTime)
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

void CTiki_Wood::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTiki_Wood* CTiki_Wood::Clone() const
{
	return new CTiki_Wood(*this);
}

void CTiki_Wood::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTiki_Wood::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CTiki_Wood::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Tiki_Woods_Idle");
}

void CTiki_Wood::ChangeAnim_Die()
{
	m_Animation->ChangeAnimation("Tiki_Woods_Die");
}

void CTiki_Wood::Tiki_Die()
{
	// 꽃 생성
	CreateFlowers();

	// 사운드 처리
	int RandNum = rand() % 2 + 1;
	std::string TikiSound = "TikiWoodDie" + std::to_string(RandNum);
	CSound* Sound = m_Scene->GetResource()->FindSound(TikiSound);

	if (Sound)
		Sound->Play();

	// 오브젝트 삭제 처리
	Destroy();
}

void CTiki_Wood::CreateFlowers()
{
	// 5 ~ 12개의 플라워 생성
	int FlowerCount = rand() % 8 + 5;
	
	for (int i = 0; i < FlowerCount; i++) {
		CShinyFlower* Flower = m_Scene->CreateObject<CShinyFlower>("ShinyFlower_TikiWood");
	
		// X, Y, Z -50 ~ 50 범위 내의 위치에서 꽃이 생겨날 수 있게 설정.
		int RandX = (float)(rand() % 100 - 50) + GetWorldPos().x;
		int RandY = (float)(rand() % 100 - 50) + GetWorldPos().y;
		int RandZ = (float)(rand() % 100 - 50) + GetWorldPos().z;

		Flower->SetWorldPosition(RandX, RandY, RandZ);
	}
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

