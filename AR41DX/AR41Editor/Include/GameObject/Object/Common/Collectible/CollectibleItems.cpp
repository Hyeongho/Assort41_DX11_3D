#include "CollectibleItems.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "../../../Player.h"
#include "Scene/Scene.h"
#include "Collectible_Bubble.h"

CCollectibleItems::CCollectibleItems()
{
    SetTypeID<CCollectibleItems>();

    m_ObjectTypeName = "CollectibleItems";

    m_ColItemType = EItemList::End;
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

    m_Bubble = m_Scene->CreateObject<CCollectible_Bubble>("Bubble");
}

bool CCollectibleItems::Init()
{
    CGameObject::Init();




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
    if (m_ColItemType != EItemList::ShinyFlower) {
        CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

        if (Player) {
            switch (m_ColItemType)
            {
            case EItemList::GoldenSpatula:
                // 플레이어 황금 뒤집개 추가
                Player->AddSpatula();

                // 컷씬 실행
                // Player->PlayCutScene(CutSceneName);
                break;
            case EItemList::Sock:
                // 플레이어 양말 추가
                Player->AddSock();
                break;
            case EItemList::UnderWear:
                // 플레이어 HP 추가
                if (Player->AddHp() == 0)
                    return;
                break;
            }
        }

        // 사운드 처리
        CSound* Sound = m_Scene->GetResource()->FindSound("GetCollectible");

        if (Sound)
            Sound->Play();

        // 삭제 처리
        Destroy();
    }
}
