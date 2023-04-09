#include "Collectible_Bubble.h"

#include "Device.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Component/StaticMeshComponent.h"

CCollectible_Bubble::CCollectible_Bubble()
{
}

CCollectible_Bubble::CCollectible_Bubble(const CCollectible_Bubble& Obj)
{
}

CCollectible_Bubble::~CCollectible_Bubble()
{
}

void CCollectible_Bubble::Start()
{
    CGameObject::Start();

#ifdef _DEBUG
    CInput::GetInst()->AddBindFunction<CCollectible_Bubble>("F1", Input_Type::Up, this, &CCollectible_Bubble::DebugF1, m_Scene);
    CInput::GetInst()->AddBindFunction<CCollectible_Bubble>("F2", Input_Type::Up, this, &CCollectible_Bubble::DebugF2, m_Scene);
    CInput::GetInst()->AddBindFunction<CCollectible_Bubble>("F3", Input_Type::Up, this, &CCollectible_Bubble::DebugF3, m_Scene);
#endif // _DEBUG

}
 
bool CCollectible_Bubble::Init()
{
    CGameObject::Init();

    m_BubbleMesh = CreateComponent<CStaticMeshComponent>("Mesh");
    m_BubbleMesh->SetMesh("Collectible_Bubble");
    //m_BubbleMesh->SetMeshSize(200.f, 200.f, 200.f);

    SetRootComponent(m_BubbleMesh);

    m_BubbleMesh->GetMaterial(0)->SetOpacity(0.5f);
    m_BubbleMesh->GetMaterial(0)->SetRenderState("AlphaBlend");

    SetWorldScale(1.5f, 1.5f, 1.5f);


    return true;
}

void CCollectible_Bubble::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CCollectible_Bubble::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CCollectible_Bubble* CCollectible_Bubble::Clone() const
{
	return nullptr;
}

void CCollectible_Bubble::Save(FILE* File)
{
}

void CCollectible_Bubble::Load(FILE* File)
{
}

void CCollectible_Bubble::DebugF1()
{
    //m_BubbleMesh->GetMaterial(0)->SetOpacity(0.4f);
}

void CCollectible_Bubble::DebugF2()
{
    //m_BubbleMesh->GetMaterial(0)->SetOpacity(0.5f);
}

void CCollectible_Bubble::DebugF3()
{
    //m_BubbleMesh->GetMaterial(0)->SetOpacity(0.8f);
}
