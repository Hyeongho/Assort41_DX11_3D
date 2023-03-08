#include "Weapon.h"
//#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"

CWeapon::CWeapon()
{
	SetTypeID<CWeapon>();
	m_ObjectTypeName = "Weapon";
}

CWeapon::CWeapon(const CWeapon& Obj) :
	CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Body = (CColliderOBB3D*)FindComponent("Body");
}

CWeapon::~CWeapon()
{
}

void CWeapon::Start()
{
	CGameObject::Start();
	m_Body->SetCollisionCallback<CWeapon>(ECollision_Result::Collision, this, &CWeapon::CollisionWeapon);
}

bool CWeapon::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Body = CreateComponent<CColliderOBB3D>("Body");

	SetRootComponent(m_Mesh);
	m_Mesh->AddChild(m_Body);

	m_Mesh->SetMesh("SpongebobWand");
	m_Mesh->SetWorldScale(0.5f, 0.5f, 0.5f);

	m_Body->SetRelativePositionY(70.f);
	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetBoxHalfSize(m_Mesh->GetMeshSize());
	m_Body->SetInheritRotX(true);
	m_Body->SetInheritRotY(true);
	m_Body->SetInheritRotZ(true);
	return true;
}

void CWeapon::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CWeapon::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CWeapon* CWeapon::Clone() const
{
	return new CWeapon(*this);
}

void CWeapon::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CWeapon::Load(FILE* File)
{
	CGameObject::Load(File);
}

bool CWeapon::SetMesh(const std::string& Name)
{
	return 	m_Mesh->SetMesh(Name);
}

void CWeapon::CollisionWeapon(const CollisionResult& result)
{
	std::string name = result.Dest->GetCollisionProfile()->Name;
	if (name == "Monster")
	{
		result.Dest->GetOwner()->InflictDamage(1);
	}
}