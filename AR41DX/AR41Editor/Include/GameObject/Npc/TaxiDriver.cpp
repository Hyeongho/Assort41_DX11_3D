#include "TaxiDriver.h"

#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Input.h"
#include "Scene/SceneManager.h"
#include "../../Scene/LoadingSceneInfo.h"

CTaxiDriver::CTaxiDriver()
{
	SetTypeID<CTaxiDriver>();

	m_ObjectTypeName = "TaxiDriver";

	m_NpcType = ENpcList::TaxiStop;
	m_NpcMapPos = EMapList::Jelly_Fish_Field;
	m_NpcMeshType = MeshType::Static;
}

CTaxiDriver::CTaxiDriver(const CTaxiDriver& Obj)
	: CNpc(Obj)
{
	m_StaticMesh = (CStaticMeshComponent*)FindComponent("Mesh");

	m_NpcType = Obj.m_NpcType;
	m_NpcMapPos = Obj.m_NpcMapPos;
	m_NpcMeshType = Obj.m_NpcMeshType;
}

CTaxiDriver::~CTaxiDriver()
{
}

void CTaxiDriver::Start()
{
	CNpc::Start();

	CInput::GetInst()->AddBindFunction<CTaxiDriver>("F", Input_Type::Up, this, &CTaxiDriver::ChangeSceneToBB, m_Scene);
}

bool CTaxiDriver::Init()
{
	CNpc::Init();

	m_StaticMesh->SetMesh("Taxi_Stop");

	Vector3 ColSize = m_StaticMesh->GetMeshSize();

	if (m_StaticMesh->GetMeshSize().x >= m_StaticMesh->GetMeshSize().z)
		ColSize.z = m_StaticMesh->GetMeshSize().x;
	else
		ColSize.x = m_StaticMesh->GetMeshSize().z;

	m_Collider->SetBoxHalfSize(ColSize / 2.f);
	m_Collider->SetRelativePositionY(ColSize.y / 2.f);
	
	return true;
}

void CTaxiDriver::Update(float DeltaTime)
{
	CNpc::Update(DeltaTime);
}

void CTaxiDriver::PostUpdate(float DeltaTime)
{
	CNpc::PostUpdate(DeltaTime);
}

CTaxiDriver* CTaxiDriver::Clone() const
{
	return new CTaxiDriver(*this);
}

void CTaxiDriver::Save(FILE* File)
{
	CNpc::Save(File);
}

void CTaxiDriver::Load(FILE* File)
{
	CNpc::Load(File);
}

void CTaxiDriver::ChangeSceneToBB()
{
	// ¸Ê º¯°æ
	//CSceneManager::GetInst()->CreateNextScene();
	//CSceneManager::GetInst()->CreateSceneInfo<CLoadingSceneInfo>(false, "BikiniBottom.scn");
}
