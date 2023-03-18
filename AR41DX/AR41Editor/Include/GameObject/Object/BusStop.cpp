#include "BusStop.h"

#include "Component/ColliderCube.h"
#include "Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "../../UI/InteractUI.h"
#include "../../UI/DialogUI.h"
#include "../Npc/BusDriver.h"

CBusStop::CBusStop() :
    m_BusExist(false)
{
    SetTypeID<CBusStop>();

    m_ObjectTypeName = "BusStop";
}

CBusStop::CBusStop(const CBusStop& Obj) :
    CGameObject(Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Cube = (CColliderCube*)FindComponent("Cube");

    m_BusExist = Obj.m_BusExist;
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

    //m_Mesh->SetMesh("Bus_Stop");

    return true;
}

void CBusStop::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    // BusExist Check
    if (m_Scene->FindObject("BusDriver"))
        m_BusExist = true;
    else
        m_BusExist = false;
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
    if (m_BusExist)
        return;

    CBusDriver* BusDriver = m_Scene->CreateObject<CBusDriver>("BusDriver");

    BusDriver->MoveToBusStop();
}
