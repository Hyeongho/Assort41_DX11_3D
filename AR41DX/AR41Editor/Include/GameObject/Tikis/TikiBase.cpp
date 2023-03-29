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

	// �׻� �÷��̾ �ٶ󺸰� �Ѵ�.
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (!Player)
		return;

	Vector3 PlayerPos = Player->GetWorldPos();

	float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);


	// �߷� ����
	if (!m_SetGravity)
		return;

	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	float Height = Nav->GetHeight(GetWorldPos());

	float Dist = GetWorldPos().y - Height;

	if (Dist < 10.f) {
		// ���� ���� �� Pos�� ������ Height��
		SetWorldPositionY(Height);
	}
	else {
		// �ٸ� Ƽ���� ���� �ƴϸ鼭, ���鿡 ���� ���� ��, Y Pos�� ����
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
	// �� ����
	CreateFlowers();

	// ������Ʈ ���� ó��
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
