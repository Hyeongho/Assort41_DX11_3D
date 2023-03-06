#include "Tiki_Thunder.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Input.h"
#include "Scene/Scene.h"

CTiki_Thunder::CTiki_Thunder()
{
}

CTiki_Thunder::CTiki_Thunder(const CTiki_Thunder& Obj)
    : CGameObject(Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("TikiThunderAnimation");
    m_Cube = (CColliderCube*)FindComponent("Cube");
    m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CTiki_Thunder::~CTiki_Thunder()
{
}

void CTiki_Thunder::Start()
{
    CGameObject::Start();

#ifdef DEBUG
    CInput::GetInst()->AddBindFunction<CTiki_Thunder>("F1", Input_Type::Up, this, &CTiki_Thunder::ChangeAnim_Idle, m_Scene);
    CInput::GetInst()->AddBindFunction<CTiki_Thunder>("F2", Input_Type::Up, this, &CTiki_Thunder::ChangeAnim_Die, m_Scene);
#endif // DEBUG

}

bool CTiki_Thunder::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
    m_Rigid = CreateComponent<CRigidBody>("Rigid");
    m_Cube = CreateComponent<CColliderCube>("Cube");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Tiki_Thunder");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiThunderAnimation");

    m_Animation->AddAnimation("Tiki_Thunder_Idle", "Tiki_Thunder_Idle", 1.f, 1.f, true);
    m_Animation->AddAnimation("Tiki_Thunder_Die", "Tiki_Thunder_Die", 1.f, 1.f, false);

    m_Animation->SetCurrentEndFunction<CTiki_Thunder>("Tiki_Thunder_Die", this, &CTiki_Thunder::Tiki_Die);

    m_Animation->SetCurrentAnimation("Tiki_Thunder_Idle");

    return true;
}

void CTiki_Thunder::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CTiki_Thunder::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CTiki_Thunder* CTiki_Thunder::Clone() const
{
    return new CTiki_Thunder(*this);
}

void CTiki_Thunder::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CTiki_Thunder::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CTiki_Thunder::ChangeAnim_Idle()
{
    m_Animation->ChangeAnimation("Tiki_Thunder_Idle");
}

void CTiki_Thunder::ChangeAnim_Die()
{
    m_Animation->ChangeAnimation("Tiki_Thunder_Die");
}

void CTiki_Thunder::Tiki_Die()
{
    // 폭발 파티클

    
    // 피해처리
    m_Scene->FindObject("Player");


    // 데칼 처리


    // 본인 삭제.
    Destroy();
}
