#include "TikiBase.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/NavigationManager3D.h"
#include "../Player.h"

CTikiBase::CTikiBase()
{
	SetTypeID<CTikiBase>();

	m_ObjectTypeName = "TikiBase";

	m_SetGravity = false;
	m_OnTiki = false;
}

CTikiBase::CTikiBase(const CTikiBase& Obj)
	: CMonster(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
	m_ColliderBottom = (CColliderOBB3D*)FindComponent("OBB3D");
	m_ColliderTop = (CColliderOBB3D*)FindComponent("OBB3D");
}

CTikiBase::~CTikiBase()
{
}

void CTikiBase::Start()
{
	CMonster::Start();

#ifdef _DEBUG
	CInput::GetInst()->AddBindFunction<CTikiBase>("F1", Input_Type::Up, this, &CTikiBase::DebugKeyF1, m_Scene);
	CInput::GetInst()->AddBindFunction<CTikiBase>("F2", Input_Type::Up, this, &CTikiBase::DebugKeyF2, m_Scene);
#endif // _DEBUG

}

bool CTikiBase::Init()
{
	CMonster::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");
	m_ColliderBottom = CreateComponent<CColliderOBB3D>("OBB3D");
	m_ColliderTop = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);
	m_Mesh->AddChild(m_Collider);
	m_Mesh->AddChild(m_ColliderBottom);
	m_Mesh->AddChild(m_ColliderTop); 
	m_Mesh->AddChild(m_Rigid);


	return true;
}

void CTikiBase::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	// 항상 플레이어를 바라보게 한다.
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (!Player)
		return;

	Vector3 PlayerPos = Player->GetWorldPos();

	float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);


	// 중력 조절
	if (!m_SetGravity)
		return;

	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	float Height = Nav->GetHeight(GetWorldPos());

	float Dist = GetWorldPos().y - Height;

	if (Dist < 10.f) {
		// 땅에 있을 때 Pos를 지면의 Height로
		SetWorldPositionY(Height);
	}
	else {
		// 다른 티기의 위가 아니면서, 지면에 있지 않을 때, Y Pos를 조정
		if (m_OnTiki)
			return;

		AddWorldPositionY(-1000.f * g_DeltaTime);
	}
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

void CTikiBase::Collision_Tikis(const CollisionResult& result)
{
	//if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::PlayerAttack) {
	std::string ProfileName = result.Dest->GetCollisionProfile()->Name;

	if (strcmp("Platform", ProfileName.c_str()) == 0) {
		m_OnTiki = true;
	}
}

void CTikiBase::Release_Tikis(const CollisionResult& result)
{
	//if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::PlayerAttack) {
	std::string ProfileName = result.Dest->GetCollisionProfile()->Name;

	if (strcmp("Platform", ProfileName.c_str()) == 0) {
		m_OnTiki = false;
	}
}

void CTikiBase::DebugKeyF1()
{
	m_SetGravity = false;
}

void CTikiBase::DebugKeyF2()
{
	m_SetGravity = true;
}
