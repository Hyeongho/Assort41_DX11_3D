#include "CBL_Platform.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Device.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "../../Player.h"

CCBL_Platform::CCBL_Platform() :
	m_Roll(false),
	m_RollDir(ERollDir::Left),
	m_RollCount(0)
{
	SetTypeID<CCBL_Platform>();

	m_ObjectTypeName = "CBL_Platform";
}

CCBL_Platform::CCBL_Platform(const CCBL_Platform& Obj) :
	CGameObject(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

	m_Roll = Obj.m_Roll;
	m_RollDir = Obj.m_RollDir;
	m_RollCount = Obj.m_RollCount;
}

CCBL_Platform::~CCBL_Platform()
{
}

void CCBL_Platform::Start()
{
	CGameObject::Start();

#ifdef _DEBUG
	CInput::GetInst()->AddBindFunction<CCBL_Platform>("F1", Input_Type::Up, this, &CCBL_Platform::DebugF1, m_Scene);
	CInput::GetInst()->AddBindFunction<CCBL_Platform>("F2", Input_Type::Up, this, &CCBL_Platform::DebugF2, m_Scene);
#endif // DEBUG
}

bool CCBL_Platform::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);
	m_Mesh->SetMesh("CBL_Platform");

	m_Mesh->AddChild(m_Collider);

	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetCollisionProfile("Platform");
	m_Collider->SetCollisionCallback<CCBL_Platform>(ECollision_Result::Collision, this, &CCBL_Platform::RoboSpongeAttackedCollision);

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

void CCBL_Platform::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_Roll) {
		if (m_RollCount >= 1080.f) {
			m_Roll = false;
			m_RollCount = 0.f;
		}

		float RotSpeed = 0.f;

		if (m_RollDir == ERollDir::Left) {
			RotSpeed = g_DeltaTime * -1000.f;
		}
		else {
			RotSpeed = g_DeltaTime * 1000.f;
		}

		AddWorldRotationZ(RotSpeed);
		m_RollCount += abs(RotSpeed);

	}
	else {
		SetWorldRotationZ(0.f);
		SetWorldRotationX(0.f);
	}
}

void CCBL_Platform::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CCBL_Platform* CCBL_Platform::Clone() const
{
	return new CCBL_Platform(*this);
}

void CCBL_Platform::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CCBL_Platform::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CCBL_Platform::RoboSpongeAttackedCollision(const CollisionResult& result)
{
}

void CCBL_Platform::SetLookBoss(const Vector3& BossPos)
{
	// Z���� ������ �����ϵ��� ����.
	float Degree = atan2(GetWorldPos().z - BossPos.z, GetWorldPos().x - BossPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);
}

void CCBL_Platform::DebugF1()
{
	// �÷��� ��ȸ�� ó��
	m_Roll = true;
	m_RollDir = ERollDir::Left;
}

void CCBL_Platform::DebugF2()
{
	// �÷��� ��ȸ�� ó��
	m_Roll = true;
	m_RollDir = ERollDir::Right;
}
