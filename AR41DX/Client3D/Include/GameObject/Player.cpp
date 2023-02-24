#include "Player.h"
#include "Weapon.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Scene/NavigationManager3D.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CPlayer::CPlayer() : m_Speed(150.f), m_KeyCount(0), m_MainCharacter(EMain_Character::Max), m_IsHolding(false), m_HoverTime(0.f), m_BellyAttackTime(0.f), m_SlamDown(false)
{
	SetTypeID<CPlayer>();

	m_ObjectTypeName = "Player";
}

CPlayer::CPlayer(const CPlayer& Obj) : CGameObject(Obj)
, m_Speed(Obj.m_Speed)
, m_KeyCount(Obj.m_KeyCount)
, m_MainCharacter(EMain_Character::Max)
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

	CInput::GetInst()->AddBindFunction<CPlayer>("W", Input_Type::Push, this, &CPlayer::MoveFront, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("S", Input_Type::Push, this, &CPlayer::MoveBack, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("A", Input_Type::Push, this, &CPlayer::MoveLeft, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("D", Input_Type::Push, this, &CPlayer::MoveRight, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("W", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("S", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("A", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("D", Input_Type::Up, this, &CPlayer::KeyUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("W", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("S", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("A", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("D", Input_Type::Down, this, &CPlayer::KeyDown, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Space", Input_Type::Down, this, &CPlayer::Jump, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("E", Input_Type::Down, this, &CPlayer::Headbutt, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Q", Input_Type::Down, this, &CPlayer::Missile, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F", Input_Type::Down, this, &CPlayer::Interaction, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Esc", Input_Type::Down, this, &CPlayer::Menu, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Tab", Input_Type::Down, this, &CPlayer::IngameUI, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("LClick", Input_Type::Down, this, &CPlayer::LClick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Push, this, &CPlayer::RClick, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("F1", Input_Type::Push, this, &CPlayer::ChangeSpongebob, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F2", Input_Type::Push, this, &CPlayer::ChangePatrick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F3", Input_Type::Push, this, &CPlayer::ChangeSandy, m_Scene);

	LoadSpongebobAnim();
	LoadPatrickAnim();
	LoadSandyAnim();

	ChangeSpongebob();

	CWeapon* weapon = (CWeapon*)m_Scene->FindObject("Weapon");
	if (!weapon)
	{
		weapon = m_Scene->CreateObject<CWeapon>("Weapon");
	}

	//源踰붿쨷 ?뚯폆 愿??
	//CWeapon3D* weapon = m_Scene->CreateObject<CWeapon3D>("Weapon");

	/*AddChildToSocket("Weapon", weapon);
	m_WeaponMesh = (CAnimationMeshComponent*)weapon->GetRootComponent();
	m_WeaponMesh->SetEnable(false);*/
}

bool CPlayer::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	SetRootComponent(m_Mesh);

	m_Mesh->SetRelativeRotationY(180.f);

	m_Mesh->AddChild(m_Rigid);
	m_Mesh->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(false);
	m_Camera->SetInheritRotY(false);

	m_Arm->SetTargetOffset(0.f, 150.f, 0.f);

	m_Rigid->SetGround(true);	//땅에 붙어있다고 설정

	//m_Animation->AddAnimation("PlayerIdle", "PlayerIdle", 1.f, 1.f, true);
	//LoadSandyAnim();

	//m_Animation->AddAnimation("PlayerIdle", "PlayerIdle", 1.f, 1.f, true);

	//m_Animation->AddAnimation("PlayerIdle", "PlayerIdle", 1.f, 1.f, true);*/

	//m_Rigid->SetGravity(true);
	m_Rigid->SetGround(true);	//?낆뿉 遺숈뼱?덈떎怨??ㅼ젙

	LoadSpongebobAnim();
	LoadPatrickAnim();
	LoadSandyAnim();

	ChangeSandy();
	//ChangePatrick();

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	CameraRotationKey();

	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	float Y = Nav->GetHeight(GetWorldPos());
	SetWorldPositionY(Y);

	if (m_Name == "Patrick")
	{
		// Patrick Slam시 1.2초동안 공중부양.
		if (m_Rigid->GetGround() == false && m_SlamDown)
		{
			m_HoverTime += g_DeltaTime;

			if (m_HoverTime >= 1.2f)
			{
				m_Rigid->SetGround(false);

				m_HoverTime = 0;;
			}

			m_SlamDown = false;
			m_Rigid->SetGround(true); // 충돌되는지 여부 검사
		}

	}
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
	m_ReserveMesh[(int)EMain_Character::Spongebob] = CResourceManager::GetInst()->FindMesh("Spongebob");
	m_Anim[(int)EMain_Character::Spongebob] = m_Mesh->SetAnimation<CAnimation>("SponegebobAnimation");
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerIdle", "Spongebob_Idle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerWalk", "Spongebob_Walk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerAttack", "Spongebob_Attack", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerAttack", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerJump", "SpongebobJump", 1.f, 1.f, true);
}

void CPlayer::LoadPatrickAnim()
{
	m_ReserveMesh[(int)EMain_Character::Patrick] = CResourceManager::GetInst()->FindMesh("Patrick");

	if (!m_ReserveMesh[(int)EMain_Character::Patrick])
	{
		return;
	}

	m_Anim[(int)EMain_Character::Patrick] = m_Mesh->SetAnimation<CAnimation>("PatrickAnimation");
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerIdle", "Patrick_Idle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerWalk", "Patrick_Walk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerAttack", "Patrick_Run", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerDoubleJump", "Patrick_DoubleJump", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerJump", "Patrick_Jump", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("Patrick_PickUp", "Patrick_PickUp", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("Patrick_PickUpIdle", "Patrick_PickUpIdle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("Patrick_PickUpWalk", "Patrick_PickUpWalk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("Patrick_Throw", "Patrick_Throw", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("Patrick_Slam", "Patrick_Slam", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerAttack", "Patrick_BellyAttack", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerDeath", "Patrick_Death", 1.f, 1.f, false);
}

void CPlayer::LoadSandyAnim()
{
	m_ReserveMesh[(int)EMain_Character::Sandy] = CResourceManager::GetInst()->FindMesh("Sandy");

	if (!m_ReserveMesh[(int)EMain_Character::Sandy])
	{
		return;
	}

	m_Anim[(int)EMain_Character::Sandy] = m_Mesh->SetAnimation<CAnimation>("SandyAnimation");
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerIdle", "Sandy_Idle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerWalk", "Sandy_Walk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_Run", "Sandy_Run", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_JumpDW", "Sandy_JumpDW", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_JumpUp", "Sandy_JumpUp", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_Jump_Landing_NonAdditive", "Sandy_Jump_Landing_NonAdditive", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_DoubleJump", "Sandy_DoubleJump", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_Karate_Chop", "Sandy_Karate_Chop", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_Karate_Kick", "Sandy_Karate_Kick", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_Lasso_Start", "Sandy_Lasso_Start", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("Sandy_Death", "Sandy_Death", 1.f, 1.f, true);
}

void CPlayer::MoveFront()
{
	//여기에 사운드
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		CResourceManager::GetInst()->SoundPlay("Patrick_Step");
		CResourceManager::GetInst()->SetVolume(100);
		if (m_IsHolding)
		{
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("Patrick_PickUpWalk");
		}
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}

	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	float angle = m_Camera->GetWorldRot().y;
	SetWorldRotationY(angle+180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CPlayer::MoveBack()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		if (m_IsHolding)
		{
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("Patrick_PickUpWalk");
		}
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}

	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	float angle = m_Camera->GetWorldRot().y-180.f;
	SetWorldRotationY(angle+180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CPlayer::MoveLeft()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		if (m_IsHolding)
		{
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("Patrick_PickUpWalk");
		}
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}

	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	float angle = m_Camera->GetWorldRot().y -90.f;
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	
	m_Mesh->AddWorldRotationY(-180.f * CEngine::GetInst()->GetDeltaTime());

}

void CPlayer::MoveRight()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		if (m_IsHolding)
		{
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("Patrick_PickUpWalk");
		}
		break;
	case EMain_Character::Sandy:
		break;
	default:
		break;
	}
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	float angle = m_Camera->GetWorldRot().y +90.f;
	SetWorldRotationY(angle+180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}


void CPlayer::Stop()
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

	m_Mesh->AddWorldRotationY(180.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::Jump()
{
	if (m_Rigid->GetGround() == false)
	{
		return;
	}
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

	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerJump");
	m_Rigid->SetGround(false);
	m_Rigid->AddForce(0, 3500.f);
}

void CPlayer::AttackKey()
{
}

void CPlayer::CameraRotationKey()
{
	const Vector2& mouseMove = CInput::GetInst()->GetMouseMove() * m_Speed * g_DeltaTime;
	m_Arm->AddRelativeRotationY(mouseMove.x);
	m_Arm->AddRelativeRotationX(mouseMove.y);
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
	++m_KeyCount;
}

void CPlayer::KeyUp()
{
	--m_KeyCount;

	if (m_KeyCount == 0)
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerIdle");
	}
}

void CPlayer::Headbutt()
{
}

void CPlayer::Missile()
{
}

void CPlayer::Patrick_BellyAttack()
{
}

void CPlayer::Patrick_BellyAttackMove()
{
	// 여기서 m_RigidBody Force 해줘야한다. 
	AddWorldPosition(GetWorldAxis(AXIS_Z) * -400.f);
}

void CPlayer::Patrick_SlamDown() // 내려찍기
{
	m_SlamDown = true;

	m_Rigid->SetGround(true);
	// 가속도 증가 추가 필요
}

void CPlayer::Patrick_PickUp()
{
}

void CPlayer::Patrick_Throw()
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

void CPlayer::LClick()
{
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleSpin");
		break;
	case EMain_Character::Patrick:
		break;
	case EMain_Character::Sandy:
		break;
	}
	m_WeaponMesh->SetEnable(true);
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerAttack");
}

void CPlayer::ResetIdle()
{
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerIdle");
	m_WeaponMesh->SetEnable(false);
}

void CPlayer::ChangeSpongebob()
{
	if (m_MainCharacter == EMain_Character::Spongebob)
	{
		return;
	}
	m_MainCharacter = EMain_Character::Spongebob;
	m_Mesh->SetAnimation(m_Anim[(int)m_MainCharacter]);
	m_Mesh->ClearMaterial();
	m_Mesh->SetMesh(m_ReserveMesh[(int)m_MainCharacter]);
	m_Anim[(int)m_MainCharacter]->Start();
	if (m_WeaponMesh)
	{
		m_WeaponMesh->SetMesh("SpongebobWand");
		m_WeaponMesh->SetWorldScale(0.5f, 0.5f, 0.5f);
	}
}

void CPlayer::ChangePatrick()
{
	if (m_MainCharacter == EMain_Character::Patrick)
	{
		return;
	}

	m_MainCharacter = EMain_Character::Patrick;
	m_Mesh->SetAnimation(m_Anim[(int)m_MainCharacter]);
	m_Mesh->ClearMaterial();
	m_Mesh->SetMesh(m_ReserveMesh[(int)m_MainCharacter]);
	m_Anim[(int)m_MainCharacter]->Start();
}

void CPlayer::ChangeSandy()
{
	if (m_MainCharacter == EMain_Character::Sandy)
	{
		return;
	}

	m_MainCharacter = EMain_Character::Sandy;
	m_Mesh->SetAnimation(m_Anim[(int)m_MainCharacter]);
	m_Mesh->ClearMaterial();
	m_Mesh->SetMesh(m_ReserveMesh[(int)m_MainCharacter]);
	m_Anim[(int)m_MainCharacter]->Start();
}


