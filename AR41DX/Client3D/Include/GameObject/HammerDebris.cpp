#include "HammerDebris.h"
#include "Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "../../Scene/LoadingSceneInfo.h"

CHammerDebris::CHammerDebris()
{
    SetTypeID<CHammerDebris>();

    m_ObjectTypeName = "HammerDebris";
}

CHammerDebris::CHammerDebris(const CHammerDebris& Obj)
{
    m_HammerDebrisMesh1 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh1");
    m_HammerDebrisMesh2 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh2");
    m_HammerDebrisMesh3 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh3");
    m_HammerDebrisMesh4 = (CStaticMeshComponent*)FindComponent("HammerDebrisMesh4");

    //m_Cube = (CColliderOBB3D*)FindComponent("Cube");
}

CHammerDebris::~CHammerDebris()
{
}

void CHammerDebris::Start()
{
    CGameObject::Start();
}

bool CHammerDebris::Init()
{
    CGameObject::Init();

    m_HammerDebrisMesh1 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh1");
    m_HammerDebrisMesh2 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh2");
    m_HammerDebrisMesh3 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh3");
    m_HammerDebrisMesh4 = CreateComponent<CStaticMeshComponent>("HammerDebrisMesh4");

    //m_Cube = CreateComponent<CColliderOBB3D>("Cube");

    m_Rigid = CreateComponent<CRigidBody>("Rigid");

    SetRootComponent(m_HammerDebrisMesh1);

    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh2);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh3);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh4);

    m_HammerDebrisMesh1->SetMesh("HammerDebris1");

    m_HammerDebrisMesh2->SetMesh("HammerDebris2");
    m_HammerDebrisMesh2->SetRelativePosition(0.f, 50.f, 150.f);

    m_HammerDebrisMesh3->SetMesh("HammerDebris3");
    m_HammerDebrisMesh4->SetRelativePosition(0.f, 50.f, 160.f);

    m_HammerDebrisMesh4->SetMesh("HammerDebris4");
    m_HammerDebrisMesh4->SetRelativePosition(0.f, 50.f, 50.f);

    m_HammerDebrisMesh1->SetWorldPositionY(200.f);

  


    //m_HammerDebris1->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CHammerDebris::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CHammerDebris::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CHammerDebris* CHammerDebris::Clone() const
{
    return new CHammerDebris(*this);
}

void CHammerDebris::Save(FILE* File)
{
}

void CHammerDebris::Load(FILE* File)
{
}

void CHammerDebris::Destroy()
{
}


