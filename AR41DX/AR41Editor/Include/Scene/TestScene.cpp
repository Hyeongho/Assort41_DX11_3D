#include "TestScene.h"

#include "Scene/Scene.h"

#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderCube.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "../GameObject/Player.h"
#include "../GameObject/Object/BusStop.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/BusDriver.h"
#include "../GameObject/Tikis/Tiki_Wood.h"
#include "../GameObject/Tikis/Tiki_Thunder.h"
#include "../GameObject/Tikis/Tiki_Stone.h"
#include "../GameObject/Player.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/Patric.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/TaxiDriver.h"
#include "../UI/TitleSceneUI.h"
#include "../UI/InteractUI.h"
#include "../UI/DialogUI.h"
#include "../UI/PauseUI.h"

CTestScene::CTestScene()
{
	m_ClassTypeName = "testScene";
}

CTestScene::~CTestScene()
{
}

bool CTestScene::Init()
{
	//CSceneInfo::Init();

	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(0.f, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CInteractUI* InteractUI = m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");
	CDialogUI* Dialog = m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	//CPauseUI* PauseUI = m_Owner->GetViewport()->CreateUIWindow<CPauseUI>("PauseUI");


	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	SetPlayerObject(Player);
	Player->SetWorldPosition(16500.f, 0.f, 12200.f);


	CMrKrabs* Krabs = m_Owner->CreateObject<CMrKrabs>("MrKrabs");
	Krabs->SetWorldPosition(16300.f, 0.f, 12500.f);

	//CBusDriver* Bus = m_Owner->CreateObject<CBusDriver>("BusDriver");
	//Bus->SetWorldPosition(17000.f, 0.f, 11500.f);

	//CBusStop* BusStop = m_Owner->CreateObject<CBusStop>("BusStop");
	//BusStop->SetWorldPosition(16700.f, 0.f, 11500.f);

	CTiki_Wood* TikiWood = m_Owner->CreateObject<CTiki_Wood>("TikiWood");
	TikiWood->SetWorldPosition(16700.f, 0.f, 12500.f);

	CTiki_Stone* TikiStone = m_Owner->CreateObject<CTiki_Stone>("TikiStone");
	TikiStone->SetWorldPosition(16900.f, 0.f, 12500.f);

	CTiki_Thunder* TikiThunder = m_Owner->CreateObject<CTiki_Thunder>("TikiThunder");
	TikiThunder->SetWorldPosition(17100.f, 0.f, 12500.f);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(680, 631, 40.f, 40.f, TEXT("LandScape/BikiniCity_Height.png"));


	//CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	//CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	//GlobalLightComponent->SetLightType(ELightType::Direction);
	//GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
	//m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	////m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");

	////title
	////m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");

	////��Ű�Ͻ�Ƽ ��
	//CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	//Player->SetWorldPosition(16500.f, 0.f, 12200.f);
	//SetPlayerObject(Player);


	//CTiki_Wood* Tiki = m_Owner->CreateObject<CTiki_Wood>("TikiWoods");
	//Tiki->SetWorldPosition(16700.f, 0.f, 12500.f);


	return true;
}
