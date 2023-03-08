#include "InteractButton.h"

#include "Scene/Scene.h"
#include "Component/AnimationMeshComponent.h"

CInteractButton::CInteractButton()
{
}

CInteractButton::CInteractButton(const CInteractButton& Obj) :
    CGameObject(Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("InteractButtonAnimation");
    m_Cube = (CColliderCube*)FindComponent("Cube");
}

CInteractButton::~CInteractButton()
{
}

void CInteractButton::Start()
{
    CGameObject::Start();
}

bool CInteractButton::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
    m_Cube = CreateComponent<CColliderCube>("Cube");

    SetRootComponent(m_Mesh);

    //m_Mesh->SetMesh("InteractButton");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("InteractButtonAnimation");

    return true;
}

void CInteractButton::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CInteractButton::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CInteractButton* CInteractButton::Clone() const
{
    return new CInteractButton(*this);
}

void CInteractButton::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CInteractButton::Load(FILE* File)
{
    CGameObject::Load(File);
}
