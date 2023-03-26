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
#include "../../Scene/LoadingSceneInfo.h"
#include "Scene/NavigationManager3D.h"

CFodderDebris::CFodderDebris()  :
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

    //m_Cube = (CColliderOBB3D*)FindComponent("Cube");
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

    //m_Cube = CreateComponent<CColliderOBB3D>("Cube");

    m_Rigid = CreateComponent<CRigidBody>("Rigid");

    SetRootComponent(m_FodderDebrisMesh1);

    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh2);
    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh3);
    m_FodderDebrisMesh1->AddChild(m_FodderDebrisMesh4);
    m_FodderDebrisMesh1->AddChild(m_Rigid);

    m_FodderDebrisMesh2->AddChild(m_Rigid);
    m_FodderDebrisMesh3->AddChild(m_Rigid);
    m_FodderDebrisMesh4->AddChild(m_Rigid);

    m_FodderDebrisMesh1->SetMesh("FodderDebris1");
    m_FodderDebrisMesh2->SetMesh("FodderDebris2");
    m_FodderDebrisMesh3->SetMesh("FodderDebris3");
    m_FodderDebrisMesh4->SetMesh("FodderDebris4");

    //m_FodderDebris1->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);

    m_Rigid->SetGravity(true);



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


    //m_Rigid->SetVelocity(0.f, 0.f, 0.f);
    //m_Rigid->SetAccel(0.f, 0.f, 0.f);


    return true;
}

void CFodderDebris::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
    //CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
    //const float timeStep = 1.0f / 60.0f; // Time step for the simulation
    //const float popDuration = 0.5f; // Duration of the popping animation
    //const float popHeight = 5.0f; // Height that the monsters reach when popped
    //float timeSinceLastPop = 0.0f;
    //{
    //    // Update the position based on the velocity and time step
    //    if (m_IsPopped)
    //    {
    //        // If the monster is popped, apply a parabolic animation for the first popDuration seconds
    //        timeSinceLastPop += timeStep;

    //        if (timeSinceLastPop <= popDuration) {
    //            float t = timeSinceLastPop / popDuration;
    //            SetWorldPositionX(popHeight * t * (2.0f - t));
    //        }
    //    }

    //    if (m_Rigid->GetVelocity().y < 0.f)
    //    {
    //        // If the monster hasn't popped yet, check if it should pop
    //        if (GetWorldPos().y <= 0.0f)
    //        {
    //            // If the monster has hit the ground, set its velocity upwards to simulate popping
    //            m_Velocity[1] = sqrtf(2.0f * popHeight * fabsf(-9.81f));
    //            m_IsPopped = true;
    //            timeSinceLastPop = 0.0f;
    //        }
    //    }

    //    m_Velocity[0] += m_Acceleration[0] * timeStep;
    //    m_Velocity[1] += m_Acceleration[1] * timeStep;
    //    m_Velocity[2] += m_Acceleration[2] * timeStep;

    //    m_FodderDebrisMesh1->AddWorldPositionX(m_Velocity[0] * timeStep);
    //    m_FodderDebrisMesh1->AddWorldPositionY(m_Velocity[1] * timeStep);
    //    m_FodderDebrisMesh1->AddWorldPositionZ(m_Velocity[2] * timeStep);
    //}

    CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();

    //if (m_Rigid->GetGround()) //Ground가 아예 안 들어온다. 
    //{
    //    m_Rigid->SetGround(false);
    //    m_Rigid->AddForce(0, 500.f, 0.f);//
    //    m_Rigid->SetVelocityY(500.f);//
    //    m_Rigid->AddForce(0, 0.f, 0.f);
    //    m_Rigid->SetVelocityY(0.f);
    //}

    //m_Rigid->SetGravityForce(70.f);
    //m_Rigid->AddForce(0, -100.f, 0.f);
   // m_Rigid->SetVelocityY(-350.f); // 이걸 음수로 줘야 떨어짐

    /*if (m_Rigid->GetVelocity().y < 0.f)
    {*/
        //AddWorldPositionX(100.f * DeltaTime);
        //float Y = Nav->GetHeight(GetWorldPos());

        //if (Y != FLT_MAX && GetWorldPos().y - Y < m_FodderDebrisMesh1->GetMeshSize().y / 2.f && m_FodderDebrisMesh1)
        //{
           /* SetWorldPositionY(Y + m_FodderDebrisMesh1->GetMeshSize().y / 2.f);
            m_Rigid->SetGround(true);*/
        //}
    //}



    //
            float Y = Nav->GetHeight(GetWorldPos());

            m_Rigid->SetGround(false);
            m_Rigid->AddForce(0, 200.f);
            m_Rigid->SetVelocityY(200.f);
            
            float PosY = GetWorldPos().y;
            if (GetWorldPos().y <= 1200.f)
            {
                m_Rigid->AddForce(0, 0.f);
                m_Rigid->SetVelocityY(0.f);

                if (Y == GetWorldPos().y)
                    m_Rigid->SetGround(true);
            }
            //if (Y != FLT_MAX && GetWorldPos().y - Y < m_FodderDebrisMesh1->GetMeshSize().y / 2.f && m_FodderDebrisMesh1)
            //{
            //    SetWorldPositionY(Y + m_FodderDebrisMesh1->GetMeshSize().y / 2.f);
            //    m_Rigid->SetGround(true);
            //}
                //if(m_Rigid->GetVelocity().y < 0.f)
                //{
               // SetWorldPositionY(Y);
                //m_Rigid->SetGround(true);
               // }
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


