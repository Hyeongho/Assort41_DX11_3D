#include "Collectible_Bubble.h"

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
}

bool CCollectible_Bubble::Init()
{
    CGameObject::Init();

    m_BubbleMesh = CreateComponent<CStaticMeshComponent>("Mesh");
    m_BubbleMesh->SetMesh("Collectible_Bubble");

    SetRootComponent(m_BubbleMesh);

    SetRelativeScale(20.f, 20.f, 20.f);

    CMaterial* Material = m_BubbleMesh->GetMaterial(0);
    Material->SetTexture(0, 0, 2, "test", TEXT("T_Oil_Slick.tga"), TEXTURE_PATH);
    Material->SetOpacity(0.4f);
    Material->SetRenderState("AlphaBlend");
    m_BubbleMesh->SetMaterial(0, Material);

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
