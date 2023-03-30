#include "HammerDebris.h"
#include "Player.h"
#include "Hammer.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "../Scene/LoadingSceneInfo.h"
#include "Scene/NavigationManager3D.h"
#include "Component/RigidBody.h"

CHammerDebris::CHammerDebris() :
    m_DefyingGravity(true)
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

    m_Mesh1Cube = (CColliderOBB3D*)FindComponent("Mesh1Cube");
    m_Mesh2Cube = (CColliderOBB3D*)FindComponent("Mesh2Cube");
    m_Mesh3Cube = (CColliderOBB3D*)FindComponent("Mesh3Cube");
    m_Mesh4Cube = (CColliderOBB3D*)FindComponent("Mesh4Cube");
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

    m_Mesh1Cube = CreateComponent<CColliderOBB3D>("Mesh1Cube");
    m_Mesh2Cube = CreateComponent<CColliderOBB3D>("Mesh2Cube");
    m_Mesh3Cube = CreateComponent<CColliderOBB3D>("Mesh3Cube");
    m_Mesh4Cube = CreateComponent<CColliderOBB3D>("Mesh4Cube");

    //m_Cube = CreateComponent<CColliderOBB3D>("Cube");

    m_Rigid = CreateComponent<CRigidBody>("Rigid");

    SetRootComponent(m_HammerDebrisMesh1);

    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh2);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh3);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh4);
    m_HammerDebrisMesh1->AddChild(m_Rigid);
    m_HammerDebrisMesh1->AddChild(m_Mesh1Cube);


    m_HammerDebrisMesh2->AddChild(m_Rigid);
    m_HammerDebrisMesh2->AddChild(m_Mesh2Cube);

    m_HammerDebrisMesh3->AddChild(m_Rigid);
    m_HammerDebrisMesh3->AddChild(m_Mesh3Cube);

    m_HammerDebrisMesh4->AddChild(m_Rigid);
    m_HammerDebrisMesh4->AddChild(m_Mesh4Cube);


    m_HammerDebrisMesh1->SetMesh("HammerDebris1");
    m_HammerDebrisMesh2->SetMesh("HammerDebris2");
    m_HammerDebrisMesh3->SetMesh("HammerDebris3");
    m_HammerDebrisMesh4->SetMesh("HammerDebris4");
    //m_HammerDebris1->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);


    m_Hammer = (CHammer*)m_Scene->FindObject("Hammer");

    m_HammerDebrisMesh1->SetWorldPositionX(m_Hammer->GetWorldPos().x * 2.f);
    m_HammerDebrisMesh1->SetWorldPositionY(m_Hammer->GetWorldPos().y + 10.f);
    m_HammerDebrisMesh1->SetWorldPositionZ(m_Hammer->GetWorldPos().z);

    m_HammerDebrisMesh2->SetRelativePosition(10.f, 10.f, 20.f);
    m_HammerDebrisMesh2->SetRelativeRotation(10.f, 20.f, 20.f);

    m_HammerDebrisMesh3->SetRelativePosition(20.f, 10.f, 20.f);
    m_HammerDebrisMesh3->SetRelativeRotation(15.f, 20.f, 34.f);

    m_HammerDebrisMesh4->SetRelativePosition(30.f, 10.f, 20.f);
    m_HammerDebrisMesh4->SetRelativeRotation(24.f, 20.f, 10.f);


    m_Mesh1Cube->SetCollisionProfile("Monster");
    m_Mesh1Cube->SetBoxHalfSize(30.f, 30.f, 30.f);
    m_Mesh1Cube->SetRelativePosition(0.f, 0.f);

    m_Mesh2Cube->SetCollisionProfile("Monster");
    m_Mesh2Cube->SetBoxHalfSize(30.f, 30.f, 30.f);
    m_Mesh2Cube->SetRelativePosition(0.f, 0.f);

    m_Mesh3Cube->SetCollisionProfile("Monster");
    m_Mesh3Cube->SetBoxHalfSize(30.f, 30.f, 30.f);
    m_Mesh3Cube->SetRelativePosition(0.f, 0.f);

    m_Mesh4Cube->SetCollisionProfile("Monster");
    m_Mesh4Cube->SetBoxHalfSize(30.f, 30.f, 30.f);
    m_Mesh4Cube->SetRelativePosition(0.f, 0.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CHammerDebris::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();

    float Y = Nav->GetHeight(GetWorldPos());

    // °øÁß¿¡ ¶¹À» ¶§
    if (m_DefyingGravity)
    {
        m_Rigid->SetGround(false);
        //m_Rigid->AddForce(0, 1.f);
        m_Rigid->SetVelocityY(250.f);
    }

    float PosY = GetWorldPos().y;

    if (GetWorldPos().y >= 480.f)
    {
        m_DefyingGravity = false;
        m_Rigid->SetVelocityY(-20.f);
        m_Rigid->AddForce(300, 10.f, 0.f);
        m_HammerDebrisMesh1->AddWorldPosition(100.f, 0.f, 0.f);
        m_HammerDebrisMesh2->AddRelativePosition(-50.f, 0.f, 19.f);
        m_HammerDebrisMesh3->AddRelativePosition(-40.f, 0.f, 5.f);
        m_HammerDebrisMesh4->AddRelativePosition(-35.f, 0.f, 0.f);
    }

    // ¶¥¿¡ ÂøÁö
    if (Y != FLT_MAX && GetWorldPos().y - Y < m_HammerDebrisMesh1->GetMeshSize().y / 2.f && m_HammerDebrisMesh1)
    {
        SetWorldPositionY(Y + m_HammerDebrisMesh1->GetMeshSize().y / 2.f);
        m_Rigid->SetGround(true);
        m_Rigid->SetVelocityY(0.f);
        m_Rigid->AddForce(0, 0.f);
    }
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


