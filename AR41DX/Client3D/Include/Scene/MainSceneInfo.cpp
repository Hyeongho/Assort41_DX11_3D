#include "MainSceneInfo.h"
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
#include "Component/TerrainComponent.h"
#include "Component/BillboardComponent.h"
#include "../UI/PlayerUI.h"

CMainSceneInfo::CMainSceneInfo()
{
}

CMainSceneInfo::~CMainSceneInfo()
{
}

bool CMainSceneInfo::Init()
{
	CSceneInfo::Init();

	//SetPlayerObject(Patrick);

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	//m_PlayerUI = m_Owner->GetViewport()->CreateUIWindow<CPlayerUI>("PlayerUI");

	SetPlayerObject(Player);

	//CKingJellyfish* KingJellyfish = m_Owner->CreateObject<CKingJellyfish>("KingJellyfish");
	CJellyfish* Jellyfish = m_Owner->CreateObject<CJellyfish>("Jellyfish");

	CFodder* Fodder = m_Owner->CreateObject<CFodder>("Fodder");
	CHammer* Hammer = m_Owner->CreateObject<CHammer>("Hammer");
	CDuplicatotron* Duplicatotron = m_Owner->CreateObject<CDuplicatotron>("Duplicatotron");


	//CJellyfishField* JellyfishField = m_Owner->CreateObject<CJellyfishField>("JellyfishField");
	CBikiniBottomBuildings* BikiniBottomBuildings = m_Owner->CreateObject<CBikiniBottomBuildings>("BikiniBottomBuildings");
	//SetPlayerObject(BikiniBottomBuildings);
	//BikiniBottomBuildings->SetWorldPosition(100.f, 100.f);

	//CWeapon* Weapon = m_Owner->CreateObject<CWeapon>("Weapon");

	//Player->AddChildToSocket("Weapon", Weapon);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("Terrain");

	CTerrainComponent* Terrain = TerrainObj->CreateComponent<CTerrainComponent>("Terrain");

	Terrain->CreateTerrain(129, 129, 100.f, 100.f, TEXT("LandScape/height1.bmp"));

	CGameObject* BillboardObj = m_Owner->CreateObject<CGameObject>("Billboard");

	CBillboardComponent* Billboard = BillboardObj->CreateComponent<CBillboardComponent>("Billboard");

	Billboard->SetWorldPosition(300.f, 0.f, 500.f);


	return true;
}