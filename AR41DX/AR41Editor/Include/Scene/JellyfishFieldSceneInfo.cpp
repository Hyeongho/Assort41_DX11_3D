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
#include "../GameObject/Npc/InfoSign.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Object/TeleportBox.h"
#include "../GameObject/Object/Gate.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
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
	TerrainComponent->SetHeight(50.f);
	TerrainComponent->CreateTerrain(360, 672, 60.f, 60.f, TEXT("LandScape/ZellyFishField.png"));

	CGameObject* Water = m_Owner->CreateObject<CGameObject>("Water");
	CStaticMeshComponent* WaterMesh = Water->CreateComponent<CStaticMeshComponent>("WaterMesh");
	WaterMesh->SetMesh("Water");
	WaterMesh->SetWorldScale(3.f, 1.f, 3.f);
	WaterMesh->SetWorldPosition(14000.f, 1350.f, 81000.f);

	CFodder* Fodder = m_Owner->CreateObject<CFodder>("Fodder");
	Fodder->SetWorldPosition(14700.f, 1450.f, 4200.f);

	//CHammer* Hammer = m_Owner->CreateObject<CHammer>("Hammer");
	//CDuplicatotron* Duplicatotron = m_Owner->CreateObject<CDuplicatotron>("Duplicatotron");

	CJellyfish* Jellyfish = m_Owner->CreateObject<CJellyfish>("Jellyfish");
	Jellyfish->SetWorldPosition(13700.f, 1500.f, 4300.f);

	


	//CJumpTree* JumpTree = m_Owner->CreateObject<CJumpTree>("JumpTree");
	//JumpTree->SetWorldPosition(13850.f, 1400.f, 6300.f);

	////
	//CGameObject* JellyfishScene = m_Owner->CreateObject<CGameObject>("JellyfishScene");
	//CStaticMeshComponent* JellyfishMesh = JellyfishScene->CreateComponent<CStaticMeshComponent>("JellyfishMesh");
	//JellyfishMesh->SetMesh("JFBackground");
	//JellyfishMesh->SetWorldScale(1.f, 1.f, 1.f);
	//JellyfishMesh->SetWorldPosition(7000.f, 3500.f, 21000.f);

	// 여기서부터 오브젝트 심기 시작.

	CGameObject* JellyfishField = m_Owner->CreateObject<CGameObject>("Jellyfushfueld1");
	CStaticMeshComponent* JellyfishFieldMesh = JellyfishField->CreateComponent<CStaticMeshComponent>("Jellyfushfueld1");
	JellyfishFieldMesh->SetMesh("Jellyfushfueld1");
	JellyfishFieldMesh->SetWorldScale(1.f, 1.f, 1.f);
	JellyfishFieldMesh->SetWorldPosition(7000.f, 3500.f, 21000.f);

	CGameObject* Bridge = m_Owner->CreateObject<CGameObject>("Bridge");
	CStaticMeshComponent* BridgeMesh = Bridge->CreateComponent<CStaticMeshComponent>("BridgeMesh");
	BridgeMesh->SetMesh("Bridge");
	BridgeMesh->SetWorldScale(1.f, 1.f, 1.f);
	BridgeMesh->SetWorldPosition(10239.30f, 4400.f, 14439.f);
	BridgeMesh->SetWorldPositionY(-50.f);

	CInfoSign* InfoSign = m_Owner->CreateObject<CInfoSign>("InfoSign");
	InfoSign->SetWorldScale(1.f, 1.f, 1.f);
	InfoSign->SetWorldPosition(8527.31f, 4400.f, 9225.f);

	CTeleportBox* TeleportBox1 = m_Owner->CreateObject<CTeleportBox>("TeleportBox");
	TeleportBox1->SetBoxIndex(1);
	TeleportBox1->SetWorldScale(1.f, 1.f, 1.f);
	TeleportBox1->SetWorldPosition(10539.29f, 4400.f, 13200.f);

	CTeleportBox* TeleportBox2 = m_Owner->CreateObject<CTeleportBox>("TeleportBox");
	TeleportBox2->SetBoxIndex(1);
	TeleportBox2->SetWorldScale(1.f, 1.f, 1.f);
	TeleportBox2->SetWorldPosition(8114.33f, 6349.73f, 20265.50f);

	CGate* Gate = m_Owner->CreateObject<CGate>("Gate");
	Gate->SetWorldScale(1.f, 1.f, 1.f);
	Gate->SetWorldPosition(8426.28f, 6446.60f, 23261.25f);

	CIceCube* IceCube = m_Owner->CreateObject<CIceCube>("IceCube");
	IceCube->SetWorldPosition(8007.31f, 4830.04f, 13510.29f);

	CDuplicatotron* Duplicatotron = m_Owner->CreateObject<CDuplicatotron>("Duplicatotron");
	Duplicatotron->SetWorldPosition(8843.32f, 6349.73f, 22203.84f);


	/*
	CTeleportBox* TeleportBox1 = m_Owner->CreateObject<CTeleportBox>("TeleportBox");
	//TeleportBox1->SetBoxIndex(1);

	//CTeleportBox* TeleportBox2 = m_Owner->CreateObject<CTeleportBox>("TeleportBox");
	//TeleportBox2->SetBoxIndex(2);
	//Vector3 TeleportBox2Pos = TeleportBox2->GetWorldPos();

	*/
	return true;
}