#include "Sock.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "../../../Player.h"

CSock::CSock()
{
	SetTypeID<CSock>();

	m_ObjectTypeName = "Sock";

	m_ColItemType = EColItemType::Sock;
}

CSock::CSock(const CSock& Obj) :
	CCollectibleItems(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

	m_ColItemType = Obj.m_ColItemType;
}

CSock::~CSock()
{
}

void CSock::Start()
{
	CCollectibleItems::Start();
}

bool CSock::Init()
{
	CCollectibleItems::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->SetMesh("Sock");

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Collectible");
	m_Collider->SetCollisionCallback<CSock>(ECollision_Result::Collision, this, &CSock::Collision_Player);

	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

	return true;
}

void CSock::Update(float DeltaTime)
{
	CCollectibleItems::Update(DeltaTime);
}

void CSock::PostUpdate(float DeltaTime)
{
	CCollectibleItems::PostUpdate(DeltaTime);
}

CSock* CSock::Clone() const
{
	return new CSock(*this);
}

void CSock::Save(FILE* File)
{
	CCollectibleItems::Save(File);
}

void CSock::Load(FILE* File)
{
	CCollectibleItems::Load(File);
}

void CSock::Collision_Player(const CollisionResult& result)
{
	CCollectibleItems::Collision_Player(result);

	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (Player) {
		// 플레이어 양말 추가
		// Player->AddSock();
	}

	// 오브젝트 삭제처리
	Destroy();
}

