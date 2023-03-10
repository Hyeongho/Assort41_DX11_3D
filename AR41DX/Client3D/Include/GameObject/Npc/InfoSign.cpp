#include "InfoSign.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"

CInfoSign::CInfoSign()
{
	SetTypeID<CInfoSign>();

	m_ObjectTypeName = "InfoSign";

	m_DialogCount = 0;
	m_NpcType = ENpcList::MrKrabs;
	m_NpcMapPos = EMapList::Bikini_Bottom;
	m_EnableDialog = false;
	m_NpcMeshType = MeshType::Animation;
}

CInfoSign::CInfoSign(const CInfoSign& Obj)
	: CNpc(Obj)
{
	m_AnimMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Animation = (CAnimation*)FindComponent("InfoSignAnimation");

	m_DialogCount = Obj.m_DialogCount;
	m_NpcType = Obj.m_NpcType;
	m_NpcMapPos = Obj.m_NpcMapPos;
	m_EnableDialog = Obj.m_EnableDialog;
	m_NpcMeshType = Obj.m_NpcMeshType;
}

CInfoSign::~CInfoSign()
{
}

void CInfoSign::Start()
{
	CNpc::Start();

	CInput::GetInst()->AddBindFunction<CInfoSign>("F", Input_Type::Up, this, &CInfoSign::StartDialog, m_Scene);
}

bool CInfoSign::Init()
{
	CNpc::Init();

	return true;
}

void CInfoSign::Update(float DeltaTime)
{
	CNpc::Update(DeltaTime);
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
}

void CInfoSign::DebugKeyF1()
{
}

void CInfoSign::DebugKeyF2()
{
}
