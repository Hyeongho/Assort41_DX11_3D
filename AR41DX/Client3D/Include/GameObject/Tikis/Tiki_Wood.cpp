#include "Tiki_Wood.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Input.h"
#include "Scene/Scene.h"

CTiki_Wood::CTiki_Wood()
{
}

CTiki_Wood::CTiki_Wood(const CTiki_Wood& Obj)
    : CGameObject(Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("TikiWoodsAnimation");
    m_Cube = (CColliderCube*)FindComponent("Cube");
    m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CTiki_Wood::~CTiki_Wood()
{
}

void CTiki_Wood::Start()
{
    CGameObject::Start();

#ifdef DEBUG
    CInput::GetInst()->AddBindFunction<CTiki_Wood>("F1", Input_Type::Up, this, &CTiki_Wood::ChangeAnim_Idle, m_Scene);
    CInput::GetInst()->AddBindFunction<CTiki_Wood>("F2", Input_Type::Up, this, &CTiki_Wood::ChangeAnim_Die, m_Scene);
#endif // DEBUG
}

bool CTiki_Wood::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
    m_Rigid = CreateComponent<CRigidBody>("Rigid");
    m_Cube = CreateComponent<CColliderCube>("Cube");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Tiki_Woods");

    m_Mesh->AddChild(m_Rigid);
    m_Mesh->AddChild(m_Cube);


    m_Cube->SetCubeSize(m_Mesh->GetMeshSize());
    m_Cube->SetCollisionCallback<CTiki_Wood>(ECollision_Result::Collision, this, &CTiki_Wood::CollisionTest);


    m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiWoodsAnimation");
    m_Animation->AddAnimation("Tiki_Woods_Idle", "Tiki_Woods_Idle", 1.f, 1.f, true);
    m_Animation->AddAnimation("Tiki_Woods_Die", "Tiki_Woods_Die", 1.f, 1.f, false);

    m_Animation->SetCurrentEndFunction<CTiki_Wood>("Tiki_Woods_Die", this, &CTiki_Wood::Tiki_Die);

    m_Animation->SetCurrentAnimation("Tiki_Woods_Idle");

    return true;
}

void CTiki_Wood::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CTiki_Wood::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CTiki_Wood* CTiki_Wood::Clone() const
{
    return new CTiki_Wood(*this);
}

void CTiki_Wood::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CTiki_Wood::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CTiki_Wood::ChangeAnim_Idle()
{
    m_Animation->ChangeAnimation("Tiki_Woods_Idle");
}

void CTiki_Wood::ChangeAnim_Die()
{
    m_Animation->ChangeAnimation("Tiki_Woods_Die");
}

void CTiki_Wood::Tiki_Die()
{
    Destroy();
}

void CTiki_Wood::CollisionTest(const CollisionResult& result)
{
}
