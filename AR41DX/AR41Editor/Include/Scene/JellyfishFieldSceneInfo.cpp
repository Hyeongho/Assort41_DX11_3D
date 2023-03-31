#include "JellyfishFieldSceneInfo.h"
#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Weapon.h"
#include "../GameObject/Jellyfish.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "../GameObject/Object/JumpTree.h"
#include "../GameObject/Duplicatotron.h"
#include "../GameObject/Object/IceCube.h"
#include "../GameObject/Npc/Squidward.h"
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
	Player->SetRespawnPos(14000.f, 1500.f, 1600.f);
	SetPlayerObject(Player);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->SetHeight(10.f);
	TerrainComponent->CreateTerrain(360, 672, 60.f, 60.f, TEXT("LandScape/ZellyFishField.png"));
	//TerrainComponent->ClearMaterial();
	//TerrainComponent->AddMaterial("JellyfishField");

	//CGameObject* TerrainObj2 = m_Owner->CreateObject<CGameObject>("TerrainObj2");
	//CTerrainComponent* TerrainComponent2 = TerrainObj2->CreateComponent<CTerrainComponent>("TerrainComponent2");
	//TerrainComponent2->SetHeight(50.f);
	//TerrainComponent2->CreateTerrain(360, 672, 60.f, 60.f, TEXT("LandScape/ZellyFishField.png"));
	////TerrainComponent->ClearMaterial();
	////TerrainComponent->AddMaterial("JellyfishField");

	CGameObject* Water = m_Owner->CreateObject<CGameObject>("Water");
	CStaticMeshComponent* WaterMesh = Water->CreateComponent<CStaticMeshComponent>("WaterMesh");
	WaterMesh->SetMesh("Water");
	WaterMesh->SetWorldScale(3.f, 1.f, 3.f);
	WaterMesh->SetWorldPosition(14000.f, 1350.f, 81000.f);

	CGameObject* JellyfishField = m_Owner->CreateObject<CGameObject>("JellyfishField1");
	CStaticMeshComponent* JellyfishFieldMesh = JellyfishField->CreateComponent<CStaticMeshComponent>("JellyfishField1");
	JellyfishFieldMesh->SetMesh("JellyfishField1");
	JellyfishFieldMesh->SetWorldScale(1.f, 1.f, 1.f);
	JellyfishFieldMesh->SetWorldPosition(7000.f, 3500.f, 21000.f);

	CFodder* Fodder = m_Owner->CreateObject<CFodder>("Fodder");
	Fodder->SetWorldPosition(14700.f, 1450.f, 4200.f);

	//CHammer* Hammer = m_Owner->CreateObject<CHammer>("Hammer");
	//CDuplicatotron* Duplicatotron = m_Owner->CreateObject<CDuplicatotron>("Duplicatotron");

	CJellyfish* Jellyfish = m_Owner->CreateObject<CJellyfish>("Jellyfish");
	Jellyfish->SetWorldPosition(13700.f, 1500.f, 4300.f);

	CIceCube* IceCube = m_Owner->CreateObject<CIceCube>("IceCube");
	IceCube->SetWorldPosition(13800.f, 1400.f, 6300.f);


	//CJumpTree* JumpTree = m_Owner->CreateObject<CJumpTree>("JumpTree");
	//JumpTree->SetWorldPosition(13850.f, 1400.f, 6300.f);

	return true;
}