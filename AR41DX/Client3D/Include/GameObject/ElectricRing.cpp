#include "ElectricRing.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Jellyfish.h"
#include "Scene/Scene.h"
#include "Player.h"

CElectricRing::CElectricRing()
{
    SetTypeID<CElectricRing>();

    m_ObjectTypeName = "ElectricRing";
}

CElectricRing::CElectricRing(const CElectricRing& Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Collider = (CColliderOBB3D*)FindComponent("Collider");
    m_InnerCollider = (CColliderOBB3D*)FindComponent("Collider");
}

CElectricRing::~CElectricRing()
{
}

void CElectricRing::Start()
{
    CGameObject::Start();

    m_Collider->SetCollisionCallback<CElectricRing>(ECollision_Result::Collision, this, &CElectricRing::Collision);
    m_InnerCollider->SetCollisionCallback<CElectricRing>(ECollision_Result::Collision, this, &CElectricRing::InnerCollision);
}

bool CElectricRing::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
    m_Collider = CreateComponent<CColliderOBB3D>("Collider");
    m_InnerCollider = CreateComponent<CColliderOBB3D>("Collider");

    m_Mesh->SetMesh("KingJellyfish_Electric");

    auto iter = m_Mesh->GetMaterials()->begin();
    auto iterEnd = m_Mesh->GetMaterials()->end();

    for (; iter != iterEnd; iter++)
    {
        (*iter)->SetEmissiveColor(1.f, 1.f, 1.f, 0.f);
    }

    SetRootComponent(m_Mesh);

    m_Mesh->AddChild(m_Collider);
    m_Mesh->AddChild(m_InnerCollider);
    m_Mesh->SetWorldScale(1.f, 30.f, 1.f);

    m_Collider->SetBoxHalfSize(15.f, 30.f, 15.f);
    m_Collider->SetCollisionProfile("Monster");
    m_Collider->SetRelativePosition(0.f, 0.f, 0.f);

    m_InnerCollider->SetBoxHalfSize(14.f, 30.f, 14.f);
    m_InnerCollider->SetCollisionProfile("Monster");
    m_InnerCollider->SetRelativePosition(0.f, 0.f, 0.f);

    return true;
}

void CElectricRing::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    Vector3 Pos = m_Scene->FindObject("KingJellyfish")->GetWorldPos();

    // 항상 플레이어를 바라보게 한다.
    m_Player = (CPlayer*)m_Scene->GetPlayerObject();

    if (!m_Player)
        return;

    m_PlayerPos = m_Player->GetWorldPos();

    float Degree = atan2(GetWorldPos().z - m_PlayerPos.z, GetWorldPos().x - m_PlayerPos.x);
    Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

    m_Collider->SetWorldRotationY(Degree);
    m_InnerCollider->SetWorldRotationY(Degree);
    m_Mesh->SetWorldPosition(Pos);

    m_Mesh->AddWorldScale(0.1f, 0.f, 0.1f);
    m_Collider->AddWorldScale(0.1f, 0.f, 0.1f);
    m_InnerCollider->AddWorldScale(0.1f, 0.f, 0.1f);
}

void CElectricRing::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    Vector3 RingSize = m_Mesh->GetMeshSize();

    if (RingSize.x > 400.f)
    {
        Destroy();
    }

    if (m_Attack)
    {
        m_Player->InflictDamage(1);

        m_Attack = false;
    }
}

CElectricRing* CElectricRing::Clone() const
{
    return new CElectricRing(*this);
}

void CElectricRing::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CElectricRing::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CElectricRing::Collision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Name == "Player")
    {
        m_Attack = true;
    }
}

void CElectricRing::InnerCollision(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Name == "Player")
    {
        m_Collider->Destroy();
        m_InnerCollider->Destroy();
    }
}
