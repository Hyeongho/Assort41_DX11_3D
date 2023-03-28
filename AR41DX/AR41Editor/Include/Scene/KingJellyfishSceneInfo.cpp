#include "KingJellyfishSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/KingJellyfish.h"
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

	m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetRespawnPos(10000.f, 0.f, 10000.f);
	SetPlayerObject(Player);

	CKingJellyfish* KingJellyfish = m_Owner->CreateObject<CKingJellyfish>("KingJellyfish");
	KingJellyfish->SetWorldPosition(10000.f, 0.f, 20000.f);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(576, 579, 40.f, 40.f, TEXT("LandScape/SM_Terrain_JF_02.png"));
	//TerrainComponent->CreateTerrain(807, 604, 40.f, 40.f, TEXT("LandScape/ZellyFishField4.png"));

	//CGameObject* Water = m_Owner->CreateObject<CGameObject>("Water");
	//CStaticMeshComponent* WaterMesh = Water->CreateComponent<CStaticMeshComponent>("WaterMesh");
	//WaterMesh->SetMesh("Water");
	//WaterMesh->SetWorldScale(2.f, 1.f, 2.f);
	//WaterMesh->SetWorldPosition(10500.f, 700.f, 55000.f);
	return true;
}
