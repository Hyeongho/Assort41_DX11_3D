#include "ElectricRing.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"

CElectricRing::CElectricRing()
{
    SetTypeID<CElectricRing>();

    m_ObjectTypeName = "ElectricRing";
}

CElectricRing::CElectricRing(const CElectricRing& Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
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
    m_Mesh->SetMesh("KingJellyfish_Electric");

    SetRootComponent(m_Mesh);

    m_Mesh->SetWorldScale(80.f, 80.f, 80.f);


    return true;
}

void CElectricRing::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    Vector3 Pos = m_Scene->FindObject("KingJellyfish")->GetWorldPos();

    m_Mesh->SetWorldPosition(Pos);

    m_Mesh->AddWorldScaleX(20.f);
}

void CElectricRing::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);

    Vector3 RingSize = m_Mesh->GetWorldScale();

    if ( RingSize.x > 800.f)
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

