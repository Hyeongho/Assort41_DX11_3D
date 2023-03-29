#include "RoboSponge_AttackWords.h"

#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Device.h"
#include "Engine.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "RoboSponge.h"
#include "../../Player.h"
#include "../../Object/CBL/CBL_Platform.h"


CRoboSponge_AttackWords::CRoboSponge_AttackWords()
{
	m_AttackWordType = EAttackWordType::KAE;
	m_MoveToPlatform = false;

	m_AttackDelayTimer = 0.f;
	m_StartTimer = false;
	m_Drop = false;
}

CRoboSponge_AttackWords::CRoboSponge_AttackWords(const CRoboSponge_AttackWords& Obj)
    : CGameObject(Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

	m_AttackWordType = Obj.m_AttackWordType;
	m_MoveToPlatform = Obj.m_MoveToPlatform;
}

CRoboSponge_AttackWords::~CRoboSponge_AttackWords()
{
}

void CRoboSponge_AttackWords::Start()
{
    CGameObject::Start();
}

bool CRoboSponge_AttackWords::Init()
{
    CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

	SetRootComponent(m_Mesh);
	m_Mesh->AddChild(m_Collider);

	m_Mesh->SetMesh("KAE");

	m_Collider->SetCollisionProfile("MonsterAttack");
	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
	m_Collider->SetCollisionCallback<CRoboSponge_AttackWords>(ECollision_Result::Collision, this, &CRoboSponge_AttackWords::Collision);
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

    return true;
}

void CRoboSponge_AttackWords::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

	if (m_MoveToPlatform) {
		// Move
		float Dist = GetWorldPos().Distance(m_DestPos);

		if (Dist >= 10.f) {
			Vector3 Dir = m_DestPos - GetWorldPos();

			Dir.Normalize();

			AddWorldPosition(Dir * 300.f * g_DeltaTime);
		}
		else {
			SetWorldPosition(m_DestPos);
			m_AttackDelayTimer = 0.f;
			m_MoveToPlatform = false;
			m_StartTimer = true;
		}
	}

	if (m_StartTimer) {
		m_AttackDelayTimer += g_DeltaTime;

		if (m_AttackDelayTimer >= 2.f) {
			m_AttackDelayTimer = 0.f;

			m_Drop = true;
			m_StartTimer = false;
		}
	}

	if (m_Drop) {
		AddWorldPositionY(-600 * g_DeltaTime);

		if (GetWorldPos().y <= 0.f) {
			Destroy();
		}
	}
}

void CRoboSponge_AttackWords::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CRoboSponge_AttackWords* CRoboSponge_AttackWords::Clone() const
{
    return new CRoboSponge_AttackWords(*this);
}

void CRoboSponge_AttackWords::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CRoboSponge_AttackWords::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CRoboSponge_AttackWords::Collision(const CollisionResult& result)
{
	// Platform Front Roll


	// Player Damage

}

void CRoboSponge_AttackWords::SetWordType(EAttackWordType WordType)
{
	std::string strWordType = "";

	switch (WordType)
	{
	case EAttackWordType::KAE:
		strWordType = "KAE";
		break;
	case EAttackWordType::RAH:
		strWordType = "RAH";
		break;
	case EAttackWordType::TAE:
		strWordType = "TAE";
		break;
	}

	m_Mesh->SetMesh(strWordType);

	// 아래 내용은 아마 하지 않아도 될 것으로 보임.
	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);
}
