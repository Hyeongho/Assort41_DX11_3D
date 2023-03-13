#include "InteractButton.h"

#include "Scene/Scene.h"
#include "Component/ColliderCube.h"
#include "Component/StaticMeshComponent.h"

CInteractButton::CInteractButton()
{
    SetTypeID<CInteractButton>();
    m_ObjectTypeName = "InteractButton";
}

CInteractButton::CInteractButton(const CInteractButton& Obj) :
    CGameObject(Obj)
{
    m_MeshButton = (CStaticMeshComponent*)FindComponent("Mesh");
    m_MeshButtonPlate = (CStaticMeshComponent*)FindComponent("Mesh");
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

    m_MeshButton = CreateComponent<CStaticMeshComponent>("Mesh");
    m_MeshButtonPlate = CreateComponent<CStaticMeshComponent>("Mesh");
    m_Cube = CreateComponent<CColliderCube>("Cube");

    SetRootComponent(m_MeshButtonPlate);

    m_MeshButton->SetMesh("Button");
    m_MeshButtonPlate->SetMesh("ButtonPlate");

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

