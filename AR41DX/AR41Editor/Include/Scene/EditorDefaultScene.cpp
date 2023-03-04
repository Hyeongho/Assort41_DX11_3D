
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/StartSceneUI.h"
//#include "../UI/LoadingUI.h"
#include "../UI/TitleSceneUI.h"
#include "../GameObject/Player.h"
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

	//m_Owner->GetViewport()->CreateUIWindow<CStartSceneUI>("StartUI");
	//m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");

	//title
	//m_Owner->GetViewport()->CreateUIWindow<CTitleSceneUI>("TitleSceneUI");

	//ºñÅ°´Ï½ÃÆ¼ ¸Ê
	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	Player->SetWorldPosition(10000.f, 0.f, 10000.f);
	SetPlayerObject(Player);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");

	CGameObject* Road = m_Owner->CreateObject<CGameObject>("Road");
	CStaticMeshComponent* RoadMesh = Road->CreateComponent<CStaticMeshComponent>("RoadMesh");
	RoadMesh->SetMesh("BikiniBottomRoad");
	RoadMesh->SetWorldScale(0.3f, 0.3f, 0.3f);
	RoadMesh->SetWorldPosition(12000.f, -40.f, 13250.f);

	//CGameObject* PineAppleHouse = m_Owner->CreateObject<CGameObject>("PineAppleHouse");
	//CStaticMeshComponent* PineAppleHouseMesh = PineAppleHouse->CreateComponent<CStaticMeshComponent>("PineAppleHouseMesh");
	//PineAppleHouseMesh->SetMesh("PineAppleHouse");
	//PineAppleHouseMesh->SetWorldScale(1.f, 1.f, 1.f);
	//PineAppleHouseMesh->SetWorldPosition(8650.f, 0.f, 9700.f);
	//CColliderCube* PineAppleHouseCube = PineAppleHouse->CreateComponent<CColliderCube>("PineAppleHouseCube");
	//PineAppleHouseMesh->AddChild(PineAppleHouseCube);
	//PineAppleHouseCube->SetCubeSize(500.f, 500.f, 500.f);
	//PineAppleHouseCube->SetCollisionProfile("Wall");

	CGameObject* PatrickHouse = m_Owner->CreateObject<CGameObject>("PatrickHouse");
	CStaticMeshComponent* PatrickHouseMesh = PatrickHouse->CreateComponent<CStaticMeshComponent>("PatrickHouseMesh");
	PatrickHouseMesh->SetMesh("PatrickHouse");
	PatrickHouseMesh->SetWorldScale(1.f, 1.f, 1.f);
	PatrickHouseMesh->SetWorldPosition(14300.f, 500.f, 14500.f);
	CColliderCube* PatrickHouseCube = PatrickHouse->CreateComponent<CColliderCube>("PatrickHouseCube");
	PatrickHouseMesh->AddChild(PatrickHouseCube); 
	PatrickHouseCube->SetCubeSize(500.f, 500.f, 500.f);
	PatrickHouseCube->SetCollisionProfile("Wall");

	CGameObject* SquidHouse = m_Owner->CreateObject<CGameObject>("SquidHouse");
	CStaticMeshComponent* SquidHouseMesh = SquidHouse->CreateComponent<CStaticMeshComponent>("SquidHouseMesh");
	SquidHouseMesh->SetMesh("SquidHouse");
	SquidHouseMesh->SetWorldScale(1.f, 1.f, 1.f);
	SquidHouseMesh->SetWorldPosition(15900.f, 4.f, 14550.f);
	CColliderCube* SquidHouseCube = SquidHouse->CreateComponent<CColliderCube>("SquidHouseCube");
	SquidHouseMesh->AddChild(SquidHouseCube);
	SquidHouseCube->SetCubeSize(600.f, 1000.f, 600.f);
	SquidHouseCube->SetCollisionProfile("Wall");

	CGameObject* ChumBucket = m_Owner->CreateObject<CGameObject>("ChumBucket");
	CStaticMeshComponent* ChumBucketMesh = ChumBucket->CreateComponent<CStaticMeshComponent>("ChumBucketMesh");
	ChumBucketMesh->SetMesh("ChumBucket");
	ChumBucketMesh->SetWorldScale(1.f, 1.f, 1.f);
	ChumBucketMesh->SetWorldPosition(6800.f, 4.f, 13800.f);
	CColliderCube* ChumBucketCube = ChumBucket->CreateComponent<CColliderCube>("ChumBucketCube");
	ChumBucketMesh->AddChild(ChumBucketCube);
	ChumBucketCube->SetCubeSize(500.f, 500.f, 500.f);
	ChumBucketCube->SetCollisionProfile("Wall");

	//CGameObject* PariticleObj = m_Owner->CreateObject<CGameObject>("PariticleObj");
	//CParticleComponent* particle = PariticleObj->CreateComponent<CParticleComponent>("ParticleComponent");

	//Á©¸®ÇÇ½¬
	return true;
}
