#include "InfoSign.h"

#include "Input.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"
#include "../Object/Common/Collectible/GoldenSpatula.h"
#include "../BossMonster/RoboSponge/RoboSponge.h"

#include "Scene/SceneManager.h"
#include "../../Scene/LoadingSceneInfo.h"

CInfoSign::CInfoSign()
{
	SetTypeID<CInfoSign>();

	m_ObjectTypeName = "InfoSign";

	m_DialogCount = 0;
	m_NpcType = ENpcList::InfoSign;
	m_NpcMapPos = EMapList::Bikini_Bottom;
	m_EnableDialog = false;
	m_NpcMeshType = MeshType::Static;

	m_RotLeft = false;
	m_RotRight = true;
	m_InfoSignName = "";
}

CInfoSign::CInfoSign(const CInfoSign& Obj)
	: CNpc(Obj)
{
	m_StaticMesh = (CStaticMeshComponent*)FindComponent("Mesh");

	m_DialogCount = Obj.m_DialogCount;
	m_NpcType = Obj.m_NpcType;
	m_NpcMapPos = Obj.m_NpcMapPos;
	m_EnableDialog = Obj.m_EnableDialog;
	m_NpcMeshType = Obj.m_NpcMeshType;

	m_RotLeft = Obj.m_RotLeft;
	m_RotRight = Obj.m_RotRight;
	m_InfoSignName = Obj.m_InfoSignName;
}

CInfoSign::~CInfoSign()
{
}

void CInfoSign::Start()
{
	CNpc::Start();

	CInput::GetInst()->AddBindFunction<CInfoSign>("F", Input_Type::Up, this, &CInfoSign::InputF, m_Scene);
	m_Collider->SetName("InfoSignCollider");
}

bool CInfoSign::Init()
{
	CNpc::Init();

	m_StaticMesh->SetMesh("InfoSign");

	Vector3 ColSize = m_StaticMesh->GetMeshSize();

	if (m_StaticMesh->GetMeshSize().x >= m_StaticMesh->GetMeshSize().z)
		ColSize.z = m_StaticMesh->GetMeshSize().x;
	else
		ColSize.x = m_StaticMesh->GetMeshSize().z;

	m_Collider->SetBoxHalfSize(ColSize / 2.f);
	m_Collider->SetRelativePositionY(ColSize.y / 2.f);

	return true;
}

void CInfoSign::Update(float DeltaTime)
{
	CNpc::Update(DeltaTime);

	int ZRot = (int)GetWorldRot().z;
	ZRot %= 360;

	if (ZRot == -22 && m_RotRight) {
		m_RotLeft = true;
		m_RotRight = false;
	}

	if (ZRot == -338 && m_RotLeft) {
		m_RotLeft = false;
		m_RotRight = true;
	}

	if ((ZRot < -22 && ZRot > -180) || (ZRot < 338 && ZRot > 180)) {
		SetWorldRotationZ(-21.f);
		m_RotLeft = false;
		m_RotRight = true;
	}

	if ((ZRot > -338 && ZRot < -180) || (ZRot > 22 && ZRot < 180)) {
		SetWorldRotationZ(-339.f);
		m_RotLeft = true;
		m_RotRight = false;
	}

	if (m_RotLeft)
		AddWorldRotationZ(g_DeltaTime * 70.f);
	if (m_RotRight)
		AddWorldRotationZ(g_DeltaTime * -70.f);
}

void CInfoSign::PostUpdate(float DeltaTime)
{
	CNpc::PostUpdate(DeltaTime);
}

CInfoSign* CInfoSign::Clone() const
{
	return new CInfoSign(*this);
}

void CInfoSign::Save(FILE* File)
{
	CNpc::Save(File);
}

void CInfoSign::Load(FILE* File)
{
	CNpc::Load(File);
}

void CInfoSign::StartDialog()
{
	if (!m_EnableDialog)
		return;


}

void CInfoSign::InputF()
{
	if (!m_EnableDialog)
		return;

	if (m_NpcMapPos == EMapList::Bikini_Bottom) {
		// MapChange To RoboSpongeSceneInfo
		m_Collider->SetEnable(false);
		CSceneManager::GetInst()->CreateNextScene(true);
		CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "Title.scn");
	}
	if (m_NpcMapPos == EMapList::Jelly_Fish_Field) {


	}
	if (m_NpcMapPos == EMapList::Chum_Bucketlab) {
		CRoboSponge* RoboSponge = (CRoboSponge*)m_Scene->FindObject("RoboSponge");

		if (RoboSponge) {
			RoboSponge->ActionStart();
		}


		CSound* Sound = CResourceManager::GetInst()->FindSound("BossStage");

		if (Sound)
			Sound->Play();

	}
}

void CInfoSign::DebugKeyF1()
{
}

void CInfoSign::DebugKeyF2()
{
}