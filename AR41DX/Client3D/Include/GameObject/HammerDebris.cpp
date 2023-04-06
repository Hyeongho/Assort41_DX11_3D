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

    m_Rigid1 = (CRigidBody*)FindComponent("Rigid1");    
    m_Rigid2 = (CRigidBody*)FindComponent("Rigid2");
    m_Rigid3 = (CRigidBody*)FindComponent("Rigid3");
    m_Rigid4 = (CRigidBody*)FindComponent("Rigid4");

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

    m_Rigid1 = CreateComponent<CRigidBody>("Rigid1");
    m_Rigid2 = CreateComponent<CRigidBody>("Rigid2");
    m_Rigid3 = CreateComponent<CRigidBody>("Rigid3");
    m_Rigid4 = CreateComponent<CRigidBody>("Rigid4");

    SetRootComponent(m_HammerDebrisMesh1);

    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh2);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh3);
    m_HammerDebrisMesh1->AddChild(m_HammerDebrisMesh4);
    m_HammerDebrisMesh1->AddChild(m_Rigid1);
    m_HammerDebrisMesh1->AddChild(m_Mesh1Cube);


    m_HammerDebrisMesh2->AddChild(m_Rigid2);
    m_HammerDebrisMesh2->AddChild(m_Mesh2Cube);

    m_HammerDebrisMesh3->AddChild(m_Rigid3);
    m_HammerDebrisMesh3->AddChild(m_Mesh3Cube);

    m_HammerDebrisMesh4->AddChild(m_Rigid4);
    m_HammerDebrisMesh4->AddChild(m_Mesh4Cube);


    m_HammerDebrisMesh1->SetMesh("HammerDebris1");
    m_HammerDebrisMesh2->SetMesh("HammerDebris2");
    m_HammerDebrisMesh3->SetMesh("HammerDebris3");
    m_HammerDebrisMesh4->SetMesh("HammerDebris4");
    //m_HammerDebris1->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);


    m_Hammer = (CHammer*)m_Scene->FindObject("Hammer");

    m_HammerDebrisMesh1->SetWorldPositionX(m_Hammer->GetWorldPos().x);
    m_HammerDebrisMesh1->SetWorldPositionY(m_Hammer->GetWorldPos().y);
    m_HammerDebrisMesh1->SetWorldPositionZ(m_Hammer->GetWorldPos().z);
    m_HammerDebrisMesh1->SetWorldRotationZ(50.f);

    m_HammerDebrisMesh2->SetRelativePosition(-150.f, 0.f, 100.f); // ¸Ó¸®
    //m_HammerDebrisMesh2->SetRelativeRotation(10.f, 20.f, 20.f);

    m_HammerDebrisMesh3->SetRelativePosition(-200.f, 0.f, -120.f);
    //m_HammerDebrisMesh3->SetRelativeRotation(15.f, 100.f, 34.f);

    m_HammerDebrisMesh4->SetRelativePosition(-70.f, 0.f, 20.f);
    //m_HammerDebrisMesh4->SetRelativeRotation(24.f, 50.f, 10.f);


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

    m_Rigid1->SetGravity(true);
    m_Rigid2->SetGravity(true);
    m_Rigid3->SetGravity(true);
    m_Rigid4->SetGravity(true);

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
        m_Rigid1->SetGround(false);
        m_Rigid1->AddForce(100, 100.f);
        m_Rigid1->SetVelocityY(230.f);

        m_Rigid2->SetGround(false);
        m_Rigid2->AddForce(110, 150.f);
        m_Rigid2->SetVelocityY(240.f);

        m_Rigid3->SetGround(false);
        m_Rigid3->AddForce(120, 80.f);
        m_Rigid3->SetVelocityY(260.f);

        m_Rigid4->SetGround(false);
        m_Rigid4->AddForce(130, 150.f);
        m_Rigid4->SetVelocityY(250.f);
    }

    float PosY = GetWorldPos().y;

    if (GetWorldPos().y >= 480.f)
    {
        m_DefyingGravity = false;

        m_Rigid1->SetVelocity(100.f, -20.f);
        m_Rigid1->AddForce(300, 30.f, 0.f);

        m_Rigid2->SetVelocity(70.f, -100.f);
        m_Rigid2->AddForce(100, 10.f, 0.f);

        m_Rigid3->SetVelocity(300.f, -20.f);
        m_Rigid3->AddForce(200, 10.f, 0.f);

        m_Rigid4->SetVelocity(250.f, -20.f);
        m_Rigid4->AddForce(300, 10.f, 0.f);

        //m_HammerDebrisMesh1->AddWorldPosition(100.f, 0.f, 0.f);
        //m_HammerDebrisMesh2->AddRelativePosition(-50.f, 0.f, 19.f);
        //m_HammerDebrisMesh3->AddRelativePosition(-40.f, 0.f, 5.f);
        //m_HammerDebrisMesh4->AddRelativePosition(-35.f, 0.f, 0.f);
    }

    // ¶¥¿¡ ÂøÁö
    if (Y != FLT_MAX && GetWorldPos().y - Y < m_HammerDebrisMesh1->GetMeshSize().y / 2.f && m_HammerDebrisMesh1)
    {
        SetWorldPositionY(Y + m_HammerDebrisMesh1->GetMeshSize().y / 2.f);

        m_Rigid1->SetGround(true);
        m_Rigid1->SetVelocityY(0.f);
        m_Rigid1->AddForce(0, 0.f);

        m_Rigid2->SetGround(true);
        m_Rigid2->SetVelocityY(0.f);
        m_Rigid2->AddForce(0, 0.f);

        m_Rigid3->SetGround(true);
        m_Rigid3->SetVelocityY(0.f);
        m_Rigid3->AddForce(0, 0.f);

        m_Rigid4->SetGround(true);
        m_Rigid4->SetVelocityY(0.f);
        m_Rigid4->AddForce(0, 0.f);
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


