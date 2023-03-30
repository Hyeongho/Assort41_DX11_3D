
#include "EditorDefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneViewport.h"
#include "../UI/LoadingUI.h"
#include "../UI/TitleSceneUI.h"
#include "../UI/DialogUI.h"
#include "../UI/InteractUI.h"
#include "../GameObject/Player.h"
#include "../GameObject/Npc/MrKrabs.h"
#include "../GameObject/Npc/Patric.h"
#include "../GameObject/Npc/Squidward.h"
#include "../GameObject/Npc/TaxiDriver.h"
#include "../GameObject/Npc/BusDriver.h"
#include "../GameObject/Tikis/Tiki_Stone.h"
#include "../GameObject/Tikis/Tiki_Thunder.h"
#include "../GameObject/Tikis/Tiki_Wood.h"
#include "../GameObject/Object/Common/InteractButton.h"
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

	//m_Owner->GetViewport()->CreateUIWindow<CLoadingUI>("LoadingUI");

	//m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	//m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");

	//CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	//Player->SetRespawnPos(10300.f, 255.f, 6500.f);
	//SetPlayerObject(Player);

	//CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	//CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	//TerrainComponent->SetHeight(1.f);
	//TerrainComponent->CreateTerrain(800, 600, 40.f, 40.f, TEXT("LandScape/RobotSpongebob.png"));
	return true;
}

