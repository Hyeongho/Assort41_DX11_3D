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
	: m_Speed(150.f)
	, m_KeyCount(0)
{
	SetTypeID<CPlayer>();

	m_ObjectTypeName = "Player";
}

CPlayer::CPlayer(const CPlayer& Obj) : CGameObject(Obj)
	, m_Speed(Obj.m_Speed)
	, m_KeyCount(Obj.m_KeyCount)
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

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveUp", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveDown", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveUp", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveDown", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveRight", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("MoveLeft", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Jump", Input_Type::Down, this, &CPlayer::Jump, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Headbutt", Input_Type::Down, this, &CPlayer::Headbutt, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Missile", Input_Type::Down, this, &CPlayer::Missile, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Interaction", Input_Type::Down, this, &CPlayer::Interaction, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Menu", Input_Type::Down, this, &CPlayer::Menu, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("IngameUI", Input_Type::Down, this, &CPlayer::IngameUI, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("LButton", Input_Type::Down, this, &CPlayer::AttackKey, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RButton", Input_Type::Push, this, &CPlayer::RClick, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Spongebob", Input_Type::Push, this, &CPlayer::ChangeSpongebob, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Patrick", Input_Type::Push, this, &CPlayer::ChangePatrick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Sandy", Input_Type::Push, this, &CPlayer::ChangeSandy, m_Scene);

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

	m_Arm->SetTargetOffset(0.f, 150.f, 0.f);

	m_Rigid->SetGround(true);	//땅에 붙어있다고 설정

	ChangeSpongebob();
	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	CameraRotationKey();
	/*
	//높이 설정
	CNavigationManager3D* nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	float y = nav->GetHeight(GetWorldPos());
	if (FLT_MIN != y)
	{
		SetWorldPositionY(y);
		m_Rigid->SetGround(true);
	}
	*/
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
	//게임데이터 여기서 저장
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

void CPlayer::MoveFront()
{
	//switch (m_MainCharacter)
	//{
	//case EMain_Character::Spongebob:
	//	break;
	//case EMain_Character::Patrick:
	//	break;
	//case EMain_Character::Sandy:
	//	break;
	//default:
	//	break;
	//}

	AddWorldPosition(GetWorldAxis(AXIS_Z) * -100.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveBack()
{
	AddWorldPosition(GetWorldAxis(AXIS_Z) * 100.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveLeft()
{
	//switch (m_MainCharacter)
	//{
	//case EMain_Character::Spongebob:
	//	break;
	//case EMain_Character::Patrick:
	//	break;
	//case EMain_Character::Sandy:
	//	break;
	//default:
	//	break;
	//}

	AddWorldRotationY(180.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveRight()
{
	//switch (m_MainCharacter)
	//{
	//case EMain_Character::Spongebob:
	//	break;
	//case EMain_Character::Patrick:
	//	break;
	//case EMain_Character::Sandy:
	//	break;
	//default:
	//	break;
	//}

	AddWorldRotationY(-180.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::Jump()
{
	//switch (m_MainCharacter)
	//{
	//case EMain_Character::Spongebob:
	//	break;
	//case EMain_Character::Patrick:
	//	break;
	//case EMain_Character::Sandy:
	//	break;
	//default:
	//	break;
	//}
}

void CPlayer::AttackKey()
{
}

void CPlayer::CameraRotationKey()
{
	const Vector2& MouseMove = CInput::GetInst()->GetMouseMove()*m_Speed * g_DeltaTime;
	m_Arm->AddRelativeRotationY(MouseMove.x);
	m_Arm->AddRelativeRotationX(MouseMove.y);
	if (m_Arm->GetRelativeRot().x > 50.f)
	{
		m_Arm->SetRelativeRotationX(50.f);
	}
	else if (m_Arm->GetRelativeRot().x < -30.f)
	{
		m_Arm->SetRelativeRotationX(-30.f);
	}
}

void CPlayer::KeyDown()
{
}

void CPlayer::KeyUp()
{
}

void CPlayer::Headbutt()
{
}

void CPlayer::Missile()
{
}

void CPlayer::Interaction()
{
}

void CPlayer::Menu()
{
}

void CPlayer::IngameUI()
{
}

void CPlayer::RClick()
{
}

void CPlayer::ChangeSpongebob()
{
	if (m_Name == "Spongebob")
	{
		return;
	}
	m_Name = "Spongebob";
	m_Mesh->SetMesh(m_Name);

	m_Animation = m_Mesh->SetAnimation<CAnimation>("SponegebobAnimation");
	m_Animation->AddAnimation("PlayerIdle", "Spongebob_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("PlayerWalk", "Spongebob_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("PlayerAttack", "Spongebob_Attack", 1.f, 1.f, false);
	m_Animation->Start();
}

void CPlayer::ChangePatrick()
{
	if (m_Name == "Patrick")
	{
		return;
	}
	m_Name = "Patrick";
	m_Mesh->SetMesh(m_Name);

	m_Animation = m_Mesh->SetAnimation<CAnimation>("PatrickAnimation");
	m_Animation->AddAnimation("PlayerIdle", "Patrick_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("PlayerWalk", "Patrick_Walk", 1.f, 1.f, true);
	m_Animation->AddAnimation("PlayerAttack", "Patrick_BellyAttack", 1.f, 1.f, false);
	m_Animation->Start();
}

void CPlayer::ChangeSandy()
{
	//if (CResourceManager::GetInst()->FindMesh("Sandy"))
	//{
	//	m_Mesh->SetMesh("Sandy");

	//	m_MainCharacter = EMain_Character::Sandy;
	//}
}
