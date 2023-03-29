#include "TikiBase.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "../Player.h"

CTikiBase::CTikiBase()
{
	SetTypeID<CTikiBase>();

	m_ObjectTypeName = "TikiBase";
}

CTikiBase::CTikiBase(const CTikiBase& Obj)
	: CMonster(Obj)
{
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
}

CTikiBase::~CTikiBase()
{
}

void CTikiBase::Start()
{
	CMonster::Start();
}

bool CTikiBase::Init()
{
	CMonster::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);

	return true;
}

void CTikiBase::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	

	// 항상 플레이어를 바라보게 한다.
	//CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	//if (!Player)
	//	return;

	//Vector3 PlayerPos = Player->GetWorldPos();

	//float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
	//Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	//SetWorldRotationY(Degree);
}

void CTikiBase::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CTikiBase* CTikiBase::Clone() const
{
	return new CTikiBase(*this);
}

void CTikiBase::Save(FILE* File)
{
	CMonster::Save(File);
}

void CTikiBase::Load(FILE* File)
{
	CMonster::Load(File);
}

void CTikiBase::Tiki_Die()
{
	// 꽃 생성
	CreateFlowers();

	// 오브젝트 삭제 처리
	Destroy();
}
