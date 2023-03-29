#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "PatrickObject.h"
#include "SpongebobMissile.h"
#include "Input.h"
#include "Engine.h"
#include "Device.h"
#include "PathManager.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ParticleComponent.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Scene/NavigationManager3D.h"
#include "Resource/Material/Material.h"
#include "Resource/Animation/SkeletonSocket.h"
#include "Animation/Animation.h"
#include "../UI/PauseUI.h"
#include "../UI/DialogUI.h"

CPlayer::CPlayer()
	: m_Speed(500.f)
	, m_InflictAngle(0.f)
	, m_SpaceTime(1.f)
	, m_LassoDistance(0.f)
	, m_KeyCount(0)
	, m_MainCharacter(EMain_Character::Max)
	, m_BellyAttackTime(0.f)
	, m_IsLoading(false)
	, m_IsDoubleJump(false)
	, m_OnCollision(false)
	, m_CanPickUp(false)
	, m_IsStop(false)
{
	SetTypeID<CPlayer>();
	m_ObjectTypeName = "Player";
}

CPlayer::CPlayer(const CPlayer& Obj) 
	: CGameObject(Obj)
	, m_Speed(Obj.m_Speed)
	, m_InflictAngle(0.f)
	, m_SpaceTime(1.f)
	, m_LassoDistance(0.f)
	, m_KeyCount(0)
	, m_MainCharacter(EMain_Character::Max)
	, m_IsDoubleJump(false)
	, m_OnCollision(false)
	, m_CanPickUp(false)
	, m_IsStop(false)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
	m_Cube = (CColliderOBB3D*)FindComponent("Cube");
	m_HeadCube = (CColliderCube*)FindComponent("HeadCube");
	m_TailCube = (CColliderCube*)FindComponent("TailCube");
	m_FrontCube = (CColliderOBB3D*)FindComponent("FrontCube");
	m_Particle = (CParticleComponent*)FindComponent("Particle");
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
	if (!m_Scene)
	{
		return;
	}
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	CInput::GetInst()->AddBindFunction<CPlayer>("F7", Input_Type::Down, this, &CPlayer::DebugF1, m_Scene);

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

	CInput::GetInst()->AddBindFunction<CPlayer>("Space", Input_Type::Down, this, &CPlayer::JumpDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Space", Input_Type::Push, this, &CPlayer::JumpPush, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Space", Input_Type::Up, this, &CPlayer::JumpUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("E", Input_Type::Down, this, &CPlayer::Headbutt, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("Q", Input_Type::Down, this, &CPlayer::Missile, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Tab", Input_Type::Down, this, &CPlayer::IngameUI, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("LClick", Input_Type::Down, this, &CPlayer::LClick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Down, this, &CPlayer::RClickDown, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Push, this, &CPlayer::RClickPush, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("RClick", Input_Type::Up, this, &CPlayer::RClickUp, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("F1", Input_Type::Down, this, &CPlayer::ChangeSpongebob, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F2", Input_Type::Down, this, &CPlayer::ChangePatrick, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F3", Input_Type::Down, this, &CPlayer::ChangeSandy, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer>("F6", Input_Type::Down, this, &CPlayer::Reset, m_Scene);

	m_Cube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionTest);
	m_Cube->SetCollisionCallback<CPlayer>(ECollision_Result::Release, this, &CPlayer::CollisionTestOut);
	m_HeadCube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionCube);
	m_TailCube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionCube);
	m_FrontCube->SetCollisionCallback<CPlayer>(ECollision_Result::Collision, this, &CPlayer::CollisionCube);

	m_PlayerUI = m_Scene->GetViewport()->CreateUIWindow<CPlayerUI>("PlayerUI");
	m_PauseUI = m_Scene->GetViewport()->CreateUIWindow<CPauseUI>("PauseUI");

	if (m_IsLoading)
	{
		CGameObject* delObj = m_Scene->FindObject("Temp");
		delObj->Destroy();
	}
	else
	{
		LoadCheck();
	}
	ChangeSpongebob();
	Reset();
}

bool CPlayer::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");
	m_Cube = CreateComponent<CColliderOBB3D>("Cube");
	m_HeadCube = CreateComponent<CColliderCube>("HeadCube");
	m_TailCube = CreateComponent<CColliderCube>("TailCube");
	m_FrontCube = CreateComponent<CColliderOBB3D>("FrontCube");
	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Mesh);

	m_Mesh->AddChild(m_Rigid);
	m_Mesh->AddChild(m_Arm);
	m_Mesh->AddChild(m_Cube);
	m_Mesh->AddChild(m_HeadCube);
	m_Mesh->AddChild(m_TailCube);
	m_Mesh->AddChild(m_FrontCube);
	m_Mesh->AddChild(m_Particle);
	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);

	m_Arm->SetTargetOffset(0.f, 150.f, 0.f);

	m_Rigid->SetGravity(true);

	m_Cube->SetRelativePositionY(70.f);
	m_Cube->SetCollisionProfile("Player");
	m_Cube->SetBoxHalfSize(50.f, 60.f, 20.f);
	m_Cube->SetInheritRotX(true);
	m_Cube->SetInheritRotY(true);
	m_Cube->SetInheritRotZ(true);

	m_HeadCube->SetCollisionProfile("PlayerAttack");
	m_HeadCube->SetRelativePositionY(175.f);
	m_HeadCube->SetCubeSize(100.f, 75.f, 100.f);

	m_TailCube->SetCollisionProfile("PlayerAttack");
	m_TailCube->SetRelativePositionY(25.f);

	m_FrontCube->SetCollisionProfile("PlayerAttack");
	m_FrontCube->SetRelativePosition(0.f, 70.f, -80.f);
	m_FrontCube->SetBoxHalfSize(60.f, 40.f, 60.f);
	m_FrontCube->SetInheritRotY(true);

	m_Particle->SetInheritRotY(true);
	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	CameraRotationKey();
	JumpCheck();
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerLassoStart")
	{
		m_LassoDistance -= 500.f * DeltaTime;
		if (m_LassoDistance < -400.f)
		{
			m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 105.f, m_LassoDistance + 505.f);
			m_FrontCube->AddRelativePositionZ((m_LassoDistance * 2.5f) * DeltaTime);
		}
	}
	if(m_FrontCube->GetEnable()&& m_MainCharacter==EMain_Character::Spongebob)
	{
		AddRelativeRotationY(-300.f* DeltaTime);
	}
}

CPlayer* CPlayer::Clone() const
{
	return new CPlayer(*this);
}

void CPlayer::Save(FILE* File)
{
	CGameObject::Save(File);
	fwrite(&m_RespawnPos, sizeof(Vector3), 1, File);
	SaveCharacter();
}

void CPlayer::Load(FILE* File)
{
	CGameObject::Load(File);
	fread(&m_RespawnPos, sizeof(Vector3), 1, File);
	m_IsLoading = true;
	LoadCheck();
}

int CPlayer::InflictDamage(int damage)
{
	int hp = --m_PlayerData.CurHP > 0? m_PlayerData.CurHP :0;
	m_PlayerUI->SetHp(hp);
	IngameUI();
	m_IsStop = true;
	m_Cube->SetEnable(false);
	if (hp <= 0)	//사망
	{
		switch (m_MainCharacter)
		{
		case EMain_Character::Spongebob:
			m_Scene->GetResource()->SoundStop("Spongebob_WalkLeft");
			m_Scene->GetResource()->SoundPlay("Spongebob_Death");
			break;
		case EMain_Character::Patrick:
			m_Scene->GetResource()->SoundStop("Patrick_Step");
			m_Scene->GetResource()->SoundPlay("Patrick_Death");
			break;
		case EMain_Character::Sandy:
			m_Scene->GetResource()->SoundStop("Sandy_Walk");
			m_Scene->GetResource()->SoundPlay("Sandy_Death");
			break;
		}
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerDeath");
	}
	else //피격 모션
	{		
		switch (m_MainCharacter)
		{
		case EMain_Character::Spongebob:
			m_Scene->GetResource()->SoundStop("Spongebob_WalkLeft");
			m_Scene->GetResource()->SoundPlay("Spongebob_Damage");
			break;
		case EMain_Character::Patrick:
			m_Scene->GetResource()->SoundStop("Patrick_Step");
			m_Scene->GetResource()->SoundPlay("Patrick_Damage");
			break;
		case EMain_Character::Sandy:
			m_Scene->GetResource()->SoundStop("Sandy_Walk");
			m_Scene->GetResource()->SoundPlay("Sandy_Damage");
			break;
		}
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerHit");
		float angle = m_InflictAngle==0.f?  GetWorldRot().y: m_InflictAngle;
		m_InflictAngle = 0.f;
		m_Rigid->SetGround(false);
		m_Rigid->AddForce(sinf(DegreeToRadian(angle))*400.f, 250.f, cosf(DegreeToRadian(angle))*400.f);
		m_Rigid->SetVelocity(sinf(DegreeToRadian(angle))*400.f, 250.f, cosf(DegreeToRadian(angle))*400.f);
	}
	return m_PlayerData.CurHP;
}

void CPlayer::Reset()
{
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerMissileLoop")
	{
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);
		m_Scene->GetResource()->SoundStop("Spongebob_BubbleLoop");
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleExplode");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerMissileEnd");
		return;
	}
	m_PlayerData.CurHP = m_PlayerData.MaxHP;
	m_PlayerUI->SetHp(m_PlayerData.CurHP);
	m_PlayerUI->SetMaxHp(m_PlayerData.MaxHP);
	m_PlayerUI->SetGlitter(m_PlayerData.ShinyFlower);
	m_PlayerUI->SetFritter(m_PlayerData.Spatula);
	m_PlayerUI->SetSocks(m_PlayerData.Socks);
	SetWorldPosition(m_RespawnPos);
	ResetIdle();
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
	fwrite(&m_PlayerData.Spatula, 4, 1, file);
	fwrite(&m_PlayerData.ShinyFlower, 4, 1, file);
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
	if(m_PlayerData.CurHP<3)
	{
		m_PlayerData.CurHP = 3;
	}
	fread(&m_PlayerData.Socks, 4, 1, file);
	fread(&m_PlayerData.Spatula, 4, 1, file);
	fread(&m_PlayerData.ShinyFlower, 4, 1, file);
	fclose(file);
	m_LoadData = m_PlayerData;
	return true;
}

void CPlayer::LoadSpongebobAnim()
{
	m_ReserveMesh[(int)EMain_Character::Spongebob] = CResourceManager::GetInst()->FindMesh("Spongebob");
	m_Mesh->SetMesh(m_ReserveMesh[(int)EMain_Character::Spongebob]);
	m_Anim[(int)EMain_Character::Spongebob] = m_Mesh->SetAnimation<CAnimation>("SponegebobAnimation");
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerIdle", "Spongebob_Idle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerWalk", "Spongebob_Walk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerAttack", "Spongebob_Attack", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerAttack", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerJumpDw", "Spongebob_JumpDw", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerJumpUp", "Spongebob_JumpUp", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBashStart", "Spongebob_BashStart", 1.f, 2.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerBashStart", this, &CPlayer::StartBash);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBashDw", "Spongebob_BashDw", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBash", "Spongebob_Bash", 1.f, 0.5f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerBash", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerHit", "Spongebob_Hit", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerHit", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerDeath", "Spongebob_Death", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerDeath", this, &CPlayer::Reset);
	//전용 모션
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBowl", "Spongebob_Bowl", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerBowlThrow", "Spongebob_BowlThrow", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerBowlThrow", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerMissileStart", "Spongebob_MissileStart", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerMissileStart", this, &CPlayer::MissileLaunch);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerMissileLoop", "Spongebob_MissileLoop", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Spongebob]->AddAnimation("PlayerMissileEnd", "Spongebob_MissileEnd", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Spongebob]->SetCurrentEndFunction<CPlayer>("PlayerMissileEnd", this, &CPlayer::ResetIdle);
}

void CPlayer::LoadPatrickAnim()
{
	m_ReserveMesh[(int)EMain_Character::Patrick] = CResourceManager::GetInst()->FindMesh("Patrick");
	m_Anim[(int)EMain_Character::Patrick] = m_Mesh->SetAnimation<CAnimation>("PatrickAnimation");
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerIdle", "Patrick_Idle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerWalk", "Patrick_Walk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerAttack", "Patrick_BellyAttack", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->SetCurrentEndFunction<CPlayer>("PlayerAttack", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerJumpUp", "Patrick_JumpUp", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerJumpDw", "Patrick_JumpDw", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerBashStart", "Patrick_SlamStart", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->SetCurrentEndFunction<CPlayer>("PlayerBashStart", this, &CPlayer::StartBash);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerBashDw", "Patrick_SlamLoop", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerBash", "Patrick_SlamEnd", 1.f, 0.5f, false);
	m_Anim[(int)EMain_Character::Patrick]->SetCurrentEndFunction<CPlayer>("PlayerBash", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerHit", "Patrick_Hit", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->SetCurrentEndFunction<CPlayer>("PlayerHit", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerDeath", "Patrick_Death", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->SetCurrentEndFunction<CPlayer>("PlayerDeath", this, &CPlayer::Reset);
	//전용 모션
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerPickUpIdle", "Patrick_PickUpIdle", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerPickUpWalk", "Patrick_PickUpWalk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerPickUp", "Patrick_PickUp", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddCurrentNotify<CPlayer>("PlayerPickUp", "PlayerPickUp", 0.6f, this, &CPlayer::Patrick_PickUp);
	m_Anim[(int)EMain_Character::Patrick]->SetCurrentEndFunction<CPlayer>("PlayerPickUp", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Patrick]->AddAnimation("PlayerThrow", "Patrick_Throw", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Patrick]->AddCurrentNotify<CPlayer>("PlayerThrow", "PlayerThrow",0.6f,this, &CPlayer::Patrick_Throw);
	m_Anim[(int)EMain_Character::Patrick]->SetCurrentEndFunction<CPlayer>("PlayerThrow", this, &CPlayer::ResetIdle);
}

void CPlayer::LoadSandyAnim()
{
	m_ReserveMesh[(int)EMain_Character::Sandy] = CResourceManager::GetInst()->FindMesh("Sandy");
	m_Anim[(int)EMain_Character::Sandy] = m_Mesh->SetAnimation<CAnimation>("SandyAnimation");
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerIdle", "Sandy_Idle", 1.f, 0.5f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerWalk", "Sandy_Walk", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerRun", "Sandy_Run", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerJumpDw", "Sandy_JumpDW", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerJumpUp", "Sandy_JumpUp", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerAttack", "Sandy_Karate_Chop", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Sandy]->SetCurrentEndFunction<CPlayer>("PlayerAttack", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerKick", "Sandy_Karate_Kick", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Sandy]->SetCurrentEndFunction<CPlayer>("PlayerKick", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerLassoStart", "Sandy_Lasso_Start", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Sandy]->SetCurrentEndFunction<CPlayer>("PlayerLassoStart", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerSwingLoop", "Sandy_Swing_Loop", 1.f, 1.f, true);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerHit", "Sandy_Hit", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Sandy]->SetCurrentEndFunction<CPlayer>("PlayerHit", this, &CPlayer::ResetIdle);
	m_Anim[(int)EMain_Character::Sandy]->AddAnimation("PlayerDeath", "Sandy_Death", 1.f, 1.f, false);
	m_Anim[(int)EMain_Character::Sandy]->SetCurrentEndFunction<CPlayer>("PlayerDeath", this, &CPlayer::Reset);
}

int CPlayer::AddHp()
{
	int Hp = 0;

	if (m_PlayerData.MaxHP > m_PlayerData.CurHP) {
		SetCurHP(m_PlayerData.CurHP + 1);
		Hp = m_PlayerData.CurHP;
	}

	IngameUI();

	return Hp;
}

int CPlayer::AddSock()
{
	SetSocks(m_PlayerData.Socks + 1);

	IngameUI();

	return m_PlayerData.Socks;
}

int CPlayer::AddSpatula()
{
	SetSpatula(m_PlayerData.Spatula + 1);

	IngameUI();

	return m_PlayerData.Spatula;
}

int CPlayer::AddShinyFlower(int Count)
{
	SetShinyFlower(m_PlayerData.ShinyFlower + Count);

	IngameUI();

	return m_PlayerData.ShinyFlower;
}

int CPlayer::AddItem(const EItemList& Item, int Count)
{
	int ItemCount = 0;

	switch (Item)
	{
	case EItemList::GoldenSpatula:
		SetSpatula(m_PlayerData.Spatula + Count);
		ItemCount = m_PlayerData.Spatula;
		break;
	case EItemList::ShinyFlower:
		SetShinyFlower(m_PlayerData.ShinyFlower + Count);
		ItemCount = m_PlayerData.ShinyFlower;
		break;
	case EItemList::Sock:
		SetSocks(m_PlayerData.Socks + Count);
		ItemCount = m_PlayerData.Socks;
		break;
	case EItemList::UnderWear:
		SetCurHP(m_PlayerData.CurHP + Count);
		ItemCount = m_PlayerData.CurHP;
		break;
	}

	IngameUI();

	return ItemCount;
}

bool CPlayer::SubSock(int Count)
{
	int Sock = m_PlayerData.Socks;

	if (Count > Sock)
		return false;

	Sock -= Count;

	SetSocks(Sock);

	IngameUI();

	return true;
}

bool CPlayer::SubSpatula(int Count)
{
	int Spatula = m_PlayerData.Spatula;

	if (Count > Spatula)
		return false;

	Spatula -= Count;

	SetSpatula(Spatula);

	IngameUI();

	return true;
}

bool CPlayer::SubShinyFlower(int Count)
{
	int ShinyFlower = m_PlayerData.ShinyFlower;

	if (Count > ShinyFlower)
		return false;

	ShinyFlower -= Count;

	SetShinyFlower(ShinyFlower);

	IngameUI();

	return true;
}

void CPlayer::LoadCheck()
{
	LoadSpongebobAnim();
	LoadPatrickAnim();
	LoadSandyAnim();
	LoadCharacter();
}

void CPlayer::KeyDown()
{
	if (m_IsStop || !m_Rigid->GetGround())
	{
		return;
	}
	if (m_KeyCount == 0)
	{
		switch (m_MainCharacter)
		{
		case EMain_Character::Spongebob:
			m_Scene->GetResource()->SoundStop("Spongebob_WalkLeft");
			m_Scene->GetResource()->SoundPlay("Spongebob_WalkLeft");
			break;
		case EMain_Character::Patrick:
			CResourceManager::GetInst()->SoundStop("Patrick_Step");
			CResourceManager::GetInst()->SoundPlay("Patrick_Step");
			break;
		case EMain_Character::Sandy:
			m_Scene->GetResource()->SoundStop("Sandy_Walk");
			m_Scene->GetResource()->SoundPlay("Sandy_Walk");
			break;
		}
	}
	++m_KeyCount;
}

void CPlayer::MoveFront()
{
	if(m_IsStop)
	{
		return;
	}
	float angle = m_Camera->GetWorldRot().y;
	if (m_WallCollision.Dest)
	{
		int differ = (int)abs(GetWorldRot().y-angle)%360;
		if(differ>100&&differ<200)
		{
			return;
		}
		else if (differ <= 100||(differ >=200 && differ < 280))
		{
			float distance = m_Cube->GetInfo().Length[0] * 0.85f;
			//float distance = GetWorldPos().Distance(m_WallCollision.HitPoint);
			AddWorldPosition(GetWorldAxis(AXIS_Z) * distance);
		}
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
	else if(m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerPickUpIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerPickUpWalk");
	}
	SetWorldRotationY(angle+180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CPlayer::MoveBack()
{
	if (m_IsStop)
	{
		return;
	}
	float angle = m_Camera->GetWorldRot().y - 180.f;
	if (m_WallCollision.Dest)
	{
		int differ = (int)abs(GetWorldRot().y - angle) % 360;
		if (differ > 100 && differ < 200)
		{
			return;
		}
		else if (differ <= 100 || (differ >= 200 && differ < 280))
		{
			float distance = m_Cube->GetInfo().Length[0] * 0.85f;
			//float distance = GetWorldPos().Distance(m_WallCollision.HitPoint);
			AddWorldPosition(GetWorldAxis(AXIS_Z) * distance);
		}
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
	else if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerPickUpIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerPickUpWalk");
	}
	SetWorldRotationY(angle+180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CPlayer::MoveLeft()
{
	if (m_IsStop)
	{
		return;
	}
	float angle = m_Camera->GetWorldRot().y - 90.f;
	if (m_WallCollision.Dest)
	{
		int differ = (int)abs(GetWorldRot().y - angle) % 360;
		if (differ > 100 && differ < 200)
		{
			return;
		}
		else if (differ <= 100 || (differ >= 200 && differ < 280))
		{
			float distance = m_Cube->GetInfo().Length[0] * 0.85f;
			//float distance = GetWorldPos().Distance(m_WallCollision.HitPoint);
			AddWorldPosition(GetWorldAxis(AXIS_Z) * distance);
		}
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
	else if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerPickUpIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerPickUpWalk");
	}
	SetWorldRotationY(angle + 180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CPlayer::MoveRight()
{
	if (m_IsStop)
	{
		return;
	}
	float angle = m_Camera->GetWorldRot().y + 90.f;
	if (m_WallCollision.Dest)
	{
		int differ = (int)abs(GetWorldRot().y - angle) % 360;
		if (differ > 100 && differ < 200)
		{
			return;
		}
		else if (differ <= 100 || (differ >= 200 && differ < 280))
		{
			float distance = m_Cube->GetInfo().Length[0] * 0.85f;
			//float distance = GetWorldPos().Distance(m_WallCollision.HitPoint);
			AddWorldPosition(GetWorldAxis(AXIS_Z) * distance);
		}
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerWalk");
	}
	else if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerPickUpIdle")
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerPickUpWalk");
	}
	SetWorldRotationY(angle+180.f);
	AddWorldPositionX(sinf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
	AddWorldPositionZ(cosf(DegreeToRadian(angle)) * m_Speed * g_DeltaTime);
}

void CPlayer::KeyUp()
{
	--m_KeyCount;
	if (m_IsStop)
	{
		return;
	}
	if (m_KeyCount == 0)
	{
		switch (m_MainCharacter)
		{
		case EMain_Character::Spongebob:
			m_Scene->GetResource()->SoundStop("Spongebob_WalkLeft");
			break;
		case EMain_Character::Patrick:
			m_Scene->GetResource()->SoundStop("Patrick_Step");
			break;
		case EMain_Character::Sandy:
			m_Scene->GetResource()->SoundStop("Sandy_Walk");
			break;
		}
		ResetIdle();
	}
}

void CPlayer::JumpDown()
{
	if(m_IsDoubleJump|| m_IsStop)
	{
		return;
	}
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		m_Scene->GetResource()->SoundStop("Spongebob_WalkLeft");
		m_Scene->GetResource()->SoundPlay("Spongebob_Jump");
		break;
	case EMain_Character::Patrick:
		if (m_Weapon->GetRootComponent()->GetEnable())
		{
			return;
		}
		m_Scene->GetResource()->SoundStop("Patrick_Step");
		m_Scene->GetResource()->SoundPlay("Patrick_Jump");
		break;
	case EMain_Character::Sandy:
		m_Scene->GetResource()->SoundStop("Sandy_Walk");
		m_Scene->GetResource()->SoundPlay("Sandy_Jump");
		break;
	}
	if (!m_Rigid->GetGround() && !m_IsDoubleJump)
	{
		m_IsDoubleJump = true;
	}
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerJumpUp");
	m_Rigid->SetGround(false);
	m_Rigid->AddForce(0, 400.f);
	m_Rigid->SetVelocityY(400.f);
}

void CPlayer::JumpPush()
{
	if (m_MainCharacter != EMain_Character::Sandy)
	{
		return;
	}
	if(m_SpaceTime>0.f&&!m_Rigid->GetGround())
	{
		m_SpaceTime -= g_DeltaTime;
		if (m_SpaceTime <= 0.f)
		{
			m_Scene->GetResource()->SoundPlay("Sandy_Hovering");
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerSwingLoop");
			m_Weapon->Lasso("Lasso_Copter");
			m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 180.f, 0.f);
		}
	}
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerSwingLoop")
	{
		m_Rigid->SetGravityForce(-150.f);
	}
}

void CPlayer::JumpUp()
{
	if (m_MainCharacter != EMain_Character::Sandy)
	{
		return;
	}
	m_Scene->GetResource()->SoundStop("Sandy_Hovering");
	m_SpaceTime = 1.f;
	m_Rigid->SetGravityForce(-500.f);
	m_Weapon->ResetIdle();
	m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 105.f, 105.f);
	if(m_Rigid->GetGround())
	{
		ResetIdle();
	}
	else
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerJumpDw");
	}
}

void CPlayer::JumpCheck()
{
	CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();
	float Y = Nav->GetHeight(GetWorldPos());
	//땅에 붙어있을때만 높이 갱신
	if (m_Rigid->GetGround()&& !m_OnCollision)
	{
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
	//아래로 내려오고 있을때 현재 높이와 지형의 차이가 30미만이면 아이들로 전환
	if (m_Rigid->GetVelocity().y < 0.f)
	{
		if (Y != FLT_MAX && GetWorldPos().y - Y < 30.f)
		{
			SetWorldPositionY(Y);
			m_Rigid->SetGround(true);
			BashCheck();
			if(m_KeyCount>0)
			{
				switch (m_MainCharacter)
				{
				case EMain_Character::Spongebob:
					m_Scene->GetResource()->SoundPlay("Spongebob_WalkLeft");
					break;
				case EMain_Character::Patrick:
					CResourceManager::GetInst()->SoundPlay("Patrick_Step");
					break;
				case EMain_Character::Sandy:
					m_Scene->GetResource()->SoundPlay("Sandy_Walk");
					break;
				}
			}
		}
	}
}

void CPlayer::CameraRotationKey()
{
	Vector2 mouseMove = CInput::GetInst()->GetMouseMove() * m_Camera->GetCameraSpeed() * g_DeltaTime;
	mouseMove.x = m_Camera->GetCameraHorizon() ? mouseMove.x : -mouseMove.x;
	mouseMove.y=m_Camera->GetCameraVertical() ? mouseMove.y : -mouseMove.y;
	m_Arm->AddRelativeRotationY(mouseMove.x);
	m_Arm->AddRelativeRotationX(mouseMove.y);
	if (m_Arm->GetRelativeRot().x > 70.f)
	{
		m_Arm->SetRelativeRotationX(70.f);
	}
	else if (m_Arm->GetRelativeRot().x < -20.f)
	{
		m_Arm->SetRelativeRotationX(-20.f);
	}
}

void CPlayer::Headbutt()
{
	if (m_IsStop)
	{
		return;
	}
	if (!m_Rigid->GetGround()|| CEngine::GetInst()->GetTimeScale() == 0.f)
	{
		return;
	}
	m_IsStop = true;
	m_Scene->GetResource()->SoundStop("Spongebob_WalkLeft");
	m_Scene->GetResource()->SoundPlay("Spongebob_DoubleJump");
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerJumpUp");
	m_Rigid->SetGround(false);
	m_Rigid->AddForce(0, 800.f);
	m_Rigid->SetVelocityY(800.f);
	m_HeadCube->SetEnable(true);
}

void CPlayer::Missile()
{
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
	{
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleCharge");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerMissileStart");
	}
}

void CPlayer::MissileLaunch()
{
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerMissileStart")
	{
		m_IsStop = true;
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleLaunch");
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleLoop");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerMissileLoop");
		CSpongebobMissile* missile= m_Scene->CreateObject<CSpongebobMissile>("SpongebobMissile");
		float angle = GetWorldRot().y;
		missile->SetWorldPosition(GetWorldPos());
		missile->SetAngle(angle);
	}
}

void CPlayer::Patrick_PickUp()
{
	m_Weapon->GetRootComponent()->SetEnable(true);
}

void CPlayer::Patrick_Throw()
{
	CPatrickObject* PatrickObject = m_Scene->CreateObject<CPatrickObject>("PatrickObject");
	float angle = GetWorldRot().y;
	PatrickObject->SetWorldPosition(GetWorldPos());
	PatrickObject->AddWorldPositionY(100.f);
	PatrickObject->SetAngle(angle - 180.f);
	m_Weapon->GetRootComponent()->SetEnable(false);
}

void CPlayer::IngameUI()
{
	m_PlayerUI->SetAllOpacity(3.f);
}

void CPlayer::LClick()
{
	if (m_IsStop)
	{
		return;
	}
	if (m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI")->GetIsActive()||
		CEngine::GetInst()->GetTimeScale() == 0.f)
	{
		return;
	}
	m_FrontCube->SetRelativeRotationY(0.f);
	m_FrontCube->SetRelativePositionZ(-80.f);
	m_FrontCube->SetEnable(true);
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleSpin");
		m_Weapon->GetRootComponent()->SetEnable(true);
		m_Particle->SetRelativePosition(-50.f, 50.f, -80.f);
		m_Particle->ChangeParticle("SpongebobAtk");
		break;
	case EMain_Character::Patrick:
	{
		if (m_Weapon->GetRootComponent()->GetEnable())
		{
			return;
		}
		m_Scene->GetResource()->SoundPlay("Patrick_Attack");
		//m_Particle->ChangeParticle("PatrickAtk");
		float angle = GetWorldRot().y-180.f;
		m_Rigid->SetGround(false);
		m_Rigid->AddForce(sinf(DegreeToRadian(angle)) * 150.f, 200.f, cosf(DegreeToRadian(angle)) * 150.f);
		m_Rigid->SetVelocity(sinf(DegreeToRadian(angle)) * 150.f, 200.f, cosf(DegreeToRadian(angle)) * 150.f);
		break;
	}
	case EMain_Character::Sandy:
		//m_Particle->ChangeParticle("SpongebobAtk");
		if (m_Rigid->GetGround())
		{
			m_Scene->GetResource()->SoundPlay("Sandy_Chop");
		}
		else
		{
			m_Scene->GetResource()->SoundPlay("Sandy_Kick");
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerKick");
			return;
		}
		break;
	}
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerAttack");
}

void CPlayer::RClickDown()
{
	if (m_IsStop)
	{
		return;
	}
	if (m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI")->GetIsActive() ||
		CEngine::GetInst()->GetTimeScale() == 0.f)
	{
		return;
	}
	if (m_Rigid->GetGround())
	{
		if(m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle"||
			m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerPickUpIdle")
		{
			switch (m_MainCharacter)
			{
			case EMain_Character::Spongebob:
				m_Scene->GetResource()->SoundPlay("Spongebob_BubbleBowl_Charge");
				break;
			case EMain_Character::Patrick:
				if (m_Weapon->GetRootComponent()->GetEnable()&& !m_CanPickUp)
				{
					m_IsStop = true;
					m_Scene->GetResource()->SoundPlay("Patrick_Throw");
					m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerThrow");
				}
				else if(m_CanPickUp)  
				{
					m_IsStop = true;
					m_Scene->GetResource()->SoundPlay("Patrick_Lift");
					m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerPickUp");
				}
				break;
			case EMain_Character::Sandy:
				m_IsStop = true;
				m_Scene->GetResource()->SoundPlay("Sandy_LassoAttack");
				m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerLassoStart");
				m_LassoDistance = 0.f;
				m_Mesh->GetSkeleton()->GetSocket("Weapon")->SetOffset(-5.f, 105.f, 105.f);
				m_Weapon->Lasso("Lasso_Start");
				m_Weapon->SetAngle(GetWorldRot().y);
				m_FrontCube->SetEnable(true);
				m_FrontCube->SetRelativePositionZ(-80.f);
				break;
			}
			return;
		}
	}
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		m_Scene->GetResource()->SoundPlay("Spongebob_BubbleBash");
		break;
	case EMain_Character::Patrick:
		if (m_Weapon->GetRootComponent()->GetEnable())
		{
			return;
		}
		m_Scene->GetResource()->SoundPlay("Patrick_Bash");
		break;
	case EMain_Character::Sandy:
		return;
	}
	m_Rigid->SetGravity(false);
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBashStart");
}

void CPlayer::RClickPush()
{
	if (m_IsStop)
	{
		return;
	}
	if (m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI")->GetIsActive() ||
		CEngine::GetInst()->GetTimeScale() == 0.f|| !m_Rigid->GetGround())
	{
		return;
	}
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerIdle")
		{
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBowl");
		}
		break;
	case EMain_Character::Patrick:
	case EMain_Character::Sandy:
		return;
	}
}

void CPlayer::RClickUp()
{	
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		m_Scene->GetResource()->SoundStop("Spongebob_BubbleBowl_Charge");
		if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerBowl")
		{
			m_IsStop = true;
			m_Scene->GetResource()->SoundPlay("Spongebob_BubbleBowl_Throw");
			m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBowlThrow");
			CBullet* bullet = m_Scene->CreateObject<CBullet>("SpongeBobBowl");
			float angle = GetWorldRot().y;
			bullet->SetWorldPosition(GetWorldPos());
			bullet->SetAngle(angle);
		}
		break;
	case EMain_Character::Patrick:
	case EMain_Character::Sandy:
		return;
	}
}

void CPlayer::StartBash()
{
	m_Rigid->SetGravity(true);
	m_Rigid->AddForce(0, -3000.f);
	m_Rigid->SetVelocityY(-3000.f);
	m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBashDw");
}

void CPlayer::BashCheck()
{
	if (m_Anim[(int)m_MainCharacter]->GetCurrentAnimationName() == "PlayerBashDw")
	{
		switch (m_MainCharacter)
		{
		case EMain_Character::Spongebob:
			m_Scene->GetResource()->SoundPlay("Spongebob_BubbleBash");
			break;
		case EMain_Character::Patrick:
			m_Scene->GetResource()->SoundPlay("Patrick_Slam");
			break;
		case EMain_Character::Sandy:
			m_Scene->GetResource()->SoundPlay("Sandy_BubbleBash");
			break;
		}
		m_Particle->ChangeParticle("BashBubble");
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerBash");
		m_TailCube->SetEnable(true);
		m_IsStop = true;
	}
	else
	{
		ResetIdle();
	}
}

void CPlayer::ResetIdle()
{
	if (!m_Rigid->GetGround())
	{
		return;
	}
	if (m_MainCharacter == EMain_Character::Spongebob)
	{
		m_Weapon->GetRootComponent()->SetEnable(false);
	}
	if (m_Weapon->GetRootComponent()->GetEnable() &&
		m_MainCharacter == EMain_Character::Patrick)
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerPickUpIdle");
	}
	else
	{
		m_Anim[(int)m_MainCharacter]->ChangeAnimation("PlayerIdle");
	}
	m_HeadCube->SetEnable(false);
	m_TailCube->SetEnable(false);
	m_FrontCube->SetEnable(false);
	m_Cube->SetEnable(true);
	m_Particle->SetRelativePosition(0.f, 0.f, 0.f);
	m_Particle->DeleteCurrentParticle();
	m_Rigid->SetVelocity(0.f, 0.f, 0.f);
	m_IsDoubleJump = false;
	m_IsStop = false;
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
	m_TailCube->SetCubeSize(300.f, 50.f, 300.f);

	if (m_Weapon)
	{
		m_Weapon->Destroy();
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
		m_Weapon->SetMesh("SpongebobWand");
		m_Weapon->SetWorldScale(0.5f, 0.5f, 0.5f);
		m_Weapon->GetRootComponent()->SetEnable(false);
	}
	else
	{
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
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
	m_TailCube->SetCubeSize(500.f, 50.f, 500.f);

	if (m_Weapon)
	{
		m_Weapon->Destroy();
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
		m_Weapon->SetMesh("Ice");
		m_Weapon->SetWorldScale(0.5f, 0.5f, 0.5f);
		m_Weapon->GetRootComponent()->SetEnable(false);
	}
	else
	{
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
	}
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
	m_Mesh->GetMaterial(1)->SetOpacity(0.5f);
	m_Mesh->GetMaterial(1)->SetRenderState("AlphaBlend");
	m_Anim[(int)m_MainCharacter]->Start();
	m_TailCube->SetCubeSize(300.f, 50.f, 300.f);

	if (m_Weapon)
	{
		m_Weapon->Destroy();
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
		m_Weapon->SetMesh("Lasso");
		m_Weapon->ResetIdle();
		m_Weapon->GetRootComponent()->SetEnable(false);
	}
	else
	{
		m_Weapon = m_Scene->CreateObject<CWeapon>("Temp");
		AddChildToSocket("Weapon", m_Weapon);
	}
}

void CPlayer::DebugF1()
{
	InflictDamage(1);
	//SetWorldPosition(16500.f, 0.f, 12200.f);
}

void CPlayer::CollisionTest(const CollisionResult& result)
{
	std::string name = result.Dest->GetCollisionProfile()->Name;
	if (name == "Wall"|| name == "Default")
	{
		float height = GetWorldPos().y>m_PrevPos.y? GetWorldPos().y : m_PrevPos.y;
		height-=result.HitPoint.y;
		if(height>=-10.f)
		{
			m_OnCollision = true;
			m_Rigid->SetGround(true);
			BashCheck();
		}
		else
		{
			m_OnCollision = false;
			m_WallCollision = result;
		}
	}
}

void CPlayer::CollisionTestOut(const CollisionResult& result)
{
	m_WallCollision.Dest = nullptr;
	m_WallCollision.Src = nullptr;
	m_WallCollision.HitPoint = Vector3(0.f,0.f,0.f);
	m_Rigid->SetGround(false);
	m_OnCollision = false;
	switch (m_MainCharacter)
	{
	case EMain_Character::Spongebob:
		m_Scene->GetResource()->SoundStop("Spongebob_WalkLeft");
		break;
	case EMain_Character::Patrick:
		m_Scene->GetResource()->SoundStop("Patrick_Step");
		break;
	case EMain_Character::Sandy:
		m_Scene->GetResource()->SoundStop("Sandy_Walk");
		break;
	}
}

void CPlayer::CollisionCube(const CollisionResult& result)
{
	std::string name = result.Dest->GetCollisionProfile()->Name;
	if(name == "Monster")
	{
		result.Dest->GetOwner()->InflictDamage(1);
	}
}