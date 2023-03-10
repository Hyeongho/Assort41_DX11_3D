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
#include "../GameObject/Cannon.h"
#include "../GameObject/Pufferfish.h"
#include "Component/TerrainComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/StaticMeshComponent.h"
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
	GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetWorldPosition(16500.f, 0.f, 12200.f);
	SetPlayerObject(Player);

	/*CTeeterRock* TeeterRock = m_Owner->CreateObject<CTeeterRock>("TeeterRock");
	TeeterRock->SetWorldPosition(16500.f, 0.f, 12200.f);*/

	CCannon* Cannon = m_Owner->CreateObject<CCannon>("Cannon");
	Cannon->SetWorldPosition(16500.f, 0.f, 12200.f);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(680, 631, 40.f, 40.f, TEXT("LandScape/BikiniCity_Height.png"));

	//애니메이션 문제
	//CPatric* Patric = m_Owner->CreateObject<CPatric>("Patric");
	//Patric->SetWorldPosition(16500.f, 0.f, 12200.f);

	return true;
}
