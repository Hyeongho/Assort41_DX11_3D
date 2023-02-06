#include "Player.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CPlayer::CPlayer()
{
	SetTypeID<CPlayer>();

	m_ObjectTypeName = "Player";
}

CPlayer::CPlayer(const CPlayer& Obj) : CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CGameObject::Start();

	if (m_Scene)
	{
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
	}

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveFront", Input_Type::Push, this, &CPlayer::MoveFront, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveBack", Input_Type::Push, this, &CPlayer::MoveBack, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", Input_Type::Push, this, &CPlayer::MoveLeft, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", Input_Type::Push, this, &CPlayer::MoveRight, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Jump", Input_Type::Down, this, &CPlayer::Jump, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("LButton", Input_Type::Down, this, &CPlayer::AttackKey, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RButton", Input_Type::Push, this, &CPlayer::CameraRotationKey, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("ChangeSpongebob", Input_Type::Down, this, &CPlayer::ChangeSpongebob, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("ChangePatrick", Input_Type::Down, this, &CPlayer::ChangePatrick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("ChangeSandy", Input_Type::Down, this, &CPlayer::ChangeSandy, m_Scene);

}

bool CPlayer::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Rigid);
	m_Mesh->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);

	m_Arm->SetInheritRotY(true);
	m_Arm->SetTargetOffset(0.f, 150.f, 0.f);

	m_Mesh->SetMesh("Sandy");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("PlayerAnimation");

	LoadSandyAnim();

	//m_Animation->AddAnimation("PlayerIdle", "PlayerIdle", 1.f, 1.f, true);

	//m_Rigid->SetGravity(true);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//m_Sprite->AddRelativeRotationZ(180.f * DeltaTime);

	//m_RightChild->AddRelativeRotationZ(360.f * DeltaTime);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer* CPlayer::Clone() const
{
	return new CPlayer(*this);
}

void CPlayer::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPlayer::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CPlayer::LoadSpongebobAnim()
{
}

void CPlayer::LoadPatrickAnim()
{
}

void CPlayer::LoadSandyAnim()
{
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
}

void CPlayer::SetMesh(std::string Mesh)
{
	m_Mesh->SetMesh(Mesh);
}

void CPlayer::MoveFront()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}

	AddWorldPosition(GetWorldAxis(AXIS_Z) * 100.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveBack()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}

	AddWorldPosition(GetWorldAxis(AXIS_Z) * -100.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveLeft()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}

	AddWorldRotationY(180.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveRight()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}

	AddWorldRotationY(-180.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::Jump()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}
}

void CPlayer::AttackKey()
{
}

void CPlayer::CameraRotationKey()
{
	const Vector2& MouseMove = CInput::GetInst()->GetMouseMove();

	float	DeltaTime = CEngine::GetInst()->GetDeltaTime();

	if (MouseMove.x != 0.f)
	{
		m_Arm->AddRelativeRotationY(MouseMove.x * DeltaTime * 180.f);
	}

	if (MouseMove.y != 0.f)
	{
		m_Arm->AddRelativeRotationX(MouseMove.y * DeltaTime * 180.f);
	}
}

void CPlayer::ChangeSpongebob()
{
	if (CResourceManager::GetInst()->FindMesh("Spongebob"))
	{
		m_Mesh->SetMesh("Spongebob");

		m_MainCharacter = EMain_Character::Spongebob;
	}
}

void CPlayer::ChangePatrick()
{
	if (CResourceManager::GetInst()->FindMesh("Patrick"))
	{
		m_Mesh->SetMesh("Patrick");

		m_MainCharacter = EMain_Character::Patrick;
	}
}

void CPlayer::ChangeSandy()
{
	if (CResourceManager::GetInst()->FindMesh("Sandy"))
	{
		m_Mesh->SetMesh("Sandy");

		m_MainCharacter = EMain_Character::Sandy;
	}
}
