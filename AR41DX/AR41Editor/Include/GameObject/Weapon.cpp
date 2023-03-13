#include "Weapon.h"
//#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
//#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"

CWeapon::CWeapon()
	: m_Angle(0.f)
{
	SetTypeID<CWeapon>();
	m_ObjectTypeName = "Weapon";
}

CWeapon::CWeapon(const CWeapon& Obj) :
	CGameObject(Obj)
	, m_Angle(0.f)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
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
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Mesh->SetMesh("SpongebobWand");
	m_Mesh->SetWorldScale(0.5f, 0.5f, 0.5f);
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
	bool isMesh = m_Mesh->SetMesh(Name);
	if(isMesh&&Name=="Lasso")
	{
		m_Anim = m_Mesh->SetAnimation<CAnimation>("WeaponAnimation");
		m_Anim->AddAnimation("Lasso_Idle", "Lasso_Idle", 1.f, 1.f, true);
		m_Anim->AddAnimation("Lasso_Start", "Lasso_Start", 1.f, 1.f, false);
		m_Anim->SetCurrentEndFunction<CWeapon>("Lasso_Start", this, &CWeapon::StartLassoAtk);
		m_Anim->AddAnimation("Lasso_End", "Lasso_End", 1.f, 10.f, false);
		m_Anim->SetCurrentEndFunction<CWeapon>("Lasso_End", this, &CWeapon::ResetIdle);
		m_Anim->AddAnimation("Lasso_Copter", "Lasso_Copter", 1.f, 1.f, true);
		m_Anim->Start();
	}
	return isMesh;
}

void CWeapon::Lasso(const std::string& Name)
{
	m_Mesh->SetEnable(true);
	m_Anim->ChangeAnimation(Name);
}

void CWeapon::StartLassoAtk()
{
	m_Mesh->SetWorldRotation(0.f,m_Angle-45.f,-30.f);
	m_Anim->ChangeAnimation("Lasso_End");
}

void CWeapon::ResetIdle()
{
	m_Mesh->SetWorldScale(0.5f, 0.5f, 0.5f);
	m_Mesh->SetWorldRotation(0.f,0.f,0.f);
	m_Mesh->SetEnable(false);
	m_Anim->ChangeAnimation("Lasso_Idle");
}