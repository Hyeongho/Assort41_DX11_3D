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
    m_Mesh->SetWorldScale(1.f, 30.f, 1.f);

    m_Collider->SetBoxHalfSize(30.f, 3.f, 30.f);
    m_Collider->SetCollisionProfile("MonsterAttack");
    m_Collider->SetRelativePosition(0.f, 0.f, 0.f);

    return true;
}

void CElectricRing::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);


    m_Mesh->AddWorldScale(0.1f, 0.f, 0.1f);
    m_Collider->AddWorldScale(0.1f, 0.f, 0.1f);
    //Vector3 Pos = m_Scene->FindObject("KingJellyfish")->GetWorldPos();

    //// 항상 플레이어를 바라보게 한다.
    //CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

    //if (!Player)
    //    return;

    //Vector3 PlayerPos = Player->GetWorldPos();

    //float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
    //Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

    //m_Collider->SetWorldRotationY(Degree);

    //m_Mesh->SetWorldPosition(Pos);


    // Vector3 Size = m_Mesh->GetWorldScale() / 2;
   // m_Collider->AddWorldScale(Size);
}

void CElectricRing::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    Vector3 RingSize = m_Mesh->GetMeshSize();

    if (RingSize.x > 500.f)
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

