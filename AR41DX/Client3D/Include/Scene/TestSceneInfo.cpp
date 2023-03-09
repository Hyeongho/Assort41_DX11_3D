#include "TestSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Weapon.h"
#include "../GameObject/KingJellyfish.h"
#include "../GameObject/Jellyfish.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "../GameObject/Duplicatotron.h"
#include "../GameObject/BikiniBottomBuildings.h"
#include "../GameObject/JellyfishField.h"
#include "../GameObject/TeeterRock.h"
#include "../GameObject/Tikis/Tiki_Wood.h"
#include "../GameObject/Tikis/Tiki_Thunder.h"
#include "../GameObject/Tikis/Tiki_Stone.h"
#include "../GameObject/Object/InteractButton.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "Component/TerrainComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/ParticleComponent.h"
#include "../UI/PlayerUI.h"
#include "Component/LightComponent.h"

CTestSceneInfo::CTestSceneInfo()
{
}

CTestSceneInfo::~CTestSceneInfo()
{
}

bool CTestSceneInfo::Init()
{
	CSceneInfo::Init();

	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(0.f, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);


	
	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetWorldPosition(16500.f, 0.f, 12200.f);
	SetPlayerObject(Player);

	//CTiki_Wood* Tiki_Woods = m_Owner->CreateObject<CTiki_Wood>("Tiki_Woods");
	//Tiki_Woods->SetWorldPositionZ(500.f);

	//CTiki_Thunder* Tiki_Thunder = m_Owner->CreateObject<CTiki_Thunder>("Tiki_Thunder");
	//Tiki_Thunder->SetWorldPositionZ(-500.f);

	//CTiki_Stone* Tiki_Stone = m_Owner->CreateObject<CTiki_Stone>("Tiki_Stone");
	//Tiki_Stone->SetWorldPositionX(500.f);

	CInteractButton* Button = m_Owner->CreateObject<CInteractButton>("InteractButton");
	Button->SetWorldPosition(16700.f, 0.f, 12500.f);

	CMrKrabs* MrKrabs = m_Owner->CreateObject<CMrKrabs>("MrKrabs");
	MrKrabs->SetWorldPosition(16400.f, 0.f, 12500.f);


	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(680, 631, 40.f, 40.f, TEXT("LandScape/BikiniCity_Height.png"));

	return true;
}
