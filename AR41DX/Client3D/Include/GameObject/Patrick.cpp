#include "Patrick.h"
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

CPatrick::CPatrick()
{
	SetTypeID<CPlayer>();

	m_ObjectTypeName = "Patrick";
}

CPatrick::CPatrick(const CPatrick& Obj) : CPlayer(Obj)
{
}

CPatrick::~CPatrick()
{
}


void CPatrick::Start()
{
	CPlayer::Start();

	if (m_Scene)
	{
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
	}

	CInput::GetInst()->AddBindFunction<CPatrick>("PatrickMoveFront", Input_Type::Push, this, &CPatrick::MoveFront, m_Scene);
	CInput::GetInst()->AddBindFunction<CPatrick>("PatrickMoveBack", Input_Type::Push, this, &CPatrick::MoveBack, m_Scene);

	CInput::GetInst()->AddBindFunction<CPatrick>("PatrickMoveLeft", Input_Type::Push, this, &CPatrick::MoveLeft, m_Scene);
	CInput::GetInst()->AddBindFunction<CPatrick>("PatrickMoveRight", Input_Type::Push, this, &CPatrick::MoveRight, m_Scene);

	CInput::GetInst()->AddBindFunction<CPatrick>("PatrickJump", Input_Type::Down, this, &CPatrick::Jump, m_Scene);

	CInput::GetInst()->AddBindFunction<CPatrick>("PatrickLButton", Input_Type::Down, this, &CPatrick::LButton, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RButton", Input_Type::Push, this, &CPlayer::CameraRotationKey, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("PatrickLButton", Input_Type::Down, this, &CPlayer::AttackKey, m_Scene);
}

bool CPatrick::Init()
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

	m_Mesh->SetMesh("Patrick");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("PatrickAnimation");

	m_Animation->AddAnimation("Patrick_Idle", "Patrick_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_Walk", "Patrick_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_Run", "Patrick_Run", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_DoubleJump", "Patrick_DoubleJump", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_Jump", "Patrick_Jump", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_PickUp", "Patrick_PickUp", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_PickUpIdle", "Patrick_PickUpIdle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_PickUpWalk", "Patrick_PickUpWalk", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_Throw", "Patrick_Throw", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_Slam", "Patrick_Slam", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_BellyAttack", "Patrick_BellyAttack", 1.f, 1.f, true);
	m_Animation->AddAnimation("Patrick_Death", "Patrick_Death", 1.f, 1.f, true);

	return true;
}

void CPatrick::Update(float DeltaTime)
{
	CPlayer::Update(DeltaTime);
}

void CPatrick::PostUpdate(float DeltaTime)
{
	CPlayer::PostUpdate(DeltaTime);
}

CPatrick* CPatrick::Clone() const
{
	return new CPatrick(*this);
}

void CPatrick::Save(FILE* File)
{
	CPlayer::Save(File);
}

void CPatrick::Load(FILE* File)
{
	CPlayer::Load(File);
}

void CPatrick::MoveFront()
{
	CPlayer::MoveFront();

	m_Animation->ChangeAnimation("Patrick_Walk");
}

void CPatrick::MoveBack()
{
	CPlayer::MoveBack();

	m_Animation->ChangeAnimation("Patrick_Walk");
}

void CPatrick::MoveLeft()
{
	CPlayer::MoveLeft();

	m_Animation->ChangeAnimation("Patrick_Walk");
}

void CPatrick::MoveRight()
{
	CPlayer::MoveRight();

	m_Animation->ChangeAnimation("Patrick_Walk");
}

void CPatrick::LButton()
{
	BellyAttack();
}

void CPatrick::RButton()
{
	SlamDown();
}

void CPatrick::Jump()
{
	m_Animation->ChangeAnimation("Patrick_Jump");
}

void CPatrick::BellyAttack()
{
	m_Animation->ChangeAnimation("Patrick_BellyAttack");
}

void CPatrick::SlamDown()
{
	m_Animation->ChangeAnimation("Patrick_Slam");
}

void CPatrick::PickUp()
{
	m_Animation->ChangeAnimation("Patrick_PickUp");
}

void CPatrick::Throw()
{
	m_Animation->ChangeAnimation("Patrick_Throw");
}

void CPatrick::Death()
{
	m_Animation->ChangeAnimation("Patrick_Death");
}
