#include "IceCube.h"
#include "../Player.h"
#include "Scene/Scene.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"

CIceCube::CIceCube()
{
	SetTypeID<CIceCube>();
	m_ObjectTypeName = "IceCube";
}

CIceCube::CIceCube(const CIceCube& obj)
	: CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Body = (CColliderOBB3D*)FindComponent("Body");
}

CIceCube::~CIceCube()
{
}

void CIceCube::Start()
{
	CGameObject::Start();
	m_Body->SetCollisionCallback<CIceCube>(ECollision_Result::Collision, this, &CIceCube::CollisionIn);
	m_Body->SetCollisionCallback<CIceCube>(ECollision_Result::Release, this, &CIceCube::CollisionOut);
}

bool CIceCube::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Body = CreateComponent<CColliderOBB3D>("Body");

	m_Mesh->AddChild(m_Body);

	m_Mesh->SetMesh("Ice");
	m_Mesh->SetPivot(0.5f, 0.5f, 0.5f);

	//m_Body->SetCollisionProfile("Collectible");
	m_Body->SetRelativePositionY(50.f);
	m_Body->SetBoxHalfSize(100.f, 50.f, 100.f);
	return true;
}

void CIceCube::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CIceCube::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CIceCube* CIceCube::Clone() const
{
	return new CIceCube(*this);
}

void CIceCube::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CIceCube::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CIceCube::CollisionIn(const CollisionResult& result)
{
	std::string name = result.Dest->GetCollisionProfile()->Name;
	if (name == "Player")
	{
		CPlayer* player = (CPlayer*)m_Scene->GetPlayerObject();
		player->SetCanPickUp(true);
	}
}

void CIceCube::CollisionOut(const CollisionResult& result)
{
	std::string name = result.Dest->GetCollisionProfile()->Name;
	if (name == "Player")
	{
		CPlayer* player = (CPlayer*)m_Scene->GetPlayerObject();
		player->SetCanPickUp(false);
	}
}