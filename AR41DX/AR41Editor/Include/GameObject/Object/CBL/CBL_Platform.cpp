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
	m_TotalRollDegree(0.f),
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
}

CCBL_Platform::~CCBL_Platform()
{
}

void CCBL_Platform::Start()
{
	CGameObject::Start();

#ifdef _DEBUG
	//CInput::GetInst()->AddBindFunction<CCBL_Platform>("F1", Input_Type::Up, this, &CCBL_Platform::DebugF1, m_Scene);
	//CInput::GetInst()->AddBindFunction<CCBL_Platform>("F2", Input_Type::Up, this, &CCBL_Platform::DebugF2, m_Scene);
	//CInput::GetInst()->AddBindFunction<CCBL_Platform>("F3", Input_Type::Up, this, &CCBL_Platform::DebugF3, m_Scene);
	//CInput::GetInst()->AddBindFunction<CCBL_Platform>("F4", Input_Type::Up, this, &CCBL_Platform::DebugF4, m_Scene);
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
	m_Collider->SetCollisionCallback<CCBL_Platform>(ECollision_Result::Collision, this, &CCBL_Platform::Collision_Rolling);

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
		if (m_RollCount == 0) {
			if (m_TotalRollDegree >= 900.f) {
				m_TotalRollDegree = 0.f;
				m_RollCount++;

				switch (m_RollDir)
				{
				case ERollDir::Left:
					m_RollDir = ERollDir::Right;
					break;
				case ERollDir::Right:
					m_RollDir = ERollDir::Left;
					break;
				case ERollDir::Front:
					m_RollDir = ERollDir::Back;
					break;
				case ERollDir::Back:
					m_RollDir = ERollDir::Front;
					break;
				}
			}
		}
		else if (m_RollCount == 1) {
			if (m_TotalRollDegree >= 270.f) {
				m_TotalRollDegree = 0.f;
				m_RollCount++;

				switch (m_RollDir)
				{
				case ERollDir::Left:
					m_RollDir = ERollDir::Right;
					break;
				case ERollDir::Right:
					m_RollDir = ERollDir::Left;
					break;
				case ERollDir::Front:
					m_RollDir = ERollDir::Back;
					break;
				case ERollDir::Back:
					m_RollDir = ERollDir::Front;
					break;
				}
			}
		}
		else if (m_RollCount == 2) {
			if (m_TotalRollDegree >= 90.f) {
				m_TotalRollDegree = 0.f;
				m_Roll = false;
			}
		}


		float RotSpeed = 0.f;

		if (m_RollDir == ERollDir::Left || m_RollDir == ERollDir::Front) {
			RotSpeed = g_DeltaTime * -1000.f;
		}
		else if(m_RollDir == ERollDir::Right || m_RollDir == ERollDir::Back) {
			RotSpeed = g_DeltaTime * 1000.f;
		}

		if (m_RollDir == ERollDir::Left || m_RollDir == ERollDir::Right) {
			AddWorldRotationZ(RotSpeed);
		}
		else if (m_RollDir == ERollDir::Front || m_RollDir == ERollDir::Back) {
			AddWorldRotationX(RotSpeed);
		}

		m_TotalRollDegree += abs(RotSpeed);
	}
	else {
		m_RollCount = 0;

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

void CCBL_Platform::Collision_Rolling(const CollisionResult& result)
{
	if (m_Roll) { // 회전 도중 부딪히는 경우 플레이어에게 피해

	}
}

void CCBL_Platform::SetLookBoss(const Vector3& BossPos)
{
	// Z축이 보스와 직교하도록 설정.
	float Degree = atan2(GetWorldPos().z - BossPos.z, GetWorldPos().x - BossPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);
}

void CCBL_Platform::Roll(const ERollDir& RollDir)
{
	m_Roll = true;

	switch (RollDir)
	{
	case ERollDir::Left:
		m_RollDir = ERollDir::Left;
		break;
	case ERollDir::Right:
		m_RollDir = ERollDir::Right;
		break;
	case ERollDir::Front:
		m_RollDir = ERollDir::Front;
		break;
	case ERollDir::Back:
		m_RollDir = ERollDir::Back;
		break;
	}
}

void CCBL_Platform::DebugF1()
{
	// 플랫폼 좌회전 처리
	m_Roll = true;
	m_RollDir = ERollDir::Left;
}

void CCBL_Platform::DebugF2()
{
	// 플랫폼 우회전 처리
	m_Roll = true;
	m_RollDir = ERollDir::Right;
}

void CCBL_Platform::DebugF3()
{
	// 플랫폼 전방 회전 처리
	m_Roll = true;
	m_RollDir = ERollDir::Front;
}

void CCBL_Platform::DebugF4()
{
	// 플랫폼 후방 회전 처리
	m_Roll = true;
	m_RollDir = ERollDir::Back;
}
