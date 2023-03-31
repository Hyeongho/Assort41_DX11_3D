#include "FodderDebris.h"
#include "Player.h"
#include "Fodder.h"
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

CFodderDebris::CFodderDebris()  :
    m_DefyingGravity(true),
    m_IsPopped(false),
    m_Mass(1.f)
{
    SetTypeID<CFodderDebris>();

    m_ObjectTypeName = "FodderDebris";
}

CFodderDebris::CFodderDebris(const CFodderDebris& Obj)
{
    m_FodderDebrisMesh1 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh1");
    m_FodderDebrisMesh2 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh2");
    m_FodderDebrisMesh3 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh3");
    m_FodderDebrisMesh4 = (CStaticMeshComponent*)FindComponent("FodderDebrisMesh4");

    m_Mesh1Cube = (CColliderOBB3D*)FindComponent("Mesh1Cube");
    m_Mesh2Cube = (CColliderOBB3D*)FindComponent("Mesh2Cube");
    m_Mesh3Cube = (CColliderOBB3D*)FindComponent("Mesh3Cube");
    m_Mesh4Cube = (CColliderOBB3D*)FindComponent("Mesh4Cube");
}

CFodderDebris::~CFodderDebris()
{
}

void CFodderDebris::Start()
{
    CGameObject::Start();
}

bool CFodderDebris::Init()
{
    CGameObject::Init();

    m_FodderDebrisMesh1 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh1");
    m_FodderDebrisMesh2 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh2");
    m_FodderDebrisMesh3 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh3");
    m_FodderDebrisMesh4 = CreateComponent<CStaticMeshComponent>("FodderDebrisMesh4");

    m_Mesh1Cube = CreateComponent<CColliderOBB3D>("Mesh1Cube");
    m_Mesh2Cube = CreateComponent<CColliderOBB3D>("Mesh2Cube");
    m_Mesh3Cube = CreateComponent<CColliderOBB3D>("Mesh3Cube");
    m_Mesh4Cube = CreateComponent<CColliderOBB3D>("Mesh4Cube");

    m_Rigid = CreateComponent<CRigidBody>("Rigid");

    SetRootComponent(m_FodderDebrisMesh1);

    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh2);
    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh3);
    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh4);
    m_FodderDebrisMesh1->AddChild(m_Rigid);
    m_FodderDebrisMesh1->AddChild(m_Mesh1Cube);

    m_FodderDebrisMesh2->AddChild(m_Rigid);
    m_FodderDebrisMesh2->AddChild(m_Mesh2Cube);

    m_FodderDebrisMesh3->AddChild(m_Rigid);
    m_FodderDebrisMesh3->AddChild(m_Mesh3Cube);

    m_FodderDebrisMesh4->AddChild(m_Rigid);
    m_FodderDebrisMesh4->AddChild(m_Mesh4Cube);

    m_FodderDebrisMesh1->SetMesh("FodderDebris1");
    m_FodderDebrisMesh2->SetMesh("FodderDebris2");
    m_FodderDebrisMesh3->SetMesh("FodderDebris3");
    m_FodderDebrisMesh4->SetMesh("FodderDebris4");

    //m_FodderDebris1->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);

    //
    m_Velocity[0] = 0.f;
    m_Velocity[1] = 0.f;
    m_Velocity[2] = 0.f;

    m_Acceleration[0] = 0.f;
    m_Acceleration[1] = -9.81f;
    m_Acceleration[2] = 0.f;

    m_Fodder = (CFodder*)m_Scene->FindObject("Fodder");

    m_FodderDebrisMesh1->SetWorldPositionX(m_Fodder->GetWorldPos().x * 2.f); 
    m_FodderDebrisMesh1->SetWorldPositionY(m_Fodder->GetWorldPos().y + 10.f);
    m_FodderDebrisMesh1->SetWorldPositionZ(m_Fodder->GetWorldPos().z);

    m_FodderDebrisMesh2->SetRelativePosition(10.f, 10.f, 20.f); 
    m_FodderDebrisMesh2->SetRelativeRotation(10.f, 20.f, 20.f);

    m_FodderDebrisMesh3->SetRelativePosition(20.f, 10.f, 20.f); 
    m_FodderDebrisMesh3->SetRelativeRotation(15.f, 20.f, 34.f);

    m_FodderDebrisMesh4->SetRelativePosition(30.f, 10.f, 20.f); 
    m_FodderDebrisMesh4->SetRelativeRotation(24.f, 20.f, 10.f);


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

    //m_Rigid->SetVelocity(0.f, 0.f, 0.f);
    //m_Rigid->SetAccel(0.f, 0.f, 0.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CFodderDebris::Update(float DeltaTime)
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
    
    if (GetWorldPos().y >= 700.f)
    {
        m_DefyingGravity = false;
        m_Rigid->SetVelocityY(-200.f);
        m_Rigid->AddForce(0, 70.f);
    }

    // ¶¥¿¡ ÂøÁö
    if (Y != FLT_MAX && GetWorldPos().y - Y < m_FodderDebrisMesh1->GetMeshSize().y / 2.f && m_FodderDebrisMesh1)
    {
        SetWorldPositionY(Y + m_FodderDebrisMesh1->GetMeshSize().y / 2.f);
        m_Rigid->SetGround(true);
        m_Rigid->SetVelocityY(0.f);
        m_Rigid->AddForce(0, 0.f);
    }
}

void CFodderDebris::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CFodderDebris* CFodderDebris::Clone() const
{
    return new CFodderDebris(*this);
}

void CFodderDebris::Save(FILE* File)
{
}

void CFodderDebris::Load(FILE* File)
{
}

void CFodderDebris::Destroy()
{
}


