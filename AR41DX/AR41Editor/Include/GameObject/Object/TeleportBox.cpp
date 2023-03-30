#include "TeleportBox.h"
#include "../Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "TeleportBox.h"


CTeleportBox::CTeleportBox() :
    m_BoxIndex(1)
{
    SetTypeID<CTeleportBox>();

    m_ObjectTypeName = "TeleportBox";
}

CTeleportBox::CTeleportBox(const CTeleportBox& Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");/*
    m_BoxWallCube1 = (CColliderOBB3D*)FindComponent("BoxWallCube1");
    m_BoxWallCube2 = (CColliderOBB3D*)FindComponent("BoxWallCube2");
    m_BoxWallCube3 = (CColliderOBB3D*)FindComponent("BoxWallCube3");
    m_BoxWallCube4 = (CColliderOBB3D*)FindComponent("BoxWallCube4");
    m_BoxBottomCube = (CColliderOBB3D*)FindComponent("BoxBottomCube");*/
    m_DetectRange = (CColliderOBB3D*)FindComponent("DetectRange");
}

CTeleportBox::~CTeleportBox()
{
}

void CTeleportBox::Start()
{
    CGameObject::Start();

    m_DetectRange->SetCollisionCallback<CTeleportBox>(ECollision_Result::Collision, this, &CTeleportBox::Collision_OpenBox);

    m_Box1BottomCube->SetCollisionCallback<CTeleportBox>(ECollision_Result::Collision, this, &CTeleportBox::Collision_Teleport);
    m_Box2BottomCube->SetCollisionCallback<CTeleportBox>(ECollision_Result::Collision, this, &CTeleportBox::Collision_Teleport);


    // 아래 위치 변경시, 충돌 함수 내에서의 텔레포트 위치 변경도 해야 한다. 
    //if (m_BoxIndex == 1)
    //    m_Mesh->SetWorldPosition(18000.f, 0.f, 13500.f);

    //if (m_BoxIndex == 2)
    //    m_Mesh->SetWorldPosition(16500.f, 0.f, 13500.f);
}

bool CTeleportBox::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");/*
    m_BoxWallCube1 = CreateComponent<CColliderOBB3D>("BoxWallCube1");
    m_BoxWallCube2 = CreateComponent<CColliderOBB3D>("BoxWallCube2");
    m_BoxWallCube3 = CreateComponent<CColliderOBB3D>("BoxWallCube3");
    m_BoxWallCube4 = CreateComponent<CColliderOBB3D>("BoxWallCube4");*/
    m_Box1BottomCube = CreateComponent<CColliderOBB3D>("Box1BottomCube");
    m_Box2BottomCube = CreateComponent<CColliderOBB3D>("Box2BottomCube");
    m_DetectRange = CreateComponent<CColliderOBB3D>("DetectRange");

    m_Mesh->SetMesh("TeleportBox");

    //m_Mesh->AddChild(m_BoxWallCube1);
    //m_Mesh->AddChild(m_BoxWallCube2);
    //m_Mesh->AddChild(m_BoxWallCube3);
    //m_Mesh->AddChild(m_BoxWallCube4);
    if (m_BoxIndex == 1)
        m_Mesh->AddChild(m_Box1BottomCube);

    if (m_BoxIndex == 2)
        m_Mesh->AddChild(m_Box2BottomCube);

    m_Mesh->AddChild(m_DetectRange);

    SetRootComponent(m_Mesh);

    //// Back
    //m_BoxWallCube1->SetCollisionProfile("Wall");
    //m_BoxWallCube1->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube1->AddRelativeRotationY(90.f);
    //m_BoxWallCube1->SetRelativePosition(0.f, 0.f, -30.f);

    //// Front
    //m_BoxWallCube2->SetCollisionProfile("Wall");
    //m_BoxWallCube2->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube2->AddRelativeRotationY(90.f);
    //m_BoxWallCube2->SetRelativePosition(0.f, 0.f, 30.f);

    //// Left
    //m_BoxWallCube3->SetCollisionProfile("Wall");
    //m_BoxWallCube3->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube3->AddRelativeRotationX(90.f);
    //m_BoxWallCube3->SetRelativePosition(0.f, 0.f, -30.f);

    //// Right
    //m_BoxWallCube4->SetCollisionProfile("Wall");
    //m_BoxWallCube4->SetCubeSize(30.f, 30.f, 10.f);
    //m_BoxWallCube4->AddRelativeRotationX(90.f);
    //m_BoxWallCube4->SetRelativePosition(0.f, 0.f, -30.f);

    // Bottom
    m_Box1BottomCube->SetCollisionProfile("Wall");
    m_Box1BottomCube->SetBoxHalfSize(100.f, 30.f, 100.f);

    m_Box2BottomCube->SetCollisionProfile("Wall");
    m_Box2BottomCube->SetBoxHalfSize(100.f, 30.f, 100.f);

    // DetectRange
    m_DetectRange->SetCollisionProfile("Wall");
    m_DetectRange->SetBoxHalfSize(700.f, 10.f, 700.f);
    m_DetectRange->AddOffsetY(50.f);

    m_Animation = m_Mesh->SetAnimation<CAnimation>("TeleportBoxMesh");

    m_Animation->AddAnimation("TeleportBox_Closed", "TeleportBox_Closed", 1.f, 1.f, false);
    m_Animation->AddAnimation("TeleportBox_Opening", "TeleportBox_Opening", 1.f, 1.f, false);
    m_Animation->AddAnimation("TeleportBox_OpeningLoop", "TeleportBox_OpeningLoop", 1.f, 1.f, false);

    //// TeleportBox->SetWorldPosition(18000.f, 0.f, 13500.f);
    //if (m_BoxIndex == 1)
    //    m_Mesh->SetWorldPosition(18000.f, 0.f, 13500.f);

    //else
    //    m_Mesh->SetWorldPosition(16500.f, 0.f, 13500.f);

    return true;
}

void CTeleportBox::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CTeleportBox::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CTeleportBox* CTeleportBox::Clone() const
{
    return new CTeleportBox(*this);
}

void CTeleportBox::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CTeleportBox::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CTeleportBox::Collision_OpenBox(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        m_Animation->ChangeAnimation("TeleportBox_Opening");
    }
}

void CTeleportBox::Collision_Teleport(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        CPlayer* Player = (CPlayer*)CSceneManager::GetInst()->GetScene()->GetPlayerObject();

        if (GetBoxIndex() == 1)
        {
            // 300.f는 박스 옆으로 이동시키기위한 임의의 숫자. 
            //Player->SetWorldPosition(16500.f + 300.f, 0.f, 13500.f);
        }

        if (GetBoxIndex() == 2) {
            //Player->SetWorldPosition(18000.f + 300.f, 0.f, 13500.f);

        }
    }
}
