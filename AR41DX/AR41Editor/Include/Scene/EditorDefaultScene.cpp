
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
//#include "../UI/LoadingUI.h"
#include "../UI/TitleSceneUI.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"
#include "../GameObject/Player.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/Patric.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/BusDriver.h"
#include "../GameObject/Object/InteractButton.h"
#include "../GameObject/Object/BusStop.h"
#include "../GameObject/Tikis/Tiki_Stone.h"
#include "../GameObject/Tikis/Tiki_Thunder.h"
#include "../GameObject/Tikis/Tiki_Wood.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderCube.h"

CEditorDefaultScene::CEditorDefaultScene()
{
	m_ClassTypeName = "EditorDefaultScene";
}

CEditorDefaultScene::~CEditorDefaultScene()
{
}

bool CEditorDefaultScene::Init()
{
	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent*  GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(0, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	//title
	//m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");

	//비키니시티 맵
	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetRespawnPos(16500.f, 0.f, 12200.f);
	SetPlayerObject(Player);

//텍스쳐 출력문제
	//CTiki_Stone* Tiki_Stone = m_Owner->CreateObject<CTiki_Stone>("Tiki_Stone");
	//Tiki_Stone->SetWorldPosition(16500.f, 0.f, 12200.f);

	//CTiki_Thunder* Tiki_Thunder = m_Owner->CreateObject<CTiki_Thunder>("Tiki_Thunder");
	//Tiki_Thunder->SetWorldPosition(16500.f, 0.f, 12200.f);

	//CTiki_Wood* Tiki_Wood = m_Owner->CreateObject<CTiki_Wood>("Tiki_Wood");
	//Tiki_Wood->SetWorldPosition(16500.f, 0.f, 12200.f);
//애니메이션 문제
	//CInteractButton* InteractButton = m_Owner->CreateObject<CInteractButton>("InteractButton");
	//InteractButton->SetWorldPosition(16500.f, 0.f, 12200.f);

	//CPatric* Patric = m_Owner->CreateObject<CPatric>("Patric");
	//Patric->SetWorldPosition(16500.f, 0.f, 12200.f);

	CMrKrabs* MrKrabs = m_Owner->CreateObject<CMrKrabs>("MrKrabs");
	MrKrabs->SetWorldPosition(11000.f, 0.f, 13000.f);
	MrKrabs->SetWorldRotationY(180.f);

	CSquidward* Squidward = m_Owner->CreateObject<CSquidward>("Squidward");
	Squidward->SetWorldPosition(15100.f, 0.f, 14000.f);

	CBusDriver* BusDriver = m_Owner->CreateObject<CBusDriver>("BusDriver");
	BusDriver->SetWorldPosition(9400.f, 0.f, 12700.f);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(680, 631, 40.f, 40.f, TEXT("LandScape/BikiniCity_Height.png"));

	CGameObject* Road = m_Owner->CreateObject<CGameObject>("Road");
	CStaticMeshComponent* RoadMesh = Road->CreateComponent<CStaticMeshComponent>("RoadMesh");
	RoadMesh->SetMesh("BikiniBottomRoad");
	RoadMesh->SetWorldScale(0.3f, 0.3f, 0.3f);
	RoadMesh->SetWorldPosition(12000.f, -40.f, 13250.f);

	CGameObject* PatrickHouse = m_Owner->CreateObject<CGameObject>("PatrickHouse");
	CStaticMeshComponent* PatrickHouseMesh = PatrickHouse->CreateComponent<CStaticMeshComponent>("PatrickHouseMesh");
	PatrickHouseMesh->SetMesh("PatrickHouse");
	PatrickHouseMesh->SetWorldPosition(14300.f, 500.f, 14500.f);
	CColliderCube* PatrickHouseCube = PatrickHouse->CreateComponent<CColliderCube>("PatrickHouseCube");
	PatrickHouseMesh->AddChild(PatrickHouseCube);
	PatrickHouseCube->SetCubeSize(800.f, 400.f, 800.f);
	PatrickHouseCube->SetRelativePositionY(-300.f);
	PatrickHouseCube->SetCollisionProfile("Wall");

	CGameObject* SquidHouse = m_Owner->CreateObject<CGameObject>("SquidHouse");
	CStaticMeshComponent* SquidHouseMesh = SquidHouse->CreateComponent<CStaticMeshComponent>("SquidHouseMesh");
	SquidHouseMesh->SetMesh("SquidHouse");
	SquidHouseMesh->SetWorldPosition(15900.f, 4.f, 14550.f);
	CColliderCube* SquidHouseCube = SquidHouse->CreateComponent<CColliderCube>("SquidHouseCube");
	SquidHouseMesh->AddChild(SquidHouseCube);
	SquidHouseCube->SetRelativePositionY(700.f);
	SquidHouseCube->SetCubeSize(800.f, 1400.f, 800.f);
	SquidHouseCube->SetCollisionProfile("Wall");

	CGameObject* PineAppleHouse = m_Owner->CreateObject<CGameObject>("PineAppleHouse");
	CStaticMeshComponent* PineAppleHouseMesh = PineAppleHouse->CreateComponent<CStaticMeshComponent>("PineAppleHouseMesh");
	PineAppleHouseMesh->SetMesh("PineAppleHouse");
	PineAppleHouseMesh->SetWorldPosition(17000.f, 0.f, 13500.f);
	CColliderCube* PineAppleHouseCube = PineAppleHouse->CreateComponent<CColliderCube>("PineAppleHouseCube");
	PineAppleHouseMesh->AddChild(PineAppleHouseCube);
	PineAppleHouseCube->SetRelativePositionY(400.f);
	PineAppleHouseCube->SetCubeSize(800.f, 800.f, 800.f);
	PineAppleHouseCube->SetCollisionProfile("Wall");

	CGameObject* ChumBucket = m_Owner->CreateObject<CGameObject>("ChumBucket");
	CStaticMeshComponent* ChumBucketMesh = ChumBucket->CreateComponent<CStaticMeshComponent>("ChumBucketMesh");
	ChumBucketMesh->SetMesh("ChumBucket");
	ChumBucketMesh->SetWorldPosition(6800.f, 4.f, 13800.f);
	ChumBucketMesh->SetWorldRotationY(180.f);
	CColliderCube* ChumBucketCube = ChumBucket->CreateComponent<CColliderCube>("ChumBucketCube");
	ChumBucketMesh->AddChild(ChumBucketCube);
	ChumBucketCube->SetRelativePositionY(100.f);
	ChumBucketCube->SetCubeSize(2000.f, 2000.f, 1800.f);
	ChumBucketCube->SetCollisionProfile("Wall");

	CGameObject* Buliding_02 = m_Owner->CreateObject<CGameObject>("Buliding_02");
	CStaticMeshComponent* Buliding_02Mesh = Buliding_02->CreateComponent<CStaticMeshComponent>("Buliding_02Mesh");
	Buliding_02Mesh->SetMesh("Buliding_02");
	Buliding_02Mesh->SetWorldPosition(10200.f, 0.f, 8200.f);
	Buliding_02 = m_Owner->CreateObject<CGameObject>("Buliding_03");
	Buliding_02Mesh = Buliding_02->CreateComponent<CStaticMeshComponent>("Buliding_03Mesh");
	Buliding_02Mesh->SetMesh("Buliding_02");
	Buliding_02Mesh->SetWorldPosition(11700.f, 0.f, 8800.f);
	Buliding_02Mesh->SetWorldRotationY(60.f);
	Buliding_02Mesh->SetWorldScale(1.2f, 1.2f, 1.2f);
	Buliding_02 = m_Owner->CreateObject<CGameObject>("Buliding_04");
	Buliding_02Mesh = Buliding_02->CreateComponent<CStaticMeshComponent>("Buliding_04Mesh");
	Buliding_02Mesh->SetMesh("Buliding_02");
	Buliding_02Mesh->SetWorldPosition(12300.f, 0.f, 7400.f);
	Buliding_02Mesh->SetWorldScale(2.f, 2.f, 2.f);

	CGameObject* Buliding_05 = m_Owner->CreateObject<CGameObject>("Buliding_05");
	CStaticMeshComponent* Buliding_05Mesh = Buliding_05->CreateComponent<CStaticMeshComponent>("Buliding_05Mesh");
	Buliding_05Mesh->SetMesh("Buliding_05");
	Buliding_05Mesh->SetWorldPosition(10900.f, 4.f, 8000.f);

	CGameObject* krustykrab = m_Owner->CreateObject<CGameObject>("krustykrab");
	CStaticMeshComponent* krustykrabMesh = krustykrab->CreateComponent<CStaticMeshComponent>("krustykrabMesh");
	krustykrabMesh->SetMesh("krustykrab");
	krustykrabMesh->SetWorldPosition(11460, 0.f, 12100.f);
	krustykrabMesh->SetWorldRotationY(180.f);
	krustykrabMesh->SetWorldScale(0.5f, 0.5f, 0.5f);
	CColliderCube* krustykrabCube = krustykrab->CreateComponent<CColliderCube>("krustykrabCube");
	krustykrabMesh->AddChild(krustykrabCube);
	krustykrabCube->SetRelativePositionY(300.f);
	krustykrabCube->SetCubeSize(1100.f, 600.f, 700.f);
	krustykrabCube->SetCollisionProfile("Wall");

	CGameObject* krustykrab_Enter = m_Owner->CreateObject<CGameObject>("krustykrab_Enter");
	CStaticMeshComponent* krustykrab_EnterMesh = krustykrab_Enter->CreateComponent<CStaticMeshComponent>("krustykrab_EnterMesh");
	krustykrab_EnterMesh->SetMesh("krustykrab_Enter");
	krustykrab_EnterMesh->SetWorldPosition(12200.f, 0.f, 13800.f);
	krustykrab_EnterMesh->SetWorldRotationY(180.f);
	CColliderCube* krustykrab_EnterCube = krustykrab_Enter->CreateComponent<CColliderCube>("krustykrab_EnterCube");
	krustykrab_EnterMesh->AddChild(krustykrab_EnterCube);
	krustykrab_EnterCube->SetRelativePositionY(100.f);
	krustykrab_EnterCube->SetCubeSize(100.f, 200.f, 100.f);
	krustykrab_EnterCube->SetCollisionProfile("Wall");

	CGameObject* krustykrab_Shell = m_Owner->CreateObject<CGameObject>("krustykrab_Shell");
	CStaticMeshComponent* krustykrab_ShellMesh = krustykrab_Shell->CreateComponent<CStaticMeshComponent>("krustykrab_ShellMesh");
	krustykrab_ShellMesh->SetMesh("krustykrab_Shell");
	krustykrab_ShellMesh->SetWorldPosition(12300.f, 0.f, 13000.f);
	krustykrab_ShellMesh->SetWorldRotationY(180.f);
	CColliderCube* krustykrab_ShellCube = krustykrab_Shell->CreateComponent<CColliderCube>("krustykrab_ShellCube");
	krustykrab_ShellMesh->AddChild(krustykrab_ShellCube);
	krustykrab_ShellCube->SetRelativePositionY(750.f);
	krustykrab_ShellCube->SetCubeSize(100.f, 1500.f, 100.f);
	krustykrab_ShellCube->SetCollisionProfile("Wall");

	CGameObject* RedTree = m_Owner->CreateObject<CGameObject>("RedTree");
	CStaticMeshComponent* RedTreeMesh = RedTree->CreateComponent<CStaticMeshComponent>("RedTreeMesh");
	RedTreeMesh->SetMesh("RedTree");
	RedTreeMesh->SetWorldPosition(12000.f, 0.f, 12700.f);

	CGameObject* PurpleTree = m_Owner->CreateObject<CGameObject>("PurpleTree");
	CStaticMeshComponent* PurpleTreeMesh = PurpleTree->CreateComponent<CStaticMeshComponent>("PurpleTreeMesh");
	PurpleTreeMesh->SetMesh("PurpleTree");
	PurpleTreeMesh->SetWorldPosition(10900.f, 0.f, 12700.f);

	CGameObject* SM_BB_FloatSign_01 = m_Owner->CreateObject<CGameObject>("SM_BB_FloatSign_01");
	CStaticMeshComponent* SM_BB_FloatSign_01Mesh = SM_BB_FloatSign_01->CreateComponent<CStaticMeshComponent>("SM_BB_FloatSign_01Mesh");
	SM_BB_FloatSign_01Mesh->SetMesh("SM_BB_FloatSign_01");
	SM_BB_FloatSign_01Mesh->SetWorldPosition(18300.f, 0.f, 11700.f);
	SM_BB_FloatSign_01Mesh->SetWorldRotationY(135.f);
	CColliderCube* SM_BB_FloatSign_01Cube = SM_BB_FloatSign_01->CreateComponent<CColliderCube>("SM_BB_FloatSign_01Cube");
	SM_BB_FloatSign_01Mesh->AddChild(SM_BB_FloatSign_01Cube);
	SM_BB_FloatSign_01Cube->SetRelativePositionY(200.f);
	SM_BB_FloatSign_01Cube->SetCubeSize(50.f, 400.f, 50.f);
	SM_BB_FloatSign_01Cube->SetCollisionProfile("Wall");

	CGameObject* fountain = m_Owner->CreateObject<CGameObject>("fountain");
	CStaticMeshComponent* fountainMesh = fountain->CreateComponent<CStaticMeshComponent>("fountainMesh");
	fountainMesh->SetMesh("fountain");
	fountainMesh->SetWorldPosition(12150.f, 0.f, 14800.f);
	CColliderCube* fountainCube = fountain->CreateComponent<CColliderCube>("fountainCube");
	fountainMesh->AddChild(fountainCube);
	fountainCube->SetRelativePositionY(100.f);
	fountainCube->SetCubeSize(500.f, 200.f, 500.f);
	fountainCube->SetCollisionProfile("Wall");

	CGameObject* TaxiStop = m_Owner->CreateObject<CGameObject>("TaxiStop");
	CStaticMeshComponent* TaxiStopMesh = TaxiStop->CreateComponent<CStaticMeshComponent>("TaxiStopMesh");
	TaxiStopMesh->SetMesh("TaxiStop");
	TaxiStopMesh->SetWorldPosition(9500.f, 0.f, 13300.f);
	TaxiStopMesh->SetWorldRotationY(20.f);

	CGameObject* Missile = m_Owner->CreateObject<CGameObject>("Missile");
	CStaticMeshComponent* MissileMesh = Missile->CreateComponent<CStaticMeshComponent>("MissileMesh");
	MissileMesh->SetMesh("Missile");
	MissileMesh->SetWorldPosition(14470.f, 0.f, 15800.f);
	MissileMesh->SetWorldScale(3.f, 3.f, 3.f);
	CColliderCube* MissileCube = Missile->CreateComponent<CColliderCube>("MissileCube");
	MissileMesh->AddChild(MissileCube);
	MissileCube->SetRelativePositionY(150.f);
	MissileCube->SetCubeSize(200.f, 300.f, 200.f);
	MissileCube->SetCollisionProfile("Wall");

	CGameObject* InfoSign = m_Owner->CreateObject<CGameObject>("InfoSign");
	CStaticMeshComponent* InfoSignMesh = InfoSign->CreateComponent<CStaticMeshComponent>("InfoSignMesh");
	InfoSignMesh->SetMesh("InfoSign");
	InfoSignMesh->SetWorldPosition(8200.f, 0.f, 14200.f);
	InfoSignMesh->SetWorldRotationY(270.f);
	CColliderCube* InfoSignCube = InfoSign->CreateComponent<CColliderCube>("InfoSignCube");
	InfoSignMesh->AddChild(InfoSignCube);
	InfoSignCube->SetRelativePositionY(90.f);
	InfoSignCube->SetCubeSize(50.f, 180.f, 100.f);

	CGameObject* Rock = m_Owner->CreateObject<CGameObject>("Rock");
	CStaticMeshComponent* RockMesh = Rock->CreateComponent<CStaticMeshComponent>("RockMesh");
	RockMesh->SetMesh("Rock");
	RockMesh->SetWorldPosition(14500.f, 0.f, 12100.f);
	RockMesh->SetWorldScale(10.f, 10.f, 10.f);
	CColliderCube* RockCube = Rock->CreateComponent<CColliderCube>("RockCube");
	RockMesh->AddChild(RockCube);
	RockCube->SetRelativePositionY(100.f);
	RockCube->SetCubeSize(700.f, 200.f, 500.f);
	RockCube->SetCollisionProfile("Wall");

	CGameObject* Rock2 = m_Owner->CreateObject<CGameObject>("Rock2");
	CStaticMeshComponent* Rock2Mesh = Rock2->CreateComponent<CStaticMeshComponent>("Rock2Mesh");
	Rock2Mesh->SetMesh("Rock2");
	Rock2Mesh->SetWorldPosition(18900.f, 0.f, 11900.f);
	Rock2Mesh->SetWorldRotationY(180.f);
	Rock2Mesh->SetWorldScale(10.f, 10.f, 10.f);
	CColliderCube* Rock2Cube = Rock2->CreateComponent<CColliderCube>("Rock2Cube");
	Rock2Mesh->AddChild(Rock2Cube);
	Rock2Cube->SetRelativePositionY(200.f);
	Rock2Cube->SetCubeSize(400.f, 400.f, 300.f);
	Rock2Cube->SetCollisionProfile("Wall");

	CGameObject* DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree");
	CStaticMeshComponent* DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh");
	DoubleTreeMesh->SetMesh("DoubleTree");
	DoubleTreeMesh->SetWorldPosition(11300.f, 0.f, 16000.f);
	CColliderCube* DoubleTreeCube = DoubleTree->CreateComponent<CColliderCube>("DoubleTreeCube");
	DoubleTreeMesh->AddChild(DoubleTreeCube);
	DoubleTreeCube->SetRelativePositionY(200.f);
	DoubleTreeCube->SetCubeSize(150.f, 400.f, 150.f);
	DoubleTreeCube->SetCollisionProfile("Wall");
	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree1");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh1");
	DoubleTreeMesh->SetMesh("DoubleTree");
	DoubleTreeMesh->SetWorldPosition(12100.f, 0.f, 16000.f);
	DoubleTreeMesh->SetWorldRotationY(90.f);
	DoubleTreeCube = DoubleTree->CreateComponent<CColliderCube>("DoubleTreeCube1");
	DoubleTreeMesh->AddChild(DoubleTreeCube);
	DoubleTreeCube->SetRelativePositionY(200.f);
	DoubleTreeCube->SetCubeSize(150.f, 400.f, 150.f);
	DoubleTreeCube->SetCollisionProfile("Wall");

	CGameObject* CoconutTree = m_Owner->CreateObject<CGameObject>("CoconutTree");
	CStaticMeshComponent* CoconutTreeMesh = CoconutTree->CreateComponent<CStaticMeshComponent>("CoconutTreeMesh");
	CoconutTreeMesh->SetMesh("CoconutTree");
	CoconutTreeMesh->SetWorldPosition(11800.f, 0.f, 16000.f);
	CColliderCube* CoconutTreeCube = CoconutTree->CreateComponent<CColliderCube>("CoconutTreeCube");
	CoconutTreeMesh->AddChild(CoconutTreeCube);
	CoconutTreeCube->SetRelativePositionY(300.f);
	CoconutTreeCube->SetCubeSize(160.f, 600.f, 200.f);
	CoconutTreeCube->SetCollisionProfile("Wall");
	CoconutTree = m_Owner->CreateObject<CGameObject>("CoconutTree1");
	CoconutTreeMesh = CoconutTree->CreateComponent<CStaticMeshComponent>("CoconutTreeMesh1");
	CoconutTreeMesh->SetMesh("CoconutTree");
	CoconutTreeMesh->SetWorldPosition(12300.f, 0.f, 16000.f);
	CoconutTreeCube = CoconutTree->CreateComponent<CColliderCube>("CoconutTreeCube");
	CoconutTreeMesh->AddChild(CoconutTreeCube);
	CoconutTreeCube->SetRelativePositionY(300.f);
	CoconutTreeCube->SetCubeSize(160.f, 600.f, 200.f);
	CoconutTreeCube->SetCollisionProfile("Wall");

	for (int i = 0; i < 10; ++i)
	{
		int x = rand() % 3700 + 11300;
		int z = rand() % 11500 + 7500;
		CGameObject* PariticleObj = m_Owner->CreateObject<CGameObject>("PariticleObj" + std::to_string(i));
		CParticleComponent* particle = PariticleObj->CreateComponent<CParticleComponent>("ParticleComponent" + std::to_string(i));
		particle->SetParticle("GroundBubble");
		PariticleObj->SetWorldPosition((float)x, 0.f, (float)z);
	}

	//젤리피쉬
	//CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	//Player->SetWorldPosition(9000.f, 300.f, 700.f);
	//SetPlayerObject(Player);

	//CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	//CTerrainComponent* TerrainComponent= TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	//TerrainComponent->CreateTerrain(360, 672, 40.f, 40.f, TEXT("LandScape/ZellyFishField.png"));
	return true;
}
