#include "Trampoline.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Component/RigidBody.h"
#include "Device.h"
#include "../../Player.h"
#include "Scene/Scene.h"

CTrampoline::CTrampoline()
{
	SetTypeID<CTrampoline>();

	m_ObjectTypeName = "Trampoline";
}

CTrampoline::CTrampoline(const CTrampoline& Obj)
	: CGameObject(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
}

CTrampoline::~CTrampoline()
{
}

void CTrampoline::Start()
{
	CGameObject::Start();
}

bool CTrampoline::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Collider);
	m_Mesh->SetMesh("Trampoline");

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionProfile("Trampoline");
	m_Collider->SetCollisionCallback<CTrampoline>(ECollision_Result::Collision, this, &CTrampoline::Collision_Bounce);
	m_Collider->SetCollisionCallback<CTrampoline>(ECollision_Result::Release, this, &CTrampoline::Release_Bounce);

	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);


	Vector2 Ratio = CDevice::GetInst()->GetHdRsRatio();
	Ratio.x = 1.f / Ratio.x;
	Ratio.y = 1.f / Ratio.y;

	SetRelativeScale(Ratio);
	m_Collider->SetRelativeScale(Ratio);

	return true;
}

void CTrampoline::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTrampoline::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTrampoline* CTrampoline::Clone() const
{
	return new CTrampoline(*this);
}

void CTrampoline::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTrampoline::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CTrampoline::Collision_Bounce(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

		CRigidBody* PlayerRigid = (CRigidBody*)Player->FindComponent("Rigid");
		PlayerRigid->AddForce(0, 1000.f);
		PlayerRigid->SetVelocityY(1000.f);
	}
}

void CTrampoline::Release_Bounce(const CollisionResult& result)
{
	if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
	{
		CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

		CRigidBody* PlayerRigid = (CRigidBody*)Player->FindComponent("Rigid");

		PlayerRigid->AddForce(0, 1000.f);
		PlayerRigid->SetVelocityY(1700.f);
	}

}