#include "JellyfishField2SceneInfo.h"

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

CJellyfishField2SceneInfo::CJellyfishField2SceneInfo()
{
    m_ClassTypeName = "JellyfishField1SceneInfo";
}

CJellyfishField2SceneInfo::~CJellyfishField2SceneInfo()
{
}

bool CJellyfishField2SceneInfo::Init()
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
	Player->SetRespawnPos(8100.f, 15000.f, 21900.f);
	SetPlayerObject(Player);



	CGameObject* Map = m_Owner->CreateObject<CGameObject>("Map");
	CStaticMeshComponent* StaticMeshComponent = Map->CreateComponent<CStaticMeshComponent>("Mesh");
	StaticMeshComponent->SetMesh("JellyFishFieldTestKKB");
	Map->SetWorldPosition(7000.f, 3500.f, 21000.f);

	// Terrain Obj
	{
		CGameObject* Ground = m_Owner->CreateObject<CGameObject>("Ground1");
		CColliderOBB3D* OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(9800.f, 4850.f, 23100.f);
		OBB->SetWorldRotationY(310.f);
		OBB->SetBoxHalfSize(400.f, 10.f, 400.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground2");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(10300.f, 5000.f, 24300.f);
		OBB->SetBoxHalfSize(500.f, 10.f, 1000.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground3");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(10400.f, 5150.f, 25900.f);
		OBB->SetWorldRotationY(30.f);
		OBB->SetBoxHalfSize(1200.f, 10.f, 1000.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground4");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(12300.f, 5650.f, 26300.f);
		OBB->SetWorldRotationY(70.f);
		OBB->SetBoxHalfSize(800.f, 10.f, 800.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground5");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(6200.f, 4900.f, 20800.f);
		OBB->SetWorldRotationY(10.f);
		OBB->SetBoxHalfSize(400.f, 10.f, 400.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground6");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(5400.f, 4700.f, 20800.f);
		OBB->SetWorldRotationY(10.f);
		OBB->SetBoxHalfSize(400.f, 10.f, 400.f);
		OBB->SetCollisionProfile("Ground");

		Ground = m_Owner->CreateObject<CGameObject>("Ground7");
		OBB = Ground->CreateComponent<CColliderOBB3D>("OBB3D");
		OBB->SetWorldPosition(4600.f, 4600.f, 21200.f);
		OBB->SetWorldRotationY(335.f);
		OBB->SetBoxHalfSize(1000.f, 10.f, 700.f);
		OBB->SetCollisionProfile("Ground");
	}

	// Object
	{
		CTeleportBox* TeleportBox1 = m_Owner->CreateObject<CTeleportBox>("TeleportBox");
		TeleportBox1->SetBoxIndex(2);
		TeleportBox1->SetWorldScale(1.f, 1.f, 1.f);
		TeleportBox1->SetWorldPosition(4000.f, 4600.f, 21100.f);

	}


    return true;
}
