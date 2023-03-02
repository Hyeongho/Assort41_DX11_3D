#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Input.h"
#include "Engine.h"
#include "Device.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/NavigationAgent3D.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Input.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Scene/NavigationManager3D.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"
#include "../UI/PlayerUI.h"
#include "../UI/PauseUI.h"

CPlayer::CPlayer()
	: m_Speed(500.f)
	, m_CameraSpeed(150.f)
	, m_KeyCount(0)
	, m_MainCharacter(EMain_Character::Max)
	, m_IsHolding(false)
	, m_HoverTime(0.f)
	, m_BellyAttackTime(0.f)
	, m_SlamDown(false)
	, m_IsLoading(false)
	, m_IsDoubleJump(false)
{
	SetTypeID<CPlayer>();

	m_ObjectTypeName = "Player";
	LoadCharacter();
}

CPlayer::CPlayer(const CPlayer& Obj) 
	: CGameObject(Obj)
	, m_Speed(Obj.m_Speed)
	, m_CameraSpeed(Obj.m_CameraSpeed)
	, m_MainCharacter(EMain_Character::Max)
	, m_IsDoubleJump(false)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_NavAgent = (CNavigationAgent3D*)FindComponent("NavAgent");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
	m_HeadCube = (CColliderCube*)FindComponent("HeadCube");
	LoadCharacter();
}

CPlayer::~CPlayer()
{
	if (m_LoadData != m_PlayerData)
	{
		SaveCharacter();
	}
}

void CPlayer::Destroy()
{
	CGameObject::Destroy();
	if (m_PlayerUI)
	{
		m_PlayerUI->Destroy();
	}
	if (m_PauseUI)
	{
		m_PauseUI->Destroy();
	}
	CInput::GetInst()->ClearCallback();
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
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Down, this, &CPlayer::RClick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Push, this, &CPlayer::Bowl, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Up, this, &CPlayer::BowlThrow, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("F1", Input_Type::Push, this, &CPlayer::ChangeSpongebob, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F2", Input_Type::Push, this, &CPlayer::ChangePatrick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F3", Input_Type::Push, this, &CPlayer::ChangeSandy, m_Scene);

	m_PlayerUI = m_Scene->GetViewport()->CreateUIWindow<CPlayerUI>("PlayerUI");
	m_PlayerUI->SetHp(m_PlayerData.CurHP);
	m_PlayerUI->SetMaxHp(m_PlayerData.MaxHP);
	m_PlayerUI->SetGlitter(m_PlayerData.Glittering);
	m_PlayerUI->SetFritter(m_PlayerData.Fritter);
	m_PlayerUI->SetSocks(m_PlayerData.Socks);
	m_PauseUI = m_Scene->GetViewport()->CreateUIWindow<CPauseUI>("PauseUI");
	m_PauseUI->SetEnable(false);

	if (m_IsLoading)
	{
		CGameObject* delObj = m_Scene->FindObject("Temp");
		delObj->Destroy();
		return;
	}
	LoadSpongebobAnim();
	LoadPatrickAnim();
	LoadSandyAnim();

	ChangeSpongebob();

	CWeapon* weapon = m_Scene->CreateObject<CWeapon>("Temp");
	AddChildToSocket("Weapon", weapon);
	m_WeaponMesh = (CAnimationMeshComponent*)weapon->GetRootComponent();
	m_WeaponMesh->SetEnable(false);

	LoadCharacter();
}

bool CPlayer::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_NavAgent = CreateComponent<CNavigationAgent3D>("NavAgent");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");
	//m_Cube = CreateComponent<CColliderCube>("Cube");
	m_HeadCube = CreateComponent<CColliderCube>("HeadCube");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Rigid);
	m_Mesh->AddChild(m_Arm);
	//m_Mesh->AddChild(m_Cube);
	m_Mesh->AddChild(m_HeadCube);
	m_Arm->AddChild(m_Camera);

	//m_Cube->SetCubeSize(500.f, 500.f, 500.f);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);

	m_Arm->SetTargetOffset(0.f, 150.f, 0.f);

	m_Rigid->SetGround(true);	//땅에 붙어있다고 설정
	//m_Rigid->SetGravity(true);

	m_HeadCube->SetEnable(false);
	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	CameraRotationKey();
	JumpCheck();

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
	SaveCharacter();
}

void CPlayer::Load(FILE* File)
{
	CGameObject::Load(File);
	m_IsLoading = true;
	LoadSpongebobAnim();
	LoadPatrickAnim();
	LoadSandyAnim();

	ChangeSpongebob();

	CWeapon* weapon = m_Scene->CreateObject<CWeapon>("LoadWeapon");
	AddChildToSocket("Weapon", weapon);
	m_WeaponMesh = (CAnimationMeshComponent*)weapon->GetRootComponent();
	m_WeaponMesh->SetEnable(false);

	LoadCharacter();
}

bool CPlayer::SaveCharacter()
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(SAVE_PATH);
	if (path)
	{
		strcpy_s(fullPath, path->PathMultibyte);
	}
	strcat_s(fullPath, "userData.pref");
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "wb");
	if (!file)
	{
		return false;
	}
	fwrite(&m_PlayerData.MaxHP, 4, 1, file);
	fwrite(&m_PlayerData.CurHP, 4, 1, file);
	fwrite(&m_PlayerData.Socks, 4, 1, file);
	fwrite(&m_PlayerData.Fritter, 4, 1, file);
	fwrite(&m_PlayerData.Glittering, 4, 1, file);
	fclose(file);
	return true;
}

bool CPlayer::LoadCharacter()
{
	char	fullPath[MAX_PATH] = {};
	const PathInfo* path = CPathManager::GetInst()->FindPath(SAVE_PATH);
	if (path)
	{
		strcpy_s(fullPath, path->PathMultibyte);
	}
	strcat_s(fullPath, "userData.pref");
	FILE* file = nullptr;
	fopen_s(&file, fullPath, "rb");
	if (!file)
	{
		return false;
	}
	fread(&m_PlayerData.MaxHP, 4, 1, file);
	fread(&m_PlayerData.CurHP, 4, 1, file);
	fread(&m_PlayerData.Socks, 4, 1, file);
	fread(&m_PlayerData.Fritter, 4, 1, file);
	fread(&m_PlayerData.Glittering, 4, 1, file);
	fclose(file);
	m_LoadData = m_PlayerData;
	return true;
}

void CPlayer::LoadSpongebobAnim()
{
	m_ReserveMesh[(int)EMain_Character::Spongebob] = CResourceManager::GetInst()->FindMesh("Spongebob");
	m_Anim[(int)EMain_Character::Spongebob] = m_Mesh->SetAnimation<CAnimation>("SponegebobAnimation");
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerIdle", "Spongebob_Idle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerWalk", "Spongebob_Walk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerAttack", "Spongebob_Attack", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerAttack", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerJumpDw", "Spongebob_JumpDw", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerJumpUp", "Spongebob_JumpUp", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBashStart", "Spongebob_BashStart", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerBashStart", this, &CPlayer::StartBash);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBashDw", "Spongebob_BashDw", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBash", "Spongebob_Bash", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("SpongebobBash", this, &CPlayer::ResetIdle);
	//전용 모션
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBounceStart", "Spongebob_BounceStart", 1.f, 1.f, false);
	//m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerBounceStart", this, &CPlayer::);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBounceLoop", "Spongebob_BounceLoop", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBounceLanding", "Spongebob_BounceLanding", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerBounceLanding", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBowl", "Spongebob_Bowl", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBowlThrow", "Spongebob_BowlThrow", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerBowlThrow", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerMissileStart", "Spongebob_MissileStart", 1.f, 1.f, false);
	//m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerMissileStart", this, &CPlayer::);
	//애니메이션 지속시간 변수 만들어서 지속끝나면 end함수 호출
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerMissileLoop", "Spongebob_MissileLoop", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerMissileEnd", "Spongebob_MissileEnd", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerMissileEnd", this, &CPlayer::ResetIdle);
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

	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
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

	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
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
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
	float angle = m_Camera->GetWorldRot().y -90.f;
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
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
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
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
}

void CPlayer::Jump()
{
	if (!m_Rigid->GetGround())
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
	if (!m_Rigid->GetGround() && !m_IsDoubleJump)
	{
		m_IsDoubleJump = true;
	}
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerJumpUp");
	m_Rigid->SetGround(false);
	m_Rigid->AddForce(0, 500.f);
	m_Rigid->SetVelocityY(500.f);
}

void CPlayer::JumpCheck()
{
	//땅에 붙어있을때만 높이 갱신
	if (m_Rigid->GetGround())
	{
		CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
		float Y = Nav->GetHeight(GetWorldPos());
		if (Y != FLT_MAX)
		{
			SetWorldPositionY(Y);
		}
	}
	//플레이어의 속력이 음수가 되면 애니메이션 전환
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerJumpUp"
		&& m_Rigid->GetVelocity().y < 0.f)
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerJumpDw");
	}
	//아래로 내려오고 있을때 현재 높이와 지형의 차이가 50미만이면 아이들로 전환
	if (m_Rigid->GetVelocity().y < 0.f)
	{
		CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
		float Y = Nav->GetHeight(GetWorldPos());
		if (Y != FLT_MAX && GetWorldPos().y - Y < 50.f)
		{
			SetWorldPositionY(Y);
			m_Rigid->SetGround(true);
			if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerJumpUp")
			{
				m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBash");
				//충돌체 생성
			}
			else
			{
				ResetIdle();
			}
		}
	}
}

void CPlayer::AttackKey()
{
}

void CPlayer::CameraRotationKey()
{
	const Vector2& mouseMove = CInput::GetInst()->GetMouseMove() * m_CameraSpeed * g_DeltaTime;
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
	if (!m_Rigid->GetGround())
	{
		return;
	}
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerJumpUp");
	m_Rigid->SetGround(false);
	m_Rigid->AddForce(0, 500.f);
	m_Rigid->SetVelocityY(500.f);
	m_HeadCube->SetEnable(true);
}

void CPlayer::Missile()
{
}

void CPlayer::Bowl()
{
	if (!m_Rigid->GetGround())
	{
		return;
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBowl");
	}
	m_Speed = 200.f;
}

void CPlayer::BowlThrow()
{	
	//불릿 바라보는 방향으로 발사
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerBowl")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBowlThrow");
		m_Speed = 500.f;
		CBullet* bullet = m_Scene->CreateObject<CBullet>("SpongeBobBowl");
		bullet->SetWorldRotationY(GetWorldRot().y - 90.f);
		bullet->SetWorldPosition(-30.f, 50.f, -100.f);
		bullet->SetDir(GetWorldPos());
		bullet->SetLifeTime(3.f);
	}
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
	if(m_PauseUI->GetEnable())
	{
		m_PauseUI->SetEnable(false);
		CEngine::GetInst()->SetTimeScale(1.f);
	}
	else
	{
		m_PauseUI->SetEnable(true);
		m_PauseUI->OpenUI();
		CEngine::GetInst()->SetTimeScale(0.f);
	}
}

void CPlayer::IngameUI()
{
	m_PlayerUI->SetAllOpacity(3.f);
}

void CPlayer::RClick()
{
	if (m_Rigid->GetGround())
	{
		return;
	}
	//엉찍 사운드
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		break;
	case EMain_Character::Patrick:
		break;
	case EMain_Character::Sandy:
		break;
	}
	m_Rigid->SetGravity(false);
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBashStart");
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

void CPlayer::StartBash()
{
	m_Rigid->SetGravity(true);
	m_Rigid->AddForce(0, -500.f);
	m_Rigid->SetVelocityY(-500.f);
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBashDw");
}

void CPlayer::ResetIdle()
{
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerIdle");
	m_WeaponMesh->SetEnable(false);
	m_IsDoubleJump = false;
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


