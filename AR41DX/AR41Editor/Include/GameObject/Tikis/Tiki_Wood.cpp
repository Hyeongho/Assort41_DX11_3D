#include "Tiki_Wood.h"

#include "Component/AnimationMeshComponent.h"
#include "Input.h"

CTiki_Wood::CTiki_Wood()
{
}

CTiki_Wood::CTiki_Wood(const CTiki_Wood& Obj)
    : CGameObject(Obj)
{
}

CTiki_Wood::~CTiki_Wood()
{
}

void CTiki_Wood::Start()
{
    CGameObject::Start();


    CInput::GetInst()->AddBindFunction<CTiki_Wood>("F1", Input_Type::Up, this, &CTiki_Wood::ChangeAnim_Idle, m_Scene);
    CInput::GetInst()->AddBindFunction<CTiki_Wood>("F2", Input_Type::Up, this, &CTiki_Wood::ChangeAnim_Die, m_Scene);
}

bool CTiki_Wood::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Tiki_Woods");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiWoodsAnimation");

    m_Animation->AddAnimation("Tiki_Woods_Idle", "Tiki_Woods_Idle", 1.f, 1.f, true);
    m_Animation->AddAnimation("Tiki_Woods_Die", "Tiki_Woods_Die", 1.f, 1.f, false);
    
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
}
