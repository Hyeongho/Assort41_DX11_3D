#include "JellyfishFieldSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Weapon.h"
//#include "../GameObject/KingJellyfish.h"
#include "../GameObject/Jellyfish.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "../GameObject/Duplicatotron.h"
#include "../GameObject/Npc/Squidward.h"
//#include "../GameObject/JellyfishField.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderCube.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"

CJellyfishFieldSceneInfo::CJellyfishFieldSceneInfo()
{
	m_ClassTypeName = "JellyfishFieldSceneInfo";
}

CJellyfishFieldSceneInfo::~CJellyfishFieldSceneInfo()
{
}

bool CJellyfishFieldSceneInfo::Init()
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
	Player->SetRespawnPos(9200.f, 300.f, 500.f);
	SetPlayerObject(Player);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent= TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(360, 672, 50.f, 50.f, TEXT("LandScape/ZellyFishField.png"));

	CFodder* Fodder = m_Owner->CreateObject<CFodder>("Fodder");
	//CHammer* Hammer = m_Owner->CreateObject<CHammer>("Hammer");
	//CDuplicatotron* Duplicatotron = m_Owner->CreateObject<CDuplicatotron>("Duplicatotron");

	CJellyfish* Jellyfish = m_Owner->CreateObject<CJellyfish>("Jellyfish");
	Jellyfish->SetWorldPosition(12600.f,300.f,3400.f);

	CGameObject* Water = m_Owner->CreateObject<CGameObject>("Water");
	CStaticMeshComponent* WaterMesh = Water->CreateComponent<CStaticMeshComponent>("WaterMesh");
	WaterMesh->SetMesh("Water");
	WaterMesh->SetWorldScale(2.5f, 1.f, 2.5f);
	WaterMesh->SetWorldPosition(11700.f, 700.f, 70000.f);
	return true;
}