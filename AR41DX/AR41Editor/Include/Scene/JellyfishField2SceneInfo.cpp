#include "JellyfishField2SceneInfo.h"

#include "Scene/Scene.h"
#include "../GameObject/Player.h"
#include "../GameObject/Monster.h"
#include "../GameObject/Jellyfish.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Object/Common/Trampoline.h"
#include "../GameObject/Hammer.h"
#include "../GameObject/Cannon.h"
#include "../GameObject/Duplicatotron.h"
#include "../GameObject/TeeterRock.h"
#include "../GameObject/Object/Gate.h"
#include "../GameObject/Object/JumpTree.h"
#include "../GameObject/Object/IceCube.h"
#include "../GameObject/Object/CheckPoint.h"
#include "../GameObject/Object/TeleportBox.h"
#include "../GameObject/Object/JFF/Grass.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/TaxiDriver.h"
#include "../GameObject/Npc/InfoSign.h"
#include "../GameObject/Tikis/Tiki_Wood.h"
#include "../GameObject/Tikis/Tiki_Stone.h"
#include "../GameObject/Tikis/Tiki_Thunder.h"
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

CJellyfishField2SceneInfo::CJellyfishField2SceneInfo()
{
    m_ClassTypeName = "JellyfishField2SceneInfo";
}

CJellyfishField2SceneInfo::~CJellyfishField2SceneInfo()
{
}

void CJellyfishField2SceneInfo::Start()
{
	m_Fade->SetState(EFade_State::FadeIn_Start);
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
	Player->SetRespawnPos(3700.f, 15000.f, 21300.f);
	SetPlayerObject(Player);



	CGameObject* Map = m_Owner->CreateObject<CGameObject>("Map");
	CStaticMeshComponent* StaticMeshComponent = Map->CreateComponent<CStaticMeshComponent>("Mesh");
	StaticMeshComponent->SetMesh("JellyFishFieldTestKKB");
	Map->SetWorldPosition(7000.f, 3500.f, 21000.f);

	// ±è±â¹é(Ground Collider Obj)
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

	// ±è±â¹é(Monster)
	{
		CTiki_Stone* TikiStone = m_Owner->CreateObject<CTiki_Stone>("TikiStone");
		TikiStone->SetWorldPosition(7000.f, 3500.f, 21000.f);


	}


	// ±è±â¹é(Object)
	{
		CTeleportBox* TeleportBox1 = m_Owner->CreateObject<CTeleportBox>("TeleportBox");
		TeleportBox1->SetBoxIndex(2);
		TeleportBox1->SetWorldScale(1.f, 1.f, 1.f);
		TeleportBox1->SetWorldPosition(4000.f, 4600.f, 21100.f);

		CTrampoline* Trampoline = m_Owner->CreateObject<CTrampoline>("Trampoline");
		Trampoline->SetWorldPosition(11100.f, 5150.f, 26300.f);

		CTeeterRock* TeeterRock = m_Owner->CreateObject<CTeeterRock>("TeeterRock");
		TeeterRock->SetWorldPosition(8100.f, 4500.f, 21900.f);
		TeeterRock->SetWorldRotationY(320.f);

		CGate* Gate = m_Owner->CreateObject<CGate>("Gate");
		Gate->SetWorldPosition(8100.f, 4500.f, 21900.f);
	}

	// ±è±â¹é(Tree)
	{
		CGameObject* RedTree = m_Owner->CreateObject<CGameObject>("Stage6_RedTree1");
		CStaticMeshComponent* RedTreeMesh = RedTree->CreateComponent<CStaticMeshComponent>("RedTreeMesh");
		RedTreeMesh->SetMesh("RedTree");
		RedTreeMesh->SetWorldPosition(10700.f, 5000.f, 24400.f);
		RedTreeMesh->SetWorldScale(2.f, 2.f, 2.f);

		CGameObject* PurpleTree = m_Owner->CreateObject<CGameObject>("Stage6_PurpleTree1");
		CStaticMeshComponent* PurpleTreeMesh = PurpleTree->CreateComponent<CStaticMeshComponent>("PurpleTreeMesh");
		PurpleTreeMesh->SetMesh("PurpleTree");
		PurpleTreeMesh->SetWorldPosition(10750.f, 5000.f, 24100.f);
		PurpleTreeMesh->SetWorldScale(3.f, 3.f, 3.f);

		CGameObject* DoubleTree = m_Owner->CreateObject<CGameObject>("Stage6_DoubleTree1");
		CStaticMeshComponent* DoubleTreeMesh = DoubleTree->CreateComponent<CStaticMeshComponent>("DoubleTreeMesh");
		DoubleTreeMesh->SetMesh("DoubleTree");
		DoubleTreeMesh->SetWorldPosition(10000.f, 5150.f, 26450.f);
		DoubleTreeMesh->SetWorldRotationY(90.f);
		DoubleTreeMesh->SetWorldScale(2.f, 2.f, 2.f);

		CGameObject* DoubleTree_Y = m_Owner->CreateObject<CGameObject>("Stage6_DoubleTree_Y1");
		CStaticMeshComponent* DoubleTree_YMesh = DoubleTree_Y->CreateComponent<CStaticMeshComponent>("DoubleTree_Y");
		DoubleTree_YMesh->SetMesh("DoubleTree_Yellow");
		DoubleTree_YMesh->SetWorldPosition(10400.f, 5150.f, 26700.f);
		DoubleTree_YMesh->SetWorldScale(3.f, 3.f, 3.f);

		CGameObject* DoubleTree_P = m_Owner->CreateObject<CGameObject>("Stage6_DoubleTree_P1");
		CStaticMeshComponent* DoubleTree_PMesh = DoubleTree_P->CreateComponent<CStaticMeshComponent>("DoubleTree_P");
		DoubleTree_PMesh->SetMesh("DoubleTree_Purple");
		DoubleTree_PMesh->SetWorldPosition(12320.f, 1850.f, 16000.f);
		DoubleTree_PMesh->SetWorldRotationY(100.f);
	}

	m_Fade = m_Owner->GetViewport()->CreateUIWindow<CFade>("Fade");

    return true;
}
