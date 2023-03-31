#include "CheckPoint.h"
#include "../Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CCheckPoint::CCheckPoint()
{
    SetTypeID<CCheckPoint>();

    m_ObjectTypeName = "CheckPoint";
}

CCheckPoint::CCheckPoint(const CCheckPoint& Obj)
    : CGameObject(Obj)
{
    m_FlagMesh = (CAnimationMeshComponent*)FindComponent("FlagMesh");
    //m_BottomMesh = (CStaticMeshComponent*)FindComponent("BottomMesh");
    m_CheckPointCube = (CColliderOBB3D*)FindComponent("CheckPointCube");
    m_DetectRange = (CColliderOBB3D*)FindComponent("DetectRange");
    m_Animation = Obj.m_Animation;
}

CCheckPoint::~CCheckPoint()
{
}

void CCheckPoint::Start()
{
    CGameObject::Start();

    m_DetectRange->SetCollisionCallback<CCheckPoint>(ECollision_Result::Collision, this, &CCheckPoint::Collision_CheckPoint_Active);
    m_DetectRange->SetCollisionCallback<CCheckPoint>(ECollision_Result::Collision, this, &CCheckPoint::Release_CheckPoint_Inactive);

    m_Animation->SetCurrentEndFunction("CheckPointPopUp", this, &CCheckPoint::CheckPointIdleAnim);
}

bool CCheckPoint::Init()
{
    CGameObject::Init();

    m_FlagMesh = CreateComponent<CAnimationMeshComponent>("FlagMesh");
    //m_BottomMesh = CreateComponent<CStaticMeshComponent>("BottomMesh");
    m_CheckPointCube = CreateComponent<CColliderOBB3D>("CheckPointCube");
    m_DetectRange = CreateComponent<CColliderOBB3D>("DetectRange");

    m_FlagMesh->SetMesh("CheckPointMesh");
    //m_BottomMesh->SetMesh("CheckPointMesh");

    //m_BottomMesh->SetMesh("JumpTreeTop"); // ¼öÁ¤

    //SetRootComponent(m_BottomMesh);
    SetRootComponent(m_FlagMesh);

    m_FlagMesh->AddChild(m_CheckPointCube);
    m_FlagMesh->AddChild(m_DetectRange);

    //m_BottomMesh->AddChild(m_CheckPointCube);
    //m_BottomMesh->AddChild(m_DetectRange);

    m_CheckPointCube->SetCollisionProfile("Wall");
    m_CheckPointCube->SetBoxHalfSize(100.f, 300.f, 100.f);
    m_CheckPointCube->SetRelativePosition(0.f, 300.f, 30.f);

    m_DetectRange->SetCollisionProfile("Wall");
    m_DetectRange->SetBoxHalfSize(600.f, 300.f, 400.f);
    m_DetectRange->SetRelativePosition(0.f, 300.f, -100.f);

    m_Animation = m_FlagMesh->SetAnimation<CAnimation>("CheckPoint"); 

    m_Animation->AddAnimation("CheckPointInactive", "CheckPointInactive", 1.f, 1.f, false); 
    m_Animation->AddAnimation("CheckPointIdle", "CheckPointIdle", 1.f, 1.f, true); 
    m_Animation->AddAnimation("CheckPointPopUp", "CheckPointPopUp", 1.f, 1.f, false); 

    return true;
}

void CCheckPoint::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CCheckPoint::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CCheckPoint* CCheckPoint::Clone() const
{
    return new CCheckPoint(*this);
}

void CCheckPoint::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CCheckPoint::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CCheckPoint::CheckPointIdleAnim()
{
    m_Animation->ChangeAnimation("CheckPointIdle");
}

void CCheckPoint::Collision_CheckPoint_Active(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        m_Animation->ChangeAnimation("CheckPointPopUp"); 
    }

}

void CCheckPoint::Release_CheckPoint_Inactive(const CollisionResult& result)
{
    if (result.Dest->GetCollisionProfile()->Channel->Channel == ECollision_Channel::Player)
    {
        CheckPointIdleAnim();
    }
}
