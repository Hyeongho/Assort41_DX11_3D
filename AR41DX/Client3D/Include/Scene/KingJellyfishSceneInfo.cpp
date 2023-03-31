#include "KingJellyfishSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/KingJellyfish.h"
#include "../GameObject/Jellyfish.h"
#include "../GameObject/Object/Pool.h"
#include "Component/ColliderOBB3D.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/LightComponent.h"
#include "Component/ParticleComponent.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"
#include "../UI/KingJellyfishIntroUI.h"


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

	//m_Owner->GetViewport()->CreateUIWindow<CKingJellyfishIntroUI>("KingJellyfishIntroUI");

	CResourceManager::GetInst()->SoundPlay("KingJellyfish_Idle");
	CResourceManager::GetInst()->SetVolume(2.f);

	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->SetHeight(1.f);
	TerrainComponent->CreateTerrain(800, 600, 9.f, 9.f, TEXT("LandScape/KingJellyFish.png"));
	TerrainComponent->GetMaterial(0)->SetOpacity(0.f);

	CGameObject* KingJellyfishMap = m_Owner->CreateObject<CGameObject>("KingJellyfishMap");
	CStaticMeshComponent* StaticMeshComponent = KingJellyfishMap->CreateComponent<CStaticMeshComponent>("KingJellyfishMap");
	StaticMeshComponent->SetMesh("JellyfishFieldBoss");
	StaticMeshComponent->SetMeshSize(1500.f, 1500.f, 1500.f);
	KingJellyfishMap->SetWorldPosition(10000.f, -4885.f, -4000.f);

	CGameObject* Water = m_Owner->CreateObject<CGameObject>("Water");
	CStaticMeshComponent* BossWater = Water->CreateComponent<CStaticMeshComponent>("Water");
	BossWater->SetMesh("BossMapWater");
	BossWater->SetWorldPosition(3800.f, -25.f, 2300.f);
	BossWater->GetMaterial(0)->SetOpacity(0.7f);
	BossWater->GetMaterial(0)->SetRenderState("AlphaBlend");

	CGameObject* WaterParticle = m_Owner->CreateObject<CGameObject>("WaterParticle");
	CParticleComponent* BossWaterParticle = WaterParticle->CreateComponent<CParticleComponent>("BossWaterParticle");
	BossWaterParticle->SetParticle("Glister");
	BossWaterParticle->SetWorldPosition(3800.f, -35.f, 2300.f);

	CGameObject* WaterDrop = m_Owner->CreateObject<CGameObject>("WaterDrop");
	CParticleComponent* WaterDropParticle = WaterDrop->CreateComponent<CParticleComponent>("WaterDropParticle");
	WaterDropParticle->SetParticle("Shower");
	WaterDropParticle->SetWorldPosition(3800.f, 2200, 1800);

	CPool* Pool = m_Owner->CreateObject<CPool>("Pool");
	Pool->SetWorldPosition(3800.f, 265.f, 2300.f);
	Pool->SetWorldScale(1.f, 1.f, 1.f);

	m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetWorldPosition(5000.f, 270.f, 2700.f);
	SetPlayerObject(Player);

	CKingJellyfish* KingJellyfish = m_Owner->CreateObject<CKingJellyfish>("KingJellyfish");
	KingJellyfish->SetWorldPosition(4400.f, 250.f, 1600.f);

	return true;
}
