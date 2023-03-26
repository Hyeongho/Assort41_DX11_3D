#include "RoboSponge_AttackWords.h"

#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Device.h"
#include "Engine.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "RoboSponge.h"
#include "../../Player.h"


CRoboSponge_AttackWords::CRoboSponge_AttackWords()
{
	m_AttackWordType = EAttackWordType::End;
	m_MoveToPlatform = false;
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
	m_Collider->SetCollisionCallback<CRoboSponge_AttackWords>(ECollision_Result::Collision, this, &CRoboSponge_AttackWords::Collision_Player);
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);

    return true;
}

void CRoboSponge_AttackWords::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
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

void CRoboSponge_AttackWords::Collision_Player(const CollisionResult& result)
{




}

void CRoboSponge_AttackWords::SetWordType(EAttackWordType WordType)
{
	std::string strWordType = "";

	switch (WordType)
	{
	case EAttackWordType::KAE:
		strWordType = "KAE";
		break;
	case EAttackWordType::RAE:
		strWordType = "RAE";
		break;
	case EAttackWordType::TAE:
		strWordType = "TAE";
		break;
	}

	m_Mesh->SetMesh(strWordType);

}

void CRoboSponge_AttackWords::SetDestPos(Vector3 DestPos)
{
}
