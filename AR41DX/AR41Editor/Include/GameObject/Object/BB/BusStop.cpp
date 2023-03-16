#include "BusStop.h"

#include "Component/ColliderCube.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "../../Npc/BusDriver.h"

CBusStop::CBusStop()
{
    SetTypeID<CBusStop>();

    m_ObjectTypeName = "BusStop";
}

CBusStop::CBusStop(const CBusStop& Obj) :
    CGameObject(Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CBusStop::~CBusStop()
{
}

void CBusStop::Start()
{
    CGameObject::Start();
}

bool CBusStop::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Bus_Stop");
    m_Mesh->SetWorldScale(5.f, 5.f, 5.f);


    return true;
}

void CBusStop::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CBusStop::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CBusStop* CBusStop::Clone() const
{
    return new CBusStop(*this);
}

void CBusStop::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CBusStop::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CBusStop::CallBus()
{
    CBusDriver* BusDriver = (CBusDriver*)m_Scene->FindObject("BusDriver");

    if (!BusDriver)
        BusDriver = m_Scene->CreateObject<CBusDriver>("BusDriver");

    Vector3 BusPos = GetWorldPos();
    BusPos.x += 3000.f;

    BusDriver->MoveToBusStop(GetWorldPos(), BusPos);
}
