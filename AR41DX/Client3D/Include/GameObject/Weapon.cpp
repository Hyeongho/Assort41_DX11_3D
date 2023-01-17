
#include "Weapon.h"
#include "Component/StaticMeshComponent.h"
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
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CWeapon::~CWeapon()
{
}

void CWeapon::Start()
{
	CGameObject::Start();

}

bool CWeapon::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Blade");

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
