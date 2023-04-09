#include "Gate.h"
#include "../Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Component/RigidBody.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "../../Scene/LoadingSceneInfo.h"

CGate::CGate() :
    m_OpenSesameOn(false)
{
    SetTypeID<CGate>();

    m_ObjectTypeName = "Gate";
}

CGate::CGate(const CGate& Obj)
{
    m_ArmMesh = (CStaticMeshComponent*)FindComponent("ArmMesh");
    m_BottomMesh = (CStaticMeshComponent*)FindComponent("BottomMesh");
    m_Cube = (CColliderOBB3D*)FindComponent("Cube");
    m_DetectRange = (CColliderOBB3D*)FindComponent("DetectRange");
}

CGate::~CGate()
{
}

void CGate::Start()
{
    CGameObject::Start();

    m_DetectRange->SetCollisionCallback<CGate>(ECollision_Result::Collision, this, &CGate::Collision_OpenSesame);
    m_DetectRange->SetCollisionCallback<CGate>(ECollision_Result::Release, this, &CGate::Release_OpenSesameOff);
}

bool CGate::Init()
{
    CGameObject::Init();

    m_ArmMesh = CreateComponent<CStaticMeshComponent>("ArmMesh");
    m_BottomMesh = CreateComponent<CStaticMeshComponent>("BottomMesh");
    m_Cube = CreateComponent<CColliderOBB3D>("Cube");
    m_DetectRange = CreateComponent<CColliderOBB3D>("DetectRange");

    m_BottomMesh->SetMesh("GateBottom");
    m_ArmMesh->SetMesh("GateArm");

    SetRootComponent(m_BottomMesh);

    m_BottomMesh->AddChild(m_ArmMesh);
    m_BottomMesh->AddChild(m_DetectRange);
    m_BottomMesh->AddChild(m_Cube);

    m_ArmMesh->SetRelativePositionY(0.f);
    //m_ArmMesh->SetPivot(70.f,-50.f,0.f);

    m_Cube->AddChild(m_DetectRange);

    //m_BottomMesh->SetWorldPosition(130.f, 50.f, 150.f);
    //m_ArmMesh->SetRelativePosition(0.f, 50.f, 0.f);

    m_Cube->SetCollisionProfile("Wall");
    m_Cube->SetBoxHalfSize(230.f, 80.f, 30.f);
    m_Cube->SetRelativePosition(-170.f, 80.f, 0.f);

    m_DetectRange->SetCollisionProfile("Wall");
    m_DetectRange->SetBoxHalfSize(450.f, 30.f, 450.f);
    m_DetectRange->SetRelativePosition(-50.f, -30.f, 150.f);

    return true;
}

void CGate::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (m_OpenSesameOn)
        m_ArmMesh->AddWorldRotationZ(-1.f);


    if (m_ArmMesh->GetWorldRot().z == -30.f && m_OpenSesameOn)
    {
        m_OpenSesameOn = false;

        m_Scene->GetResource()->SoundStop("JellyfishField");

        ChangeScene();
    }
}

void CGate::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CGate* CGate::Clone() const
{
    return new CGate(*this);
}

void CGate::Save(FILE* File)
{
}

void CGate::Load(FILE* File)
{
}

void CGate::ChangeScene()
{
    // 로딩 Scene을 생성한다.
    CSceneManager::GetInst()->CreateNextScene(true);

    CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false);
}

void CGate::Collision_OpenSesame(const CollisionResult& result)
{
    m_OpenSesameOn = true;

    CResourceManager::GetInst()->SoundPlay("Gate_Opening");
    CResourceManager::GetInst()->SetVolume(20.f);
}

void CGate::Release_OpenSesameOff(const CollisionResult& result)
{
    m_OpenSesameOn = false;
}
