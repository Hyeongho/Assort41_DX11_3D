
#include "Bullet.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderCube.h"

CBullet::CBullet()
	: m_Speed(50.f)
{
	SetTypeID<CBullet>();

	m_ObjectTypeName = "Bullet";
}

CBullet::CBullet(const CBullet& Obj)
	: CGameObject(Obj)
	, m_Speed(Obj.m_Speed)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Body = (CColliderCube*)FindComponent("Body");
}

CBullet::~CBullet()
{
}

void CBullet::Start()
{
	CGameObject::Start();

	m_Anim = m_Mesh->SetAnimation<CAnimation>("SpongebobMissileAnimation");
	m_Anim->AddAnimation("MissileStart", "SpongebobMissile_Start", 1.f, 1.f, false);
	m_Anim->SetCurrentEndFunction<CBullet>("MissileStart", this, &CBullet::ResetIdle);
	m_Anim->AddAnimation("MissileIdle", "SpongebobMissile_Idle", 1.f, 1.f, true);
	m_Anim->Start();
}

bool CBullet::Init()
{
	CGameObject::Init();
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Body = CreateComponent<CColliderCube>("Body");

	m_Mesh->AddChild(m_Body);

	m_Mesh->SetMesh("SpongebobMissile");
	m_Mesh->SetPivot(0.5f, 0.5f);

	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetCollisionCallback<CBullet>(ECollision_Result::Collision, this, &CBullet::CollisionBullet);
	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_Anim->GetCurrentAnimationName() == "MissileIdle")
	{
		AddWorldPosition(m_Dir * m_Speed * DeltaTime);
	}
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBullet* CBullet::Clone() const
{
	return new CBullet(*this);
}

void CBullet::SetDir(const Vector3& vec)
{
	m_Dir= GetWorldPos()- vec ;
	m_Dir.y = 0;
}

void CBullet::CollisionBullet(const CollisionResult& result)
{
	Destroy();
}

void CBullet::ResetIdle()
{
	m_Anim->ChangeAnimation("MissileIdle");
}
