#include "JellyfishField1SceneInfo.h"

#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Jellyfish.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "../GameObject/Cannon.h"
#include "../GameObject/Duplicatotron.h"
#include "../GameObject/Object/JumpTree.h"
#include "../GameObject/Object/IceCube.h"
#include "../GameObject/Object/CheckPoint.h"
#include "../GameObject/Object/TeleportBox.h"
#include "../GameObject/Object/JFF/Grass.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/TaxiDriver.h"
#include "../GameObject/Npc/InfoSign.h"
#include "../GameObject/Tikis/Tiki_Wood.h"
#include "../GameObject/Object/Common/Collectible/ShinyFlower.h"
#include "../GameObject/Object/Common/Collectible/GoldenSpatula.h"
#include "../GameObject/Object/Common/Collectible/Sock.h"
#include "../GameObject/Object/Common/Collectible/UnderWear.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"
#include "../UI/Fade.h"

CJellyfishField1SceneInfo::CJellyfishField1SceneInfo()
{
    m_ClassTypeName = "JellyfishField1SceneInfo";
}

CJellyfishField1SceneInfo::~CJellyfishField1SceneInfo()
{
}

void CJellyfishField1SceneInfo::Start()
{
	m_Fade->SetState(EFade_State::FadeIn_Start);
}

bool CJellyfishField1SceneInfo::Init()
{
	CSceneInfo::Init();

	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CResourceManager::GetInst()->SoundPlay("JellyfishField");
	CResourceManager::GetInst()->SetVolume(30.f);

	m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetRespawnPos(14000.f, 15000.f, 900.f);
	SetPlayerObject(Player);

	/*CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->SetHeight(10.f);
	TerrainComponent->CreateTerrain(715.f, 680.f, 60.f, 60.f, TEXT("LandScape/JellyfishField1_1.PNG"));*/
	//TerrainComponent->SetWorldScale(1.f, 1.f, 1.f);
	//TerrainComponent->SetWorldPosition(12000.f, 2100.f, 0.f);
	//TerrainComponent->SetWorldRotation(5.f, 0.f, 0.f);

	CGameObject* Water = m_Owner->CreateObject<CGameObject>("Water");
	CStaticMeshComponent* WaterMesh = Water->CreateComponent<CStaticMeshComponent>("WaterMesh");
	WaterMesh->SetMesh("Water");
	WaterMesh->SetMaterial(0, "Sea");
	WaterMesh->SetWorldScale(3.f, 1.f, 3.f);
	WaterMesh->SetWorldPosition(14000.f, 1350.f, 81000.f);

	CGameObject* JellyfishField = m_Owner->CreateObject<CGameObject>("JellyfishField");
	CStaticMeshComponent* JellyfishFieldMesh = JellyfishField->CreateComponent<CStaticMeshComponent>("Mesh");
	JellyfishFieldMesh->SetMesh("JellyfishField1");
	JellyfishFieldMesh->SetWorldScale(1.f, 1.f, 1.f);
	JellyfishFieldMesh->SetWorldPosition(7000.f, 3500.f, 21000.f);


	CGameObject* Map = m_Owner->CreateObject<CGameObject>("Map");
	CStaticMeshComponent* StaticMeshComponent = Map->CreateComponent<CStaticMeshComponent>("Mesh");
	StaticMeshComponent->SetMesh("JellyFishFieldTestKKB");
	Map->SetWorldPosition(7000.f, 3500.f, 21000.f);


	// ±èÇüÈ£(Ground)
	{
		CGameObject* Ground = m_Owner->CreateObject<CGameObject>("Ground1");
		CColliderOBB3D* OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(14300.f, 2100.f, 1600.f);
		OBB->SetWorldRotationY(330.f);
		OBB->SetBoxHalfSize(800.f, 10.f, 800.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground2");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(13700.f, 2000.f, 1600.f);
		OBB->SetWorldRotationY(20.f);
		OBB->SetBoxHalfSize(1100.f, 10.f, 1300.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground3");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(14000.f, 1800.f, 4600.f);
		OBB->SetBoxHalfSize(2000.f, 10.f, 2000.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground4");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(14300.f, 1800.f, 8000.f);
		OBB->SetWorldRotationY(30.f);
		OBB->SetBoxHalfSize(700.f, 10.f, 2000.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground5");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(15500.f, 1800.f, 10000.f);
		OBB->SetWorldRotationY(30.f);
		OBB->SetBoxHalfSize(1900.f, 10.f, 1700.f);
		OBB->SetCollisionProfile("Ground");

		//CGameObject* Platform1 = m_Owner->CreateObject<CGameObject>("Platform1");
		//CColliderOBB3D* OBB1 = Platform1->CreateComponent<CColliderOBB3D>("OBB1");
		//OBB1->SetBoxHalfSize(1000.f, 10.f, 1000.f);
		//OBB1->SetWorldPosition(14000.f, 2100.f, 1600.f);
		//OBB1->SetCollisionProfile("Ground");

		//CGameObject* Platform2 = m_Owner->CreateObject<CGameObject>("Platform2");
		//CColliderOBB3D* OBB2 = Platform2->CreateComponent<CColliderOBB3D>("OBB2");
		//OBB2->SetBoxHalfSize(2000.f, 10.f, 2000.f);
		//OBB2->SetWorldPosition(14000.f, 1700.f, 4600.f);
		//OBB2->SetCollisionProfile("Ground");

		//CGameObject* Platform3 = m_Owner->CreateObject<CGameObject>("Platform3");
		//CColliderOBB3D* OBB3 = Platform3->CreateComponent<CColliderOBB3D>("OBB3");
		//OBB3->SetBoxHalfSize(1000.f, 10.f, 2000.f);
		//OBB3->SetWorldPosition(14000.f, 1700.f, 8000.f);
		//OBB3->SetWorldRotation(0.f, 45.f, 0.f);
		//OBB3->SetCollisionProfile("Ground");

		CGameObject* Platform4 = m_Owner->CreateObject<CGameObject>("Platform4");
		CColliderOBB3D* OBB4 = Platform4->CreateComponent<CColliderOBB3D>("OBB4");
		OBB4->SetBoxHalfSize(2000.f, 10.f, 6000.f);
		OBB4->SetWorldPosition(8000.f, 1700.f, 12000.f);
		OBB4->SetWorldRotation(0.f, 60.f, 0.f);
		OBB4->SetCollisionProfile("Ground");
	}


	// ±è±â¹é(Monster, Npc, Cannon, Collectible)
	{
		CTiki_Wood* Tiki = m_Owner->CreateObject<CTiki_Wood>("TikiWood1");
		Tiki->SetWorldPosition(0.f, 0.f, 0.f);

		for (int i = 0; i < 3; i++) {
			std::string TikiWoodName = "TikiWood" + std::to_string(i + 2);

			Tiki = m_Owner->CreateObject<CTiki_Wood>(TikiWoodName);
			Tiki->SetWorldPosition(13000.f, 1750.f, 3900.f + 200.f * i);
		}

		for (int i = 0; i < 3; i++) {
			std::string TikiWoodName = "TikiWood" + std::to_string(i + 5);

			Tiki = m_Owner->CreateObject<CTiki_Wood>(TikiWoodName);
			Tiki->SetWorldPosition(15000.f, 1720.f, 5500.f + 200.f * i);
		}

		for (int i = 0; i < 3; i++) {
			std::string TikiWoodName = "TikiWood" + std::to_string(i + 8);

			Tiki = m_Owner->CreateObject<CTiki_Wood>(TikiWoodName);
			Tiki->SetWorldPosition(16500.f, 1800.f, 10200.f - 200.f * i);
		}


		CCannon* Cannon = m_Owner->CreateObject<CCannon>("Cannon");
		Cannon->SetWorldPosition(12000.f, 2900.f, 17000.f);
		Cannon->SetWorldRotationY(340.f);


		CHammer* Hammer = m_Owner->CreateObject<CHammer>("Hammer1");
		Hammer->SetWorldPosition(16000.f, 1650, 9800.f);


		CFodder* Fodder = m_Owner->CreateObject<CFodder>("Fodder1");
		Fodder->SetWorldPosition(15000.f, 1750.f, 3500.f);

		Fodder = m_Owner->CreateObject<CFodder>("Fodder2");
		Fodder->SetWorldPosition(15000.f, 1650.f, 9000.f);


		CJellyfish* Jellyfish = m_Owner->CreateObject<CJellyfish>("Jellyfish1");
		Jellyfish->SetWorldPosition(14100.f, 1900.f, 5000.f);

		Jellyfish = m_Owner->CreateObject<CJellyfish>("Jellyfish2");
		Jellyfish->SetWorldPosition(16000.f, 2000.f, 11000.f);


		CTaxiDriver* TaxiDriver = m_Owner->CreateObject<CTaxiDriver>("Taxi");
		TaxiDriver->SetWorldPosition(13000.f, 1900.f, 800.f);
		TaxiDriver->SetWorldRotationY(50.f);


		CSquidward* Squidward = m_Owner->CreateObject<CSquidward>("Squidward");
		Squidward->SetWorldPosition(13600.f, 2100.f, 2000.f);
		Squidward->SetWorldRotationY(270.f);


		CUnderWear* UnderWear = m_Owner->CreateObject<CUnderWear>("UnderWear");
		UnderWear->SetWorldPosition(13500.f, 1800.f, 7200.f);

		for (int i = 0; i < 5; i++) {
			CShinyFlower* Flower = m_Owner->CreateObject<CShinyFlower>("ShinyFlower_JumpTree1_" + std::to_string(i + 1));
			Flower->SetWorldPosition(15600.f, 2200.f + 300.f * i, 3500.f);
			Flower->SetFixedFlower();
		}

		CGoldenSpatula* GoldenSpatula = m_Owner->CreateObject<CGoldenSpatula>("GoldenSpatula");
		GoldenSpatula->SetWorldPosition(15600.f, 3700.f, 3500.f);

		for (int i = 0; i < 5; i++) {
			CShinyFlower* Flower = m_Owner->CreateObject<CShinyFlower>("ShinyFlower_JumpTree2_" + std::to_string(i + 1));
			Flower->SetWorldPosition(15600.f, 2950.f + 300.f * i, 5000.f);
			Flower->SetFixedFlower();
		}

		CSock* Sock = m_Owner->CreateObject<CSock>("Sock");
		Sock->SetWorldPosition(15600.f, 2950.f + 1500.f, 5000.f);


		for (int i = 0; i < 5; i++) {
			CShinyFlower* Flower = m_Owner->CreateObject<CShinyFlower>("ShinyFlower_Stage2_JumpTree1_" + std::to_string(i + 1));
			Flower->SetWorldPosition(11800.f, 3000.f + 300.f * i, 15528.f);
			Flower->SetFixedFlower();
		}

		GoldenSpatula = m_Owner->CreateObject<CGoldenSpatula>("GoldenSpatula");
		GoldenSpatula->SetWorldPosition(11800.f, 4500.f, 15528.f);
	}


	// ±è¹üÁß(Tree) -> Stage 1
	{
		CJumpTree* JumpTree = m_Owner->CreateObject<CJumpTree>("JumpTree1");
		JumpTree->SetWorldPosition(15600.f, 900.f, 3500.f);

		JumpTree = m_Owner->CreateObject<CJumpTree>("JumpTree2");
		JumpTree->SetWorldPosition(15600.f, 1650.f, 5000.f);


		CGameObject* RedTree = m_Owner->CreateObject<CGameObject>("RedTree1");
		CStaticMeshComponent* RedTreeMesh = RedTree->CreateComponent<CStaticMeshComponent>("RedTreeMesh");
		RedTreeMesh->SetMesh("RedTree");
		RedTreeMesh->SetWorldPosition(13600.f, 2050.f, 1500.f);
		RedTree->SetWorldScale(2.0f, 2.0f, 2.0f);

		RedTree = m_Owner->CreateObject<CGameObject>("RedTree2");
		RedTreeMesh = RedTree->CreateComponent<CStaticMeshComponent>("RedTreeMesh");
		RedTreeMesh->SetMesh("RedTree");
		RedTreeMesh->SetWorldPosition(13100.f, 1700.f, 5100.f);
		RedTree->SetWorldScale(2.0f, 2.0f, 2.0f);

		RedTree = m_Owner->CreateObject<CGameObject>("RedTree3");
		RedTreeMesh = RedTree->CreateComponent<CStaticMeshComponent>("RedTreeMesh");
		RedTreeMesh->SetMesh("RedTree");
		RedTreeMesh->SetWorldPosition(16600.f, 1600.f, 9300.f);
		RedTree->SetWorldScale(2.0f, 2.0f, 2.0f);


		CGameObject* PurpleTree = m_Owner->CreateObject<CGameObject>("PurpleTree1");
		CStaticMeshComponent* PurpleTreeMesh = PurpleTree->CreateComponent<CStaticMeshComponent>("PurpleTreeMesh");
		PurpleTreeMesh->SetMesh("PurpleTree");
		PurpleTreeMesh->SetWorldPosition(16000.f, 1700.f, 3700.f);
		PurpleTree->SetWorldScale(2.0f, 2.0f, 2.0f);


		CGameObject* DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree1");
		CStaticMeshComponent* DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh");
		DoubleTreeMesh->SetMesh("DoubleTree");
		DoubleTreeMesh->SetWorldPosition(12900.f, 1700.f, 6300.f);
		DoubleTreeMesh->SetWorldRotationY(250.f);
		DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

		DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree2");
		DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh");
		DoubleTreeMesh->SetMesh("DoubleTree");
		DoubleTreeMesh->SetWorldPosition(15300.f, 1600.f, 11900.f);
		DoubleTreeMesh->SetWorldRotationY(180.f);
		DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);


		CGameObject* DoubleTree_Y = m_Owner->CreateObject<CGameObject>("DoubleTree_Y1");
		CStaticMeshComponent* DoubleTree_YMesh = DoubleTree_Y->CreateComponent<CStaticMeshComponent>("DoubleTree_Y");
		DoubleTree_YMesh->SetMesh("DoubleTree_Yellow");
		DoubleTree_YMesh->SetWorldPosition(14800.f, 2100.f, 2000.f);
		DoubleTree_YMesh->SetWorldRotationY(220.f);
		DoubleTree_Y->SetWorldScale(2.0f, 2.0f, 2.0f);

		DoubleTree_Y = m_Owner->CreateObject<CGameObject>("DoubleTree_Y2");
		DoubleTree_YMesh = DoubleTree_Y->CreateComponent<CStaticMeshComponent>("DoubleTree_Y");
		DoubleTree_YMesh->SetMesh("DoubleTree_Yellow");
		DoubleTree_YMesh->SetWorldPosition(15000.f, 1600.f, 8300.f);
		DoubleTree_YMesh->SetWorldRotationY(220.f);
		DoubleTree_Y->SetWorldScale(2.0f, 2.0f, 2.0f);


		CGameObject* DoubleTree_P = m_Owner->CreateObject<CGameObject>("DoubleTree_P1");
		CStaticMeshComponent* DoubleTree_PMesh = DoubleTree_P->CreateComponent<CStaticMeshComponent>("DoubleTree_P");
		DoubleTree_PMesh->SetMesh("DoubleTree_Purple");
		DoubleTree_PMesh->SetWorldPosition(16500.f, 1600.f, 8700.f);
		DoubleTree_PMesh->SetWorldRotationY(250.f);
		DoubleTree_P->SetWorldScale(2.0f, 2.0f, 2.0f);
	}


	// ±è¹üÁß(Tree) -> Stage 2
	{
		CJumpTree* JumpTree = m_Owner->CreateObject<CJumpTree>("Stage2_JumpTree1");
		JumpTree->SetWorldPosition(11800.f, 1700.f, 15528.f);

		CGameObject* RedTree = m_Owner->CreateObject<CGameObject>("Stage2_RedTree1");
		CStaticMeshComponent* RedTreeMesh = RedTree->CreateComponent<CStaticMeshComponent>("RedTreeMesh");
		RedTreeMesh->SetMesh("RedTree");
		RedTreeMesh->SetWorldPosition(12740.f, 1900.f, 15868.f);

		RedTree = m_Owner->CreateObject<CGameObject>("Stage2_RedTree2");
		RedTreeMesh = RedTree->CreateComponent<CStaticMeshComponent>("RedTreeMesh");
		RedTreeMesh->SetMesh("RedTree");
		RedTreeMesh->SetWorldPosition(4417.f, 2000.f, 14364.f);

		CGameObject* PurpleTree = m_Owner->CreateObject<CGameObject>("Stage2_PurpleTree1");
		CStaticMeshComponent* PurpleTreeMesh = PurpleTree->CreateComponent<CStaticMeshComponent>("PurpleTreeMesh");
		PurpleTreeMesh->SetMesh("PurpleTree");
		PurpleTreeMesh->SetWorldPosition(8000.f, 2500.f, 16700.f);

		CGameObject* DoubleTree = m_Owner->CreateObject<CGameObject>("Stage2_DoubleTree1");
		CStaticMeshComponent* DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh");
		DoubleTreeMesh->SetMesh("DoubleTree");
		DoubleTreeMesh->SetWorldPosition(13125.f, 1700.f, 17185.f);
		DoubleTreeMesh->SetWorldRotationY(90.f);

		CGameObject* DoubleTree_Y = m_Owner->CreateObject<CGameObject>("Stage2_DoubleTree_Y1");
		CStaticMeshComponent* DoubleTree_YMesh = DoubleTree_Y->CreateComponent<CStaticMeshComponent>("DoubleTree_Y");
		DoubleTree_YMesh->SetMesh("DoubleTree_Yellow");
		DoubleTree_YMesh->SetWorldPosition(14011.f, 1700.f, 16600.f);
		DoubleTree_YMesh->SetWorldRotationY(150.f);

		DoubleTree_Y = m_Owner->CreateObject<CGameObject>("Stage2_DoubleTree_Y2");
		DoubleTree_YMesh = DoubleTree_Y->CreateComponent<CStaticMeshComponent>("DoubleTree_Y");
		DoubleTree_YMesh->SetMesh("DoubleTree_Yellow");
		DoubleTree_YMesh->SetWorldPosition(5735.f, 2400.f, 15700.f);
		DoubleTree_YMesh->SetWorldRotationY(90.f);

		CGameObject* DoubleTree_P = m_Owner->CreateObject<CGameObject>("Stage2_DoubleTree_P1");
		CStaticMeshComponent* DoubleTree_PMesh = DoubleTree_P->CreateComponent<CStaticMeshComponent>("DoubleTree_P");
		DoubleTree_PMesh->SetMesh("DoubleTree_Purple");
		DoubleTree_PMesh->SetWorldPosition(12320.f, 1850.f, 16000.f);
		DoubleTree_PMesh->SetWorldRotationY(100.f);
	}


	// ÀÌ¼ÒÇÏ(Rock, WaterFall)
	{
		CGameObject* OBJ = m_Owner->CreateObject<CGameObject>("OBJ");
		CStaticMeshComponent* TwoRock = OBJ->CreateComponent<CStaticMeshComponent>("TwoRock");
		TwoRock->SetMesh("TwoRock");
		OBJ->SetWorldPosition(38000.f, 4500.f, 5000.f);
		OBJ->SetWorldRotationY(120.f);

		OBJ = m_Owner->CreateObject<CGameObject>("OBJ");
		CStaticMeshComponent* WaterFall = OBJ->CreateComponent<CStaticMeshComponent>("WaterFall");
		WaterFall->SetMesh("WaterFallSmall");
		OBJ->SetWorldPosition(18000.f, 500.f, 6700.f);
		OBJ->SetWorldScale(2.f, 2.f, 2.f);
	}


	// ±èº¸¿ø(Object)
	{
		CCheckPoint* CheckPoint = m_Owner->CreateObject<CCheckPoint>("CheckPoint");
		CheckPoint->SetWorldPosition(12605.79f, 1624.55f, 13326.54f);
		CheckPoint->SetWorldRotationY(-70.f);

		CGameObject* Bridge = m_Owner->CreateObject<CGameObject>("Bridge");
		CStaticMeshComponent* BridgeMesh = Bridge->CreateComponent<CStaticMeshComponent>("BridgeMesh");
		BridgeMesh->SetMesh("Bridge");
		BridgeMesh->SetWorldScale(1.f, 1.f, 1.f);
		BridgeMesh->SetWorldPosition(14000.00f, 1200.00f, 13000.00f);
		BridgeMesh->SetWorldPositionY(-50.f);
		BridgeMesh->SetWorldScale(0.f, 0.f, 1.2f);

		CInfoSign* InfoSign = m_Owner->CreateObject<CInfoSign>("InfoSign");
		InfoSign->SetWorldScale(1.f, 1.f, 1.f);
		InfoSign->SetWorldPosition(13020.78f, 1732.00f, 3542.88f);
		InfoSign->SetWorldRotationY(280.f);
		InfoSign->SetMapPos(EMapList::Jelly_Fish_Field);

		CTeleportBox* TeleportBox1 = m_Owner->CreateObject<CTeleportBox>("TeleportBox");
		TeleportBox1->SetBoxIndex(1);
		TeleportBox1->SetWorldScale(1.f, 1.f, 1.f);
		TeleportBox1->SetWorldPosition(14000.f, 1700.f, 10000.f);

		CIceCube* IceCube = m_Owner->CreateObject<CIceCube>("IceCube");
		IceCube->SetWorldPosition(10800.00f, 2514.21f, 11290.00f);
		IceCube->SetWorldRotationZ(10.f);
	}

	m_Fade = m_Owner->GetViewport()->CreateUIWindow<CFade>("Fade");

	return true;
}
