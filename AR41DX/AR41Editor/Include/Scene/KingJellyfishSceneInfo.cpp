#include "KingJellyfishSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/KingJellyfish.h"
#include "../GameObject/Object/Pool.h"
#include "Component/ColliderOBB3D.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/LightComponent.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"

CKingJellyfishSceneInfo::CKingJellyfishSceneInfo()
{
	m_ClassTypeName = "KingJellyfishSceneInfo";
}

CKingJellyfishSceneInfo::~CKingJellyfishSceneInfo()
{
}

bool CKingJellyfishSceneInfo::Init()
{
	CSceneInfo::Init();

	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CGameObject* KingJellyfishMap = m_Owner->CreateObject<CGameObject>("KingJellyfishMap");
	CStaticMeshComponent* StaticMeshComponent = KingJellyfishMap->CreateComponent<CStaticMeshComponent>("KingJellyfishMap");
	StaticMeshComponent->SetMesh("JellyfishFieldBoss");
	StaticMeshComponent->SetMeshSize(1500.f, 1500.f, 1500.f);
	KingJellyfishMap->SetWorldPosition(15000.f, -5150.f, -8000.f);

	CGameObject* MapCollider = m_Owner->CreateObject<CGameObject>("MapCollider");
	CColliderOBB3D* Collider = MapCollider->CreateComponent<CColliderOBB3D>("Collider");
	Collider->SetWorldPosition(8750.f, -520.f, -1600.f);
	Collider->SetBoxHalfSize(2500.f, 500.f, 2500.f);
	Collider->SetCollisionProfile("Ground");

	CPool* Pool = m_Owner->CreateObject<CPool>("Pool");
	Pool->SetWorldPosition(8800.f, 0.f, -1700.f);
	Pool->SetWorldScale(1.f, 1.f, 1.f);

	m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetRespawnPos(8800.f, 0.f, -1700.f);
	SetPlayerObject(Player);

	CKingJellyfish* KingJellyfish = m_Owner->CreateObject<CKingJellyfish>("KingJellyfish");
	KingJellyfish->SetWorldPosition(8800.f, 0.f, -1500.f);

	//CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	//CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	//TerrainComponent->CreateTerrain(807, 604, 40.f, 40.f, TEXT("LandScape/ZellyFishField4.png"));

	//CGameObject* Water = m_Owner->CreateObject<CGameObject>("Water");
	//CStaticMeshComponent* WaterMesh = Water->CreateComponent<CStaticMeshComponent>("WaterMesh");
	//WaterMesh->SetMesh("Water");
	//WaterMesh->SetWorldScale(2.f, 1.f, 2.f);
	//WaterMesh->SetWorldPosition(10500.f, 700.f, 55000.f);
	return true;
}
