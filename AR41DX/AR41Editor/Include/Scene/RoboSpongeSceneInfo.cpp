#include "RoboSpongeSceneInfo.h"

#include "Scene/Scene.h"

#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderCube.h"
#include "Component/LightComponent.h"
#include "Component/TerrainComponent.h"
#include "../GameObject/Player.h"
#include "../GameObject/Object/Common/Trampoline.h"
#include "../GameObject/Object/CBL/CBL_Platform.h"
#include "../GameObject/Object/CBL/CBL_Floor.h"
#include "../GameObject/Object/CBL/CBL_BaseMesh.h"
#include "../GameObject/BossMonster/RoboSponge/RoboSponge.h"
#include "../GameObject/BossMonster/RoboSponge/RoboSponge_Knob.h"
#include "../UI/TitleSceneUI.h"
#include "../UI/InteractUI.h"
#include "../UI/DialogUI.h"
#include "../UI/PauseUI.h"

CRoboSpongeSceneInfo::CRoboSpongeSceneInfo()
{
	m_ClassTypeName = "RoboSpongeSceneInfo";
}

CRoboSpongeSceneInfo::~CRoboSpongeSceneInfo()
{
}

bool CRoboSpongeSceneInfo::Init()
{
	CSceneInfo::Init();

	CGameObject* GlobalLightObj = m_Owner->CreateObject<CGameObject>("GlobalLight");
	CLightComponent* GlobalLightComponent = GlobalLightObj->CreateComponent<CLightComponent>("GlobalLight");
	GlobalLightComponent->SetLightType(ELightType::Direction);
	GlobalLightComponent->SetRelativeRotation(0.f, 90.f, 0.f);
	m_Owner->GetLightManager()->SetGlobalLightObject(GlobalLightObj);

	CInteractUI* InteractUI = m_Owner->GetViewport()->CreateUIWindow<CInteractUI>("InteractUI");
	CDialogUI* Dialog = m_Owner->GetViewport()->CreateUIWindow<CDialogUI>("DialogUI");
	CPauseUI* PauseUI = m_Owner->GetViewport()->CreateUIWindow<CPauseUI>("PauseUI");

	int TerrainCount = 400;
	float TerrainSize = 40.f;
	Vector2 CenterPos(TerrainCount / 2 * TerrainSize, TerrainCount / 2 * TerrainSize);

	CGameObject* TerrainObj = m_Owner->CreateObject<CGameObject>("TerrainObj");
	CTerrainComponent* TerrainComponent = TerrainObj->CreateComponent<CTerrainComponent>("TerrainComponent");
	TerrainComponent->CreateTerrain(TerrainCount, TerrainCount, TerrainSize, TerrainSize, TEXT("LandScape/RoboSponge_Height.png"));
	TerrainObj->SetWorldPosition(0.f, 0.f, 0.f);
	TerrainComponent->GetSize();

	CCBL_BaseMesh* BaseMesh = m_Owner->CreateObject<CCBL_BaseMesh>("BaseMesh");
	BaseMesh->SetWorldPosition(CenterPos.x, 600.f, CenterPos.y);

	CCBL_Floor* Floor = m_Owner->CreateObject<CCBL_Floor>("Floor");
	Floor->SetWorldPosition(CenterPos.x, 605.f, CenterPos.y);

	CRoboSponge* RoboSponge = m_Owner->CreateObject<CRoboSponge>("RoboSponge");
	RoboSponge->SetWorldPosition(CenterPos.x, 1000.f, CenterPos.y);
	RoboSponge->SetMapCenter(CenterPos.x, 1000.f, CenterPos.y);

	CRoboSponge_Knob* Knob = m_Owner->CreateObject<CRoboSponge_Knob>("Knob");
	Knob->SetWorldPosition(5500.f, 800.f, 6000.f);

	float StandardPos = 60 * TerrainSize;

	for (int i = 0; i < 12; i++) {
		float Radian = i * 30.f * PI / 180.f;

		std::string PlatformName = "Platform" + std::to_string(i);
		
		CCBL_Platform* Platform = m_Owner->CreateObject<CCBL_Platform>(PlatformName);
		Platform->SetWorldPosition(CenterPos.x + StandardPos * sinf(Radian), 1800.f, CenterPos.y + StandardPos * cosf(Radian));
		Platform->SetLookBoss(RoboSponge->GetWorldPos());

		std::string TrampolineName = "Platform" + std::to_string(i);
		CTrampoline* Trampoline = m_Owner->CreateObject<CTrampoline>(TrampolineName);
		Trampoline->SetWorldPosition(CenterPos.x + StandardPos * 1.2f * sinf(Radian), 610.f, CenterPos.y + StandardPos * 1.2f * cosf(Radian));

	}



	CPlayer* Player = m_Owner->CreateObject<CPlayer>("Player");
	SetPlayerObject(Player);
	Player->SetRespawnPos(TerrainSize * TerrainCount * 0.5f, 0.f, TerrainSize * TerrainCount * 0.5f);



	return true;
}
