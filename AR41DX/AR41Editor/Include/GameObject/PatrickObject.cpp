#include "PatrickObject.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderCube.h"
#include "Component/RigidBody.h"

CPatrickObject::CPatrickObject()
	: m_Speed(50.f)
{
	SetTypeID<CPatrickObject>();
	m_ObjectTypeName = "PatrickObject";
}

CPatrickObject::CPatrickObject(const CPatrickObject& obj)
	: CGameObject(obj)
	, m_Speed(obj.m_Speed)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Body = (CColliderCube*)FindComponent("Body");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CPatrickObject::~CPatrickObject()
{
}

void CPatrickObject::Start()
{
	CGameObject::Start();
}

bool CPatrickObject::Init()
{
	CGameObject::Init();
	SetLifeTime(3.f);

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Body = CreateComponent<CColliderCube>("Body");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	m_Mesh->AddChild(m_Body);
	m_Mesh->AddChild(m_Rigid);

	m_Mesh->SetMesh("Ice");
	m_Mesh->SetWorldScale(0.5f,0.5f,0.5f);
	m_Mesh->SetPivot(0.5f, 0.5f,0.5f);

	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetRelativePositionY(25.f);
	m_Body->SetCubeSize(50.f, 50.f, 50.f);
	m_Body->SetCollisionCallback<CPatrickObject>(ECollision_Result::Collision, this, &CPatrickObject::CollisionObject);

	m_Rigid->SetGravity(true);
	return true;
}

void CPatrickObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

}

void CPatrickObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPatrickObject* CPatrickObject::Clone() const
{
	return new CPatrickObject(*this);
}

void CPatrickObject::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPatrickObject::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CPatrickObject::SetAngle(float f)
{
	SetWorldRotationY(f);
	AddWorldPosition(sinf(DegreeToRadian(f)) * 100.f, 0.f, cosf(DegreeToRadian(f)) * 100.f);
	m_Rigid->AddForce(sinf(DegreeToRadian(f)) * 400.f, 400.f, cosf(DegreeToRadian(f)) * 400.f);
	m_Rigid->SetVelocity(sinf(DegreeToRadian(f)) * 400.f, 400.f, cosf(DegreeToRadian(f)) * 400.f);
}

void CPatrickObject::CollisionObject(const CollisionResult& result)
{
	Destroy();
	//effect
}
