#include "BikiniCitySceneInfo.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"
#include "../GameObject/Player.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/Patric.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/BusDriver.h"
#include "../GameObject/Npc/InfoSign.h"
#include "../GameObject/Object/Common/InteractButton.h"
#include "../GameObject/Object/BB/BusStop.h"
#include "../GameObject/Tikis/Tiki_Stone.h"
#include "../GameObject/Tikis/Tiki_Thunder.h"
#include "../GameObject/Tikis/Tiki_Wood.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "../UI/Fade.h"
//#include "GameObject/SkySphere.h"

CBikiniCitySceneInfo::CBikiniCitySceneInfo()
{
	m_ClassTypeName = "BikiniCitySceneInfo";
}

CBikiniCitySceneInfo::~CBikiniCitySceneInfo()
{
}

void CBikiniCitySceneInfo::Start()
{
	//m_Owner->SetSkyMaterial("Sky2");

	m_Fade->SetState(EFade_State::FadeIn_Start);
}

bool CBikiniCitySceneInfo::Init()
{
	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(45.f, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CResourceManager::GetInst()->SoundPlay("BikiniBottom");
	CResourceManager::GetInst()->SetVolume(30.f);

	m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	//비키니시티 맵
	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetRespawnPos(16500.f, 0.f, 12200.f);
	SetPlayerObject(Player);

	CTiki_Stone* Tiki_Stone = m_Owner->CreateObject<CTiki_Stone>("Tiki_Stone");
	Tiki_Stone->SetWorldPosition(9500.f, 0.f, 15000.f);

	CTiki_Thunder* Tiki_Thunder = m_Owner->CreateObject<CTiki_Thunder>("Tiki_Thunder");
	Tiki_Thunder->SetWorldPosition(10000.f, 0.f, 14600.f);

	CTiki_Wood* Tiki_Wood = m_Owner->CreateObject<CTiki_Wood>("Tiki_Wood");
	Tiki_Wood->SetWorldPosition(9300.f, 0.f, 14500.f);

	Tiki_Wood = m_Owner->CreateObject<CTiki_Wood>("Tiki_Wood1");
	Tiki_Wood->SetWorldPosition(14569.97f, 0.f, 13102.94f);


	CPatric* Patric = m_Owner->CreateObject<CPatric>("Patric");
	Patric->SetWorldPosition(15000.f, 0.f, 14000.f);

	CMrKrabs* MrKrabs = m_Owner->CreateObject<CMrKrabs>("MrKrabs");
	MrKrabs->SetWorldPosition(11000.f, 0.f, 13000.f);
	MrKrabs->SetWorldRotationY(180.f);

	CSquidward* Squidward = m_Owner->CreateObject<CSquidward>("Squidward");
	Squidward->SetWorldPosition(15600, 0.f, 13400.f);

	CBusStop* BusStop = m_Owner->CreateObject<CBusStop>("BusStop");
	BusStop->SetWorldPosition(9200.f, 0.f, 13400.f);

	CGameObject* Jellyfishfieldsign = m_Owner->CreateObject<CGameObject>("Jellyfishfieldsign");
	CStaticMeshComponent* JellyfishfieldsignMesh = Jellyfishfieldsign->CreateComponent<CStaticMeshComponent>("JellyfishfieldsignMesh");
	JellyfishfieldsignMesh->SetMesh("Jellyfishfieldsign");
	//JellyfishfieldsignMesh->SetWorldScale(0.3f, 0.f, 0.3f);
	JellyfishfieldsignMesh->SetWorldPosition(9949.63f, 0.f, 12793.74f);
	JellyfishfieldsignMesh->SetWorldRotationY(-45.f);

	CInteractButton* InteractButton = m_Owner->CreateObject<CInteractButton>("InteractButton");
	InteractButton->SetWorldPosition(9510.f, 0.f, 13450.f);
	InteractButton->SetInteractObject(EInteractObjectList::BusStop, "BusStop");
	InteractButton->SetDir(EButtonDir::Virtical);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(680, 631, 40.f, 40.f, TEXT("LandScape/BikiniCity_Height.png"));
	TerrainComponent->GetMaterial(0)->SetOpacity(0.f);

	/*CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(1292, 1382, 40.f, 40.f, TEXT("LandScape/BikiniBottom_WT.png"));
	TerrainObj->SetWorldScale(0.3f, 0.f, 0.3f);
	TerrainObj->SetWorldPosition(20000.f, -150.f, 4900.f);
	TerrainObj->SetWorldRotationY(-90.f);*/
	//TerrainComponent->CreateTerrain(680, 631, 40.f, 40.f, TEXT("LandScape/BikiniCity_Height.png"));

	CGameObject* BBObj = m_Owner->CreateObject<CGameObject>("BBObj");
	CStaticMeshComponent* BBMesh = BBObj->CreateComponent<CStaticMeshComponent>("BBMesh");
	BBMesh->SetMesh("BikiniBottomMesh");
	BBMesh->SetWorldScale(0.3f, 0.f, 0.3f);
	BBMesh->SetWorldPosition(12000.f, 10.f, 13250.f);

	CGameObject* PatrickHouse = m_Owner->CreateObject<CGameObject>("PatrickHouse");
	CStaticMeshComponent* PatrickHouseMesh = PatrickHouse->CreateComponent<CStaticMeshComponent>("PatrickHouseMesh");
	PatrickHouseMesh->SetMesh("PatrickHouse");
	PatrickHouseMesh->SetWorldPosition(14300.f, 500.f, 14500.f);
	CColliderOBB3D* PatrickHouseCube = PatrickHouse->CreateComponent<CColliderOBB3D>("PatrickHouseCube");
	PatrickHouseMesh->AddChild(PatrickHouseCube);
	PatrickHouseCube->SetBoxHalfSize(PatrickHouseMesh->GetMeshSize() * 0.5f);
	PatrickHouseCube->SetRelativePositionY(PatrickHouseMesh->GetMeshSize().y * -0.25f);
	PatrickHouseCube->SetCollisionProfile("Wall");

	CGameObject* SquidHouse = m_Owner->CreateObject<CGameObject>("SquidHouse");
	CStaticMeshComponent* SquidHouseMesh = SquidHouse->CreateComponent<CStaticMeshComponent>("SquidHouseMesh");
	SquidHouseMesh->SetMesh("SquidHouse");
	SquidHouseMesh->SetWorldPosition(15900.f, 4.f, 14550.f);
	CColliderOBB3D* SquidHouseCube = SquidHouse->CreateComponent<CColliderOBB3D>("SquidHouseCube");
	SquidHouseMesh->AddChild(SquidHouseCube);
	SquidHouseCube->SetBoxHalfSize(SquidHouseMesh->GetMeshSize() * 0.5f);
	SquidHouseCube->SetRelativePositionY(SquidHouseMesh->GetMeshSize().y * 0.5f);
	SquidHouseCube->SetCollisionProfile("Wall");

	CGameObject* PineAppleHouse = m_Owner->CreateObject<CGameObject>("PineAppleHouse");
	CStaticMeshComponent* PineAppleHouseMesh = PineAppleHouse->CreateComponent<CStaticMeshComponent>("PineAppleHouseMesh");
	PineAppleHouseMesh->SetMesh("PineAppleHouse");
	PineAppleHouseMesh->SetWorldPosition(17000.f, 0.f, 13500.f);
	CColliderOBB3D* PineAppleHouseCube = PineAppleHouse->CreateComponent<CColliderOBB3D>("PineAppleHouseCube");
	PineAppleHouseMesh->AddChild(PineAppleHouseCube);
	PineAppleHouseCube->SetBoxHalfSize(PineAppleHouseMesh->GetMeshSize() * 0.5f);
	PineAppleHouseCube->SetRelativePositionY(PineAppleHouseMesh->GetMeshSize().y * 0.5f);
	PineAppleHouseCube->SetCollisionProfile("Wall");

	CGameObject* Seaflower = m_Owner->CreateObject<CGameObject>("Seaflower");
	CStaticMeshComponent* SeaflowerMesh = Seaflower->CreateComponent<CStaticMeshComponent>("SeaflowerMesh");
	SeaflowerMesh->SetMesh("Seaflower");
	SeaflowerMesh->SetWorldPosition(17100.f, 0.f, 13400.f);

	CGameObject* ChumBucket = m_Owner->CreateObject<CGameObject>("ChumBucket");
	CStaticMeshComponent* ChumBucketMesh = ChumBucket->CreateComponent<CStaticMeshComponent>("ChumBucketMesh");
	ChumBucketMesh->SetMesh("ChumBucket");
	ChumBucketMesh->SetWorldPosition(6800.f, 4.f, 13800.f);
	ChumBucketMesh->SetWorldRotationY(180.f);
	CColliderOBB3D* ChumBucketCube = ChumBucket->CreateComponent<CColliderOBB3D>("ChumBucketCube");
	ChumBucketMesh->AddChild(ChumBucketCube);
	ChumBucketCube->SetBoxHalfSize(ChumBucketMesh->GetMeshSize() * 0.5f);
	ChumBucketCube->SetRelativePositionY(ChumBucketMesh->GetMeshSize().y * 0.5f);
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
	CColliderOBB3D* krustykrabCube = krustykrab->CreateComponent<CColliderOBB3D>("krustykrabCube");
	krustykrabMesh->AddChild(krustykrabCube);
	Vector3 size = krustykrabMesh->GetMeshSize() * krustykrabMesh ->GetWorldScale()* 0.5f;
	krustykrabCube->SetBoxHalfSize(size);
	krustykrabCube->SetRelativePositionY(size.y);
	krustykrabCube->SetCollisionProfile("Wall");

	CGameObject* krustykrab_Enter = m_Owner->CreateObject<CGameObject>("krustykrab_Enter");
	CStaticMeshComponent* krustykrab_EnterMesh = krustykrab_Enter->CreateComponent<CStaticMeshComponent>("krustykrab_EnterMesh");
	krustykrab_EnterMesh->SetMesh("krustykrab_Enter");
	krustykrab_EnterMesh->SetWorldPosition(12200.f, 0.f, 13800.f);
	krustykrab_EnterMesh->SetWorldRotationY(180.f);
	CColliderOBB3D* krustykrab_EnterCube = krustykrab_Enter->CreateComponent<CColliderOBB3D>("krustykrab_EnterCube");
	krustykrab_EnterMesh->AddChild(krustykrab_EnterCube);
	krustykrab_EnterCube->SetBoxHalfSize(krustykrab_EnterMesh->GetMeshSize() * 0.5f);
	krustykrab_EnterCube->SetRelativePositionY(krustykrab_EnterMesh->GetMeshSize().y * 0.5f);
	krustykrab_EnterCube->SetCollisionProfile("Wall");

	CGameObject* krustykrab_Shell = m_Owner->CreateObject<CGameObject>("krustykrab_Shell");
	CStaticMeshComponent* krustykrab_ShellMesh = krustykrab_Shell->CreateComponent<CStaticMeshComponent>("krustykrab_ShellMesh");
	krustykrab_ShellMesh->SetMesh("krustykrab_Shell");
	krustykrab_ShellMesh->SetWorldPosition(12300.f, 0.f, 13000.f);
	krustykrab_ShellMesh->SetWorldRotationY(180.f);

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
	CColliderOBB3D* SM_BB_FloatSign_01Cube = SM_BB_FloatSign_01->CreateComponent<CColliderOBB3D>("SM_BB_FloatSign_01Cube");
	SM_BB_FloatSign_01Mesh->AddChild(SM_BB_FloatSign_01Cube);
	SM_BB_FloatSign_01Cube->SetBoxHalfSize(SM_BB_FloatSign_01Mesh->GetMeshSize() * 0.5f);
	SM_BB_FloatSign_01Cube->SetRelativePositionY(SM_BB_FloatSign_01Mesh->GetMeshSize().y * 0.5f);
	SM_BB_FloatSign_01Cube->SetCollisionProfile("Wall");

	CGameObject* fountain = m_Owner->CreateObject<CGameObject>("fountain");
	CStaticMeshComponent* fountainMesh = fountain->CreateComponent<CStaticMeshComponent>("fountainMesh");
	fountainMesh->SetMesh("fountain");
	fountainMesh->SetWorldPosition(12150.f, 0.f, 14800.f);
	CColliderOBB3D* fountainCube = fountain->CreateComponent<CColliderOBB3D>("fountainCube");
	fountainMesh->AddChild(fountainCube);
	fountainCube->SetBoxHalfSize(fountainMesh->GetMeshSize() * 0.5f);
	fountainCube->SetRelativePositionY(fountainMesh->GetMeshSize().y * -0.17f);
	fountainCube->SetCollisionProfile("Wall");

	CGameObject* Missile = m_Owner->CreateObject<CGameObject>("Missile");
	CStaticMeshComponent* MissileMesh = Missile->CreateComponent<CStaticMeshComponent>("MissileMesh");
	MissileMesh->SetMesh("Missile");
	MissileMesh->SetWorldPosition(13316.08f, 0.f, 14804.86f);
	MissileMesh->SetWorldScale(3.f, 3.f, 3.f);
	CColliderOBB3D* MissileCube = Missile->CreateComponent<CColliderOBB3D>("MissileCube");
	MissileMesh->AddChild(MissileCube);
	size = MissileMesh->GetMeshSize() * MissileMesh->GetWorldScale() * 0.5f;
	MissileCube->SetBoxHalfSize(size);
	MissileCube->SetRelativePositionY(size.y);
	MissileCube->SetCollisionProfile("Wall");

	CInfoSign* InfoSign=m_Owner->CreateObject<CInfoSign>("InfoSign");
	InfoSign->SetWorldPosition(8200.f, 0.f, 14200.f);
	InfoSign->SetWorldRotationY(270.f);

	CGameObject* Rock = m_Owner->CreateObject<CGameObject>("Rock");
	CStaticMeshComponent* RockMesh = Rock->CreateComponent<CStaticMeshComponent>("RockMesh");
	RockMesh->SetMesh("Rock");
	RockMesh->SetWorldPosition(14500.f, 0.f, 12100.f);
	RockMesh->SetWorldScale(10.f, 10.f, 10.f);
	CColliderOBB3D* RockCube = Rock->CreateComponent<CColliderOBB3D>("RockCube");
	RockMesh->AddChild(RockCube);
	size = RockMesh->GetMeshSize() * RockMesh->GetWorldScale() * 0.5f;
	RockCube->SetBoxHalfSize(size);
	RockCube->SetRelativePositionY(size.y);
	RockCube->SetCollisionProfile("Wall");

	CGameObject* Rock2 = m_Owner->CreateObject<CGameObject>("Rock2");
	CStaticMeshComponent* Rock2Mesh = Rock2->CreateComponent<CStaticMeshComponent>("Rock2Mesh");
	Rock2Mesh->SetMesh("Rock2");
	Rock2Mesh->SetWorldPosition(18900.f, 0.f, 11900.f);
	Rock2Mesh->SetWorldRotationY(180.f);
	Rock2Mesh->SetWorldScale(10.f, 10.f, 10.f);
	CColliderOBB3D* Rock2Cube = Rock2->CreateComponent<CColliderOBB3D>("Rock2Cube");
	Rock2Mesh->AddChild(Rock2Cube);
	size = Rock2Mesh->GetMeshSize() * Rock2Mesh->GetWorldScale() * 0.5f;
	Rock2Cube->SetBoxHalfSize(size);
	Rock2Cube->SetRelativePositionY(size.y);
	Rock2Cube->SetCollisionProfile("Wall");

	CGameObject* DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree");
	CStaticMeshComponent* DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh");
	DoubleTreeMesh->SetMesh("DoubleTree");
	DoubleTreeMesh->SetWorldPosition(11300.f, 0.f, 16000.f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree1");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh1");
	DoubleTreeMesh->SetMesh("DoubleTree_Purple");
	DoubleTreeMesh->SetWorldPosition(12100.f, 0.f, 16300.f);
	DoubleTreeMesh->SetWorldRotationY(90.f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);


	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree2");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh2");
	DoubleTreeMesh->SetMesh("DoubleTree_Yellow");
	DoubleTreeMesh->SetWorldPosition(9615.98f, 0.f, 15570.54f);
	DoubleTreeMesh->SetWorldRotationY(90.f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree3");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh3");
	DoubleTreeMesh->SetMesh("DoubleTree_Yellow");
	DoubleTreeMesh->SetWorldPosition(13975.61f, 0.f, 12794.09f);
	DoubleTreeMesh->SetWorldRotationY(90.f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree4");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh4");
	DoubleTreeMesh->SetMesh("Seaflower");
	DoubleTreeMesh->SetWorldPosition(13845.21f, 0.f, 12946.34f);
	DoubleTreeMesh->SetWorldRotationY(90.f);
	DoubleTree->SetWorldScale(1.0f, 1.0f, 1.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree5");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh5");
	DoubleTreeMesh->SetMesh("DoubleTree_Purple");
	DoubleTreeMesh->SetWorldPosition(17071.30f, 0.f, 11940.25f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree6");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh6");
	DoubleTreeMesh->SetMesh("DoubleTree_Red");
	DoubleTreeMesh->SetWorldPosition(10118.15f, 0.f, 12134.60f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree6");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh6");
	DoubleTreeMesh->SetMesh("DoubleTree_Red");
	DoubleTreeMesh->SetWorldPosition(10118.15f, 0.f, 12134.60f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree7");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh7");
	DoubleTreeMesh->SetMesh("SmallTree_Purple");
	DoubleTreeMesh->SetWorldPosition(13753.74f, 0.f, 13433.06f);
	DoubleTree->SetWorldScale(2.0f, 2.0f, 2.0f);

	DoubleTree = m_Owner->CreateObject<CGameObject>("DoubleTree8");
	DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh8");
	DoubleTreeMesh->SetMesh("Seaflower");
	DoubleTreeMesh->SetWorldPosition(17098.46f, 0.f, 12077.07f);
	DoubleTree->SetWorldScale(1.0f, 1.0f, 1.0f);

	CGameObject* SmallTree = m_Owner->CreateObject<CGameObject>("SmallTree");
	CStaticMeshComponent* SmallTreeMesh = SmallTree->CreateComponent<CStaticMeshComponent>("SmallTreeMesh");
	SmallTreeMesh->SetMesh("SmallTree_Red");
	SmallTreeMesh->SetWorldPosition(11300.f, 0.f, 16000.f);
	SmallTree->SetWorldScale(1.0f, 1.0f, 1.0f);
	
	SmallTree = m_Owner->CreateObject<CGameObject>("SmallTree1");
	SmallTreeMesh = SmallTree->CreateComponent<CStaticMeshComponent>("SmallTreeMesh1");
	SmallTreeMesh->SetMesh("SmallTree_Purple");
	SmallTreeMesh->SetWorldPosition(15382.15f, 0.f, 11727.04f);
	SmallTree->SetWorldScale(1.0f, 1.0f, 1.0f);

	SmallTree = m_Owner->CreateObject<CGameObject>("SmallTree1");
	SmallTreeMesh = SmallTree->CreateComponent<CStaticMeshComponent>("SmallTreeMesh1");
	SmallTreeMesh->SetMesh("SmallTree_Red");
	SmallTreeMesh->SetWorldPosition(8122.58f, 0.f, 14704.44f);
	SmallTree->SetWorldScale(2.2f, 2.2f, 2.2f);

	CGameObject* RedTree1 = m_Owner->CreateObject<CGameObject>("RedTree1");
	CStaticMeshComponent* RedTreeMesh1 = RedTree1->CreateComponent<CStaticMeshComponent>("RedTreeMesh1");
	RedTreeMesh1->SetMesh("RedTree");
	RedTreeMesh1->SetWorldPosition(12996.68f, 0.f, 13110.89f);


	CGameObject* CoconutTree = m_Owner->CreateObject<CGameObject>("CoconutTree");
	CStaticMeshComponent* CoconutTreeMesh = CoconutTree->CreateComponent<CStaticMeshComponent>("CoconutTreeMesh");
	CoconutTreeMesh->SetMesh("CoconutTree");
	CoconutTreeMesh->SetWorldPosition(12666.12f, 0.f, 15394.38f);

	CoconutTree = m_Owner->CreateObject<CGameObject>("CoconutTree1");
	CoconutTreeMesh = CoconutTree->CreateComponent<CStaticMeshComponent>("CoconutTreeMesh1");
	CoconutTreeMesh->SetMesh("CoconutTree");
	CoconutTreeMesh->SetWorldPosition(12996.68f, 0.f, 13110.89f);

	for (int i = 0; i < 10; ++i)
	{
		int x = rand() % 3700 + 11300;
		int z = rand() % 11500 + 7500;
		CGameObject* PariticleObj = m_Owner->CreateObject<CGameObject>("PariticleObj" + std::to_string(i));
		CParticleComponent* particle = PariticleObj->CreateComponent<CParticleComponent>("ParticleComponent" + std::to_string(i));
		particle->SetParticle("GroundBubble");
		PariticleObj->SetWorldPosition((float)x, 0.f, (float)z);
	}

	CGameObject* Mountain = m_Owner->CreateObject<CGameObject>("Mountain");
	CStaticMeshComponent* MountainMesh = Mountain->CreateComponent<CStaticMeshComponent>("MountainMesh");
	MountainMesh->SetMesh("Mountain");
	MountainMesh->SetWorldPosition(7081.32f, 0.f, 8064.23f);
	MountainMesh->SetWorldScale(0.3f, 0.3f, 0.3f);

	CGameObject* SandCastle = m_Owner->CreateObject<CGameObject>("SandCastle");
	CStaticMeshComponent* SandCastleMesh = Mountain->CreateComponent<CStaticMeshComponent>("SandCastleMesh");
	SandCastleMesh->SetMesh("SandCastle");
	SandCastleMesh->SetWorldPosition(6674.17f, 0.f, 17409.38f);
	SandCastleMesh->SetWorldScale(1.f, 1.f, 1.f);

	CGameObject* DutchmanObj = m_Owner->CreateObject<CGameObject>("DutchmanObj");
	CStaticMeshComponent* DutchmanObjMesh = DutchmanObj->CreateComponent<CStaticMeshComponent>("DutchmanObjMesh");
	DutchmanObjMesh->SetMesh("Dutchman");
	DutchmanObjMesh->SetWorldScale(0.1f, 0.1f, 0.1f);
	DutchmanObjMesh->SetWorldPosition(17257.03f, 0.f, 6439.80f);

	m_Fade = m_Owner->GetViewport()->CreateUIWindow<CFade>("Fade");

	return true;
}