#include "Tiki_Thunder.h"

#include "Component/AnimationMeshComponent.h"
#include "Input.h"

CTiki_Thunder::CTiki_Thunder()
{
}

CTiki_Thunder::CTiki_Thunder(const CTiki_Thunder& Obj)
    : CGameObject(Obj)
{
}

CTiki_Thunder::~CTiki_Thunder()
{
}

void CTiki_Thunder::Start()
{
    CGameObject::Start();


    CInput::GetInst()->AddBindFunction<CTiki_Thunder>("F1", Input_Type::Up, this, &CTiki_Thunder::ChangeAnim_Idle, m_Scene);
    CInput::GetInst()->AddBindFunction<CTiki_Thunder>("F2", Input_Type::Up, this, &CTiki_Thunder::ChangeAnim_Die, m_Scene);
}

bool CTiki_Thunder::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Tiki_Thunder");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiThunderAnimation");

    m_Animation->AddAnimation("Tiki_Thunder_Idle", "Tiki_Thunder_Idle", 1.f, 1.f, true);
    m_Animation->AddAnimation("Tiki_Thunder_Die", "Tiki_Thunder_Die", 1.f, 1.f, false);

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
}
