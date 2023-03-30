#include "JellyfishField1SceneInfo.h"
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
#include "Component/ColliderOBB3D.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"

CJellyfishField1SceneInfo::CJellyfishField1SceneInfo()
{
    m_ClassTypeName = "JellyfishField1SceneInfo";
}

CJellyfishField1SceneInfo::~CJellyfishField1SceneInfo()
{
}

bool CJellyfishField1SceneInfo::Init()
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
	Player->SetRespawnPos(14000.f, 5000.f, 1600.f);
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
	WaterMesh->SetWorldScale(3.f, 1.f, 3.f);
	WaterMesh->SetWorldPosition(14000.f, 1350.f, 81000.f);

	CGameObject* JellyfishField = m_Owner->CreateObject<CGameObject>("JellyfishField1");
	CStaticMeshComponent* JellyfishFieldMesh = JellyfishField->CreateComponent<CStaticMeshComponent>("JellyfishField1");
	JellyfishFieldMesh->SetMesh("JellyfishField1");
	JellyfishFieldMesh->SetWorldScale(1.f, 1.f, 1.f);
	JellyfishFieldMesh->SetWorldPosition(7000.f, 3500.f, 21000.f);

	CGameObject* Platform1 = m_Owner->CreateObject<CGameObject>("Platform1");
	CColliderOBB3D* OBB1 = Platform1->CreateComponent<CColliderOBB3D>("OBB1");
	OBB1->SetBoxHalfSize(1000.f, 10.f, 1000.f);
	OBB1->SetWorldPosition(14000.f, 2100.f, 1600.f);
	OBB1->SetCollisionProfile("Ground");

	CGameObject* Platform2 = m_Owner->CreateObject<CGameObject>("Platform2");
	CColliderOBB3D* OBB2 = Platform2->CreateComponent<CColliderOBB3D>("OBB2");
	OBB2->SetBoxHalfSize(2000.f, 10.f, 2000.f);
	OBB2->SetWorldPosition(14000.f, 1700.f, 4600.f);
	OBB2->SetCollisionProfile("Ground");

	CGameObject* Platform3 = m_Owner->CreateObject<CGameObject>("Platform3");
	CColliderOBB3D* OBB3 = Platform3->CreateComponent<CColliderOBB3D>("OBB3");
	OBB3->SetBoxHalfSize(1000.f, 10.f, 2000.f);
	OBB3->SetWorldPosition(14000.f, 1700.f, 8000.f);
	OBB3->SetWorldRotation(0.f, 45.f, 0.f);
	OBB3->SetCollisionProfile("Ground");

	CGameObject* Platform4 = m_Owner->CreateObject<CGameObject>("Platform4");
	CColliderOBB3D* OBB4 = Platform4->CreateComponent<CColliderOBB3D>("OBB4");
	OBB4->SetBoxHalfSize(2000.f, 10.f, 6000.f);
	OBB4->SetWorldPosition(8000.f, 1700.f, 12000.f);
	OBB3->SetWorldRotation(0.f, 60.f, 0.f);
	OBB4->SetCollisionProfile("Ground");

	return true;
}
