#include "RoboSponge.h"

#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Input.h"
#include "Engine.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

#include "Scene/Scene.h"
#include "../UI/PauseUI.h"

CRoboSponge::CRoboSponge()
{
	SetTypeID<CRoboSponge>();

	m_ObjectTypeName = "Robo_Sponge";
}

CRoboSponge::CRoboSponge(const CRoboSponge& Obj) : CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
}

CRoboSponge::~CRoboSponge()
{
}

void CRoboSponge::Start()
{
	CGameObject::Start();

	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F1", Input_Type::Up, this, &CRoboSponge::AttackHorizL, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F2", Input_Type::Up, this, &CRoboSponge::AttackHorizR, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F3", Input_Type::Up, this, &CRoboSponge::AttackVerticLLoop, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F4", Input_Type::Up, this, &CRoboSponge::AttackVerticRLoop, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F5", Input_Type::Up, this, &CRoboSponge::AttackWordsLoop, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F6", Input_Type::Up, this, &CRoboSponge::Hit1, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F7", Input_Type::Up, this, &CRoboSponge::Hit2, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F8", Input_Type::Up, this, &CRoboSponge::Idle, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F9", Input_Type::Up, this, &CRoboSponge::Victory, m_Scene);
}

bool CRoboSponge::Init()
{
	CGameObject::Init();


	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Robo_Sponge");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("RoboSpongeAnimation");

	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_L", "Robo_Sponge_Attack_Horiz_L", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_L_Pose", "Robo_Sponge_Attack_Horiz_L_Pose", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_R", "Robo_Sponge_Attack_Horiz_R", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_R_Pose", "Robo_Sponge_Attack_Horiz_R_Pose", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_L_Loop", "Robo_Sponge_Attack_Vertic_L_Loop", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Words_Loop", "Robo_Sponge_Attack_Words_Loop", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_L_Start", "Robo_Sponge_Attack_Vertic_L_Start", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_R_Loop", "Robo_Sponge_Attack_Vertic_R_Loop", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_R_Start", "Robo_Sponge_Attack_Vertic_R_Start", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Words_Start", "Robo_Sponge_Attack_Words_Start", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Hit1", "Robo_Sponge_Hit1", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Hit2", "Robo_Sponge_Hit2", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Hit2_Pose", "Robo_Sponge_Hit2_Pose", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Idle", "Robo_Sponge_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Vertic_L_Hold", "Robo_Sponge_Vertic_L_Hold", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Vertic_R_Hold", "Robo_Sponge_Vertic_R_Hold", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Victory", "Robo_Sponge_Victory", 1.f, 1.f, true);

	m_Animation->SetCurrentAnimation("Robo_Sponge_Idle");

    return true;
}

void CRoboSponge::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);





}

void CRoboSponge::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CRoboSponge* CRoboSponge::Clone() const
{
	return new CRoboSponge(*this);
}

void CRoboSponge::Save(FILE* File)
{
	CRoboSponge::Save(File);
}

void CRoboSponge::Load(FILE* File)
{
	CRoboSponge::Load(File);
}

void CRoboSponge::AttackHorizL()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Horiz_L");
}

void CRoboSponge::AttackHorizLPose()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Horiz_L_Pose");
}

void CRoboSponge::AttackHorizR()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Horiz_R");
}

void CRoboSponge::AttackHorizRPose()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Horiz_R_Pose");
}

void CRoboSponge::AttackVerticLLoop()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Vertic_L_Loop");
}

void CRoboSponge::AttackVerticLStart()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Vertic_L_Start");
}

void CRoboSponge::AttackVerticRLoop()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Vertic_R_Loop");
}

void CRoboSponge::AttackVerticRStart()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Vertic_R_Start");
}

void CRoboSponge::AttackWordsLoop()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Words_Loop");
}

void CRoboSponge::AttackWordsStart()
{
	m_Animation->ChangeAnimation("RoboSponge_Attack_Words_Start");
}

void CRoboSponge::Hit1()
{
	m_Animation->ChangeAnimation("RoboSponge_Hit1");
}

void CRoboSponge::Hit2()
{
	m_Animation->ChangeAnimation("RoboSponge_Hit2");
}

void CRoboSponge::Hit2Pose()
{
	m_Animation->ChangeAnimation("RoboSponge_Hit2_Pose");
}

void CRoboSponge::Idle()
{
	m_Animation->ChangeAnimation("RoboSponge_Idle");
}

void CRoboSponge::VerticLHold()
{
	m_Animation->ChangeAnimation("RoboSponge_Vertic_L_Hold");
}

void CRoboSponge::VerticRHold()
{
	m_Animation->ChangeAnimation("RoboSponge_Vertic_R_Hold");
}

void CRoboSponge::Victory()
{
	m_Animation->ChangeAnimation("RoboSponge_Victory");
}