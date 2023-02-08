#include "MainSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Weapon.h"
#include "../GameObject/Patrick.h"
#include "../GameObject/Sandy.h"
#include "Component/TerrainComponent.h"
#include "Component/BillboardComponent.h"

CMainSceneInfo::CMainSceneInfo()
{
}

CMainSceneInfo::~CMainSceneInfo()
{
}

bool CMainSceneInfo::Init()
{
	CSceneInfo::Init();

	//CPatrick* Patrick = m_Owner->CreateObject<CPatrick>("Patrick");
	//SetPlayerObject(Patrick);
	
	CPlayer* Sandy = m_Owner->CreateObject<CPlayer>("Sandy");
	SetPlayerObject(Sandy);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("Terrain");

	CTerrainComponent* Terrain = TerrainObj->CreateComponent<CTerrainComponent>("Terrain");

	Terrain->CreateTerrain(129, 129, 100.f, 100.f, TEXT("LandScape/height1.bmp"));

	CGameObject* BillboardObj = m_Owner->CreateObject<CGameObject>("Billboard");

	CBillboardComponent* Billboard = BillboardObj->CreateComponent<CBillboardComponent>("Billboard");

	Billboard->SetWorldPosition(300.f, 0.f, 500.f);


	return true;
}
