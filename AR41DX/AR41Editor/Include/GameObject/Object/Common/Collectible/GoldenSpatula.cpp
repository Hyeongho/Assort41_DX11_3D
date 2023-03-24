#include "GoldenSpatula.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "../../../Player.h"

CGoldenSpatula::CGoldenSpatula()
{
	SetTypeID<CGoldenSpatula>();

	m_ObjectTypeName = "GoldenSpatula";

	m_ColItemType = EColItemType::GoldenSpatula;
}

CGoldenSpatula::CGoldenSpatula(const CGoldenSpatula& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

	m_ColItemType = Obj.m_ColItemType;
}

CGoldenSpatula::~CGoldenSpatula()
{
}

void CGoldenSpatula::Start()
{
	CCollectibleItems::Start();
}

bool CGoldenSpatula::Init()
{
	CCollectibleItems::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->SetMesh("GoldenSpatula");


	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CGoldenSpatula>(ECollision_Result::Collision, this, &CGoldenSpatula::Collision_Player);


	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

	return true;
}

void CGoldenSpatula::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);
}

void CGoldenSpatula::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CGoldenSpatula* CGoldenSpatula::Clone() const
{
	return new CGoldenSpatula(*this);
}

void CGoldenSpatula::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CGoldenSpatula::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}

void CGoldenSpatula::Collision_Player(const CollisionResult& result)
{
	CCollectibleItems::Collision_Player(result);

	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (Player) {
		// 플레이어 황금 뒤집개 추가
		// Player->AddGoldenSpatula();
	}

	// 컷씬 실행

	// 오브젝트 삭제처리
	Destroy();
}
