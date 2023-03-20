#include "ElectricRing.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Jellyfish.h"
#include "Scene/Scene.h"

CElectricRing::CElectricRing()
{
    SetTypeID<CElectricRing>();

    m_ObjectTypeName = "ElectricRing";
}

CElectricRing::CElectricRing(const CElectricRing& Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Collider = (CColliderOBB3D*)FindComponent("Collider");
}

CElectricRing::~CElectricRing()
{
}

void CElectricRing::Start()
{
    CGameObject::Start();
}

bool CElectricRing::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
    m_Collider = CreateComponent<CColliderOBB3D>("Collider");

    m_Mesh->SetMesh("KingJellyfish_Electric");

    auto iter = m_Mesh->GetMaterials()->begin();
    auto iterEnd = m_Mesh->GetMaterials()->end();

    for (; iter != iterEnd; iter++)
    {
        (*iter)->SetEmissiveColor(1.f, 1.f, 1.f, 0.f);
    }

    SetRootComponent(m_Mesh);
    
    m_Mesh->AddChild(m_Collider);
    m_Mesh->SetWorldScale(1.f, 20.f, 1.f);
    
    Vector3 Size = m_Mesh->GetWorldScale();

    m_Collider->SetBoxHalfSize(Size);
    m_Collider->SetCollisionProfile("MonsterAttack");

    return true;
}

void CElectricRing::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    Vector3 Pos = m_Scene->FindObject("KingJellyfish")->GetWorldPos();

    m_Mesh->SetWorldPosition(Pos);

    m_Mesh->AddWorldScale(0.2f, 0.f, 0.2f);
    m_Collider->SetInheritScale(true);
}

void CElectricRing::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    Vector3 RingSize = m_Mesh->GetMeshSize();

    if (RingSize.x > 1000.f)
    {
        Destroy();
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

