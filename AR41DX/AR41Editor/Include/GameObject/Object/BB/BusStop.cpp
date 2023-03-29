#include "BusStop.h"

#include "Component/ColliderCube.h"
#include "Component/StaticMeshComponent.h"
#include "Component/TargetArm.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "../../Npc/BusDriver.h"
#include "../../Player.h"

CBusStop::CBusStop()
{
    SetTypeID<CBusStop>();

    m_ObjectTypeName = "BusStop";

    m_PurposeScene = EMapList::End;
}

CBusStop::CBusStop(const CBusStop& Obj) :
    CGameObject(Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Arm = (CTargetArm*)FindComponent("Arm");
    m_Camera = (CCameraComponent*)FindComponent("Camera");

    m_PurposeScene = Obj.m_PurposeScene;
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
    m_Arm = CreateComponent<CTargetArm>("Arm");
    m_Camera = CreateComponent<CCameraComponent>("Camera");

    SetRootComponent(m_Mesh);
    m_Mesh->AddChild(m_Arm);
    m_Arm->AddChild(m_Camera);

    m_Mesh->SetMesh("Bus_Stop");
    m_Mesh->SetWorldScale(5.f, 5.f, 5.f);

    float m_ArmHeight = m_Mesh->GetMeshSize().y * 6.f;

    m_Arm->SetWorldPositionY(2500.f);
    m_Arm->SetWorldRotationX(15.f);
    m_Arm->SetWorldRotationZ(15.f);
    

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

    //BusPos.x += 3000.f;
    BusPos.z += 3000.f;

    BusDriver->MoveToBusStop(GetWorldPos(), BusPos);
    BusDriver->SetPurposeScene(EMapList::Jelly_Fish_Field);
    //BusDriver->SetPurposeScene(m_PurposeScene);
}

void CBusStop::CutSceneStart()
{
    m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

    m_Arm->SetRelativePositionX(100.f);
    m_Arm->SetRelativePositionZ(-100.f);
    m_Arm->SetWorldPositionY(1200.f);

    CBusDriver* BusDriver = (CBusDriver*)m_Scene->FindObject("BusDriver");

    if (!BusDriver)
        BusDriver = m_Scene->CreateObject<CBusDriver>("BusDriver");

    Vector3 BusPos = GetWorldPos();

    float Degree = atan2(GetWorldPos().z - BusPos.z, GetWorldPos().x - BusPos.x);
    Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

    m_Camera->SetWorldRotationY(-Degree);
}
