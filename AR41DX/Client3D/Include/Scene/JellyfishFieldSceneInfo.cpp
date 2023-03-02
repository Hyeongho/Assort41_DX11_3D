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
#include "../GameObject/JellyfishFieldObj.h"
#include "Component/TerrainComponent.h"
#include "Component/BillboardComponent.h"
#include "Component/ParticleComponent.h"
#include "../UI/PlayerUI.h"
#include "Component/LightComponent.h"
#include "Component/StaticMeshComponent.h"

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

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	//m_PlayerUI = m_Owner->GetViewport()->CreateUIWindow<CPlayerUI>("PlayerUI");

	SetPlayerObject(Player);

	/*
	DoubleTree_Red 
	DoubleTree_Yellow
	DoubleTree_Purple
	SmallTree_Red
	SmallTree_Purple
	Seaflower
	BouncingTree
	Clam
	Missile
	Bridge
	*/

	// Objects 
	CJellyfishFieldObj* DoubleTree_Red = m_Owner->CreateObject<CJellyfishFieldObj>("JellyfishFieldObj");
	DoubleTree_Red->CreateComponent<CStaticMeshComponent>("JellyfishFieldObj")->SetMesh("DoubleTree_Red", TEXT("Objects/JellyfishFields/DoubleTree_Red.fbx"), MESH_PATH);
	DoubleTree_Red->SetWorldPosition(100.f, 100.f, 100.f);




	return true;
}