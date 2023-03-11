#include "JumpTree.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderCube.h"
#include "Scene/Scene.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "JumpTree.h"


CJumpTree::CJumpTree()
{
    SetTypeID<CJumpTree>();

    m_ObjectTypeName = "JumpTree";
}

CJumpTree::CJumpTree(const CJumpTree& Obj)
{
    m_TopMesh = (CAnimationMeshComponent*)FindComponent("TopMesh");
    m_BottomMesh = (CStaticMeshComponent*)FindComponent("BottomMesh");
    m_TopCube = (CColliderCube*)FindComponent("TopCube");    
    m_BottomCube = (CColliderCube*)FindComponent("BottomCube");

}

CJumpTree::~CJumpTree()
{
}

void CJumpTree::Start()
{
    CGameObject::Start();

    m_Animation->AddAnimation("JumpTreeTop_Bounce", "JumpTreeTop_Bounce", 1.f, 1.f, false);

    m_TopCube->SetCollisionCallback<CJumpTree>(ECollision_Result::Collision, this, &CJumpTree::Collision_Bounce);
    m_TopCube->SetCollisionCallback<CJumpTree>(ECollision_Result::Collision, this, &CJumpTree::Release_Idle);
}

bool CJumpTree::Init()
{
    CGameObject::Init();

    m_TopMesh = CreateComponent<CAnimationMeshComponent>("TopMesh");
    m_BottomMesh = CreateComponent<CStaticMeshComponent>("BottomMesh");
    m_TopCube = CreateComponent<CColliderCube>("TopCube");
    m_BottomCube = CreateComponent<CColliderCube>("BottomCube");

    m_TopMesh->SetMesh("JumpTreeTop");
    m_BottomMesh->SetMesh("JumpTreeBottom");

    m_BottomMesh->AddChild(m_TopMesh);
    m_BottomMesh->AddChild(m_BottomCube);

    m_TopMesh->SetRelativePositionY(700.f);

    //m_BottomCube->SetRelativePositionY(70.f);
    m_BottomCube->SetCollisionProfile("Object");
    m_BottomCube->SetCubeSize(500.f, 700.f, 500.f);

    m_TopMesh->AddChild(m_TopCube);

    //m_TopCube->SetRelativePositionY(70.f);
    m_TopCube->SetCollisionProfile("Object");
    m_TopCube->SetCubeSize(500.f, 200.f, 500.f);

    return true;
}

void CJumpTree::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CJumpTree::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CJumpTree* CJumpTree::Clone() const
{
    return new CJumpTree(*this);
}

void CJumpTree::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CJumpTree::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CJumpTree::Collision_Bounce(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        m_Animation->ChangeAnimation("JumpTreeTop_Bounce");
    }
}

void CJumpTree::Release_Idle(const CollisionResult& result)
{
}
