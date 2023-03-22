#include "CollectibleItems.h"

#include "Scene/Scene.h"
#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CCollectibleItems::CCollectibleItems()
{
    SetTypeID<CCollectibleItems>();

    m_ObjectTypeName = "CollectibleItems";

    m_ColItemType = EColItemType::End;
}

CCollectibleItems::CCollectibleItems(const CCollectibleItems& Obj) :
    CGameObject(Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

    m_ColItemType = Obj.m_ColItemType;
}

CCollectibleItems::~CCollectibleItems()
{
}

void CCollectibleItems::Start()
{
    CGameObject::Start();
}

bool CCollectibleItems::Init()
{
    CGameObject::Init();

    //m_Bubble = CreateComponent<CStaticMeshComponent>("Mesh");
    //m_Bubble->SetMesh("Collectible_Bubble");
    //m_Bubble->SetWorldScale(20.f, 20.f, 20.f);
    //m_Bubble->SetRelativePositionY(m_Mesh->GetMeshSize().y / 2.f);

    //CMaterial* Material = m_Bubble->GetMaterial(0);
    //Material->SetTexture(0, 0, 2, "test", TEXT("T_Oil_Slick.tga"), TEXTURE_PATH);
    //Material->SetOpacity(0.35f);
    //Material->SetRenderState("AlphaBlend");
    //m_Bubble->SetMaterial(0, Material);

    return true;
}

void CCollectibleItems::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    AddWorldRotationY(DeltaTime * 100.f);
}

void CCollectibleItems::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CCollectibleItems* CCollectibleItems::Clone() const
{
    return new CCollectibleItems(*this);
}

void CCollectibleItems::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CCollectibleItems::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CCollectibleItems::Collision_Player(const CollisionResult& result)
{
    if (m_ColItemType != EColItemType::ShinyFlower) {
        // 사운드 처리
        CSound* Sound = m_Scene->GetResource()->FindSound("GetCollectible");

        if (Sound)
            Sound->Play();
    }
}
