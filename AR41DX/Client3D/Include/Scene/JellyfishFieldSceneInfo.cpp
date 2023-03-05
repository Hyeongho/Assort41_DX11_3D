#include "JellyfishFieldSceneInfo.h"
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
#include "Component/ParticleComponent.h"
#include "../UI/PlayerUI.h"
#include "Component/LightComponent.h"

CJellyfishFieldSceneInfo::CJellyfishFieldSceneInfo()
{
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
	GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CJellyfishField* JellyfishField = m_Owner->CreateObject<CJellyfishField>("JellyfishField");


	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	//m_PlayerUI = m_Owner->GetViewport()->CreateUIWindow<CPlayerUI>("PlayerUI");

	CFodder* Fodder = m_Owner->CreateObject<CFodder>("Fodder");
	CHammer* Hammer = m_Owner->CreateObject<CHammer>("Hammer");
	CDuplicatotron* Duplicatotron = m_Owner->CreateObject<CDuplicatotron>("Duplicatotron");

	SetPlayerObject(Player);



	return true;
}