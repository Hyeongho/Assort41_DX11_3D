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
	GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);



	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");

	SetPlayerObject(Player);

	CTeeterRock* TeeterRock = m_Owner->CreateObject<CTeeterRock>("TeeterRock");


	return true;
}
