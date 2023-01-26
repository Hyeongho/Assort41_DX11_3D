#include "Sandy.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Input.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CSandy::CSandy()
{
	SetTypeID<CPlayer>();

	m_ObjectTypeName = "Sandy";
}

CSandy::CSandy(const CSandy& Obj) : CPlayer(Obj)
{
}

CSandy::~CSandy()
{
}

void CSandy::Start()
{
	CPlayer::Start();

	if (m_Scene)
	{
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
	}

	CInput::GetInst()->AddBindFunction<CPlayer>("SandyMoveFront", Input_Type::Push, this, &CPlayer::MoveFront, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("SandyMoveBack", Input_Type::Push, this, &CPlayer::MoveBack, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("SandyMoveLeft", Input_Type::Push, this, &CPlayer::MoveLeft, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("SandyMoveRight", Input_Type::Push, this, &CPlayer::MoveRight, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("SandyJump", Input_Type::Down, this, &CPlayer::Jump, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("SandyLButton", Input_Type::Down, this, &CPlayer::AttackKey, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RButton", Input_Type::Push, this, &CPlayer::CameraRotationKey, m_Scene);
}

bool CSandy::Init()
{
	if (!CPlayer::Init())
	{
		return false;
	}

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);

	m_Arm->SetTargetOffset(0.f, 150.f, 0.f);

	m_Mesh->SetMesh("Sandy");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("SandyAnimation");

	m_Animation->AddAnimation("Sandy_Idle", "Sandy_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_Walk", "Sandy_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_Run", "Sandy_Run", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_JumpDW", "Sandy_JumpDW", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_JumpUp", "Sandy_JumpUp", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_Jump_Landing_NonAdditive", "Sandy_Jump_Landing_NonAdditive", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_DoubleJump", "Sandy_DoubleJump", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_Karate_Chop", "Sandy_Karate_Chop", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_Karate_Kick", "Sandy_Karate_Kick", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_Lasso_Start", "Sandy_Lasso_Start", 1.f, 1.f, true);
	m_Animation->AddAnimation("Sandy_Death", "Sandy_Death", 1.f, 1.f, true);


	return true;
}

void CSandy::Update(float DeltaTime)
{
	CPlayer::Update(DeltaTime);
}

void CSandy::PostUpdate(float DeltaTime)
{
	CPlayer::PostUpdate(DeltaTime);
}

CPlayer* CSandy::Clone() const
{
	return new CSandy(*this);
}

void CSandy::Save(FILE* File)
{
	CPlayer::Save(File);
}

void CSandy::Load(FILE* File)
{
	CPlayer::Load(File);
}
