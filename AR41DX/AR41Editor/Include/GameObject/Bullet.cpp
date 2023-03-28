#include "Bullet.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderCube.h"

CBullet::CBullet()
	: m_Speed(1000.f)
	, m_Angle(0.f)
{
	SetTypeID<CBullet>();

	m_ObjectTypeName = "Bullet";
}

CBullet::CBullet(const CBullet& Obj)
	: CGameObject(Obj)
	, m_Speed(Obj.m_Speed)
	, m_Angle(0.f)
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
}

bool CBullet::Init()
{
	CGameObject::Init();
	SetLifeTime(3.f);

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Body = CreateComponent<CColliderCube>("Body");

	m_Mesh->AddChild(m_Body);

	m_Mesh->SetMesh("SpongebobMissile");
	m_Mesh->SetPivot(0.5f, 0.5f);

	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetCollisionCallback<CBullet>(ECollision_Result::Collision, this, &CBullet::CollisionBullet);

	m_Anim = m_Mesh->SetAnimation<CAnimation>("SpongebobMissileAnimation");
	m_Anim->AddAnimation("MissileStart", "SpongebobMissile_Start", 1.f, 1.f, false);
	m_Anim->SetCurrentEndFunction<CBullet>("MissileStart", this, &CBullet::ResetIdle);
	m_Anim->AddAnimation("MissileIdle", "SpongebobMissile_Idle", 1.f, 1.f, true);
	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_Anim->GetCurrentAnimationName() == "MissileIdle")
	{		
		AddWorldPositionX(sinf(DegreeToRadian(m_Angle)) * m_Speed * DeltaTime);
		AddWorldPositionZ(cosf(DegreeToRadian(m_Angle)) * m_Speed * DeltaTime);
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

void CBullet::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CBullet::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CBullet::SetAngle(float f)
{
	SetWorldRotationY(f -90.f);
	AddWorldPositionX(sinf(DegreeToRadian(f-135.f)) * 100.f);
	AddWorldPositionZ(cosf(DegreeToRadian(f-135.f)) * 100.f);
	m_Angle = f-180.f;
	AddWorldPositionY(50.f);
}

void CBullet::CollisionBullet(const CollisionResult& result)
{
	std::string name = result.Dest->GetCollisionProfile()->Name;
	if (name == "Monster")
	{
		result.Dest->GetOwner()->InflictDamage(1);
	}
	Destroy();
}

void CBullet::ResetIdle()
{
	m_Anim->ChangeAnimation("MissileIdle");
}
