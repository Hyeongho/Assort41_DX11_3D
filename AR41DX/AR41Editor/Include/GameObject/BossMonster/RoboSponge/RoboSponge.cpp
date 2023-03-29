#include "RoboSponge.h"

#include "Animation/Animation.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Device.h"
#include "Engine.h"
#include "Input.h"
#include "Resource/Material/Material.h"
#include "Scene/Scene.h"
#include "RoboSponge_Knob.h"
#include "RoboSponge_AttackWords.h"
#include "../../Object/CBL/CBL_Floor.h"
#include "../../Object/CBL/CBL_Platform.h"
#include "../../Player.h"


/* RoboSponge Pattern
* HP 9점
* 3점 마다 아래 패턴 하나씩을 추가하여 실행.
* 남은 체력
* 7~9점 (1)
* 4~6점 (1, 2)
* 1~3점 (1, 2, 3)
* 
* 
* 1. 수직 공격 패턴 (AttackVerticPattern)
* MoveToPlatform			F1
* AttackVertic(1~2회)		F2
* MoveToCenter				F5
* ChasePlayer				
* 
* 2. 수평 공격 패턴 (AttackHorizPattern)
* AttackHoriz				F3
* AttackHorizEnd			F7
* 
* 3. 문자 공격 패턴 (AttackWordsPattern)
* MoveToPlatform			F1
* AttackWords(1~2회)		F4
* MoveToCenter				F5
* 
* 피격 시 (RoboSpongeAttacked)
* MoveToCenter				F5
* Damaged					F6
* 
* 플레이어 공격 성공 시 (RoboSpongeAttackSuccess)
* Victory					F8
*/	


CRoboSponge::CRoboSponge()
{
	SetTypeID<CRoboSponge>();

	m_ObjectTypeName = "Robo_Sponge";

	m_ChasePlayer = true;
	m_MoveToCenter = false;
	m_MoveToPlatform = false;
	m_TargetPlatformName = "";

	m_LR = ELRCheck::End;

	m_AttackHorizEnd = false;
	m_AttackHorizEndTimer = 0.f;

	m_AttackWords = false;
	m_AttackWordsCount = 0;
}

CRoboSponge::CRoboSponge(const CRoboSponge& Obj)
	: CBossMonster(Obj)
{
}

CRoboSponge::~CRoboSponge()
{
	SAFE_DELETE(m_BossData);
}

void CRoboSponge::Start()
{
	CBossMonster::Start();

#ifdef _DEBUG
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F1", Input_Type::Up, this, &CRoboSponge::MoveToPlatform, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F2", Input_Type::Up, this, &CRoboSponge::AttackVertric, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F3", Input_Type::Up, this, &CRoboSponge::AttackHoriz, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F4", Input_Type::Up, this, &CRoboSponge::AttackWords, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F5", Input_Type::Up, this, &CRoboSponge::MoveToCenter, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F6", Input_Type::Up, this, &CRoboSponge::Damaged, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F7", Input_Type::Up, this, &CRoboSponge::AttackHorizEnd, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F8", Input_Type::Up, this, &CRoboSponge::ChangeAnim_Victory, m_Scene);
#endif // _DEBUG

	// Animation
	CreateAnim();
}

bool CRoboSponge::Init()
{
	CBossMonster::Init();
	
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Robo_Sponge");


	// SetBossData
	m_BossData = new BossData;
	m_BossData->MaxHP = 10;
	m_BossData->CurHP = 10;


	// Animation
	CreateAnim();


	// Socket


	for (int i = 0; i < 12; i++) {
		const std::string& KnobName = "HitMarker" + std::to_string(i + 1);

		CRoboSponge_Knob* Knob = m_Scene->CreateObject<CRoboSponge_Knob>(KnobName);

		AddChildToSocket(KnobName, Knob);

		break;
	}




	Vector2 Ratio = CDevice::GetInst()->GetHdRsRatio();
	Ratio.x = 1.f / Ratio.x;
	Ratio.y = 1.f / Ratio.y;

	SetRelativeScale(Ratio);

    return true;
}

void CRoboSponge::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);

	// Player를 바라보는 액션
	if (m_ChasePlayer) {
		// Chase중에는 플레이어를 바라보게 한다.
		LookPlayer();
	}

	// Map 중앙으로 이동하는 액션
	if (m_MoveToCenter) {
		LookPlayer();

		Vector3 MyPos = GetWorldPos();

		float Dist = abs(m_MapCenterPoint.Distance(MyPos));

		if (Dist >= 10.f) {
			Vector3 Dir = m_MapCenterPoint - m_Mesh->GetWorldPos();

			Dir.y = 0.f; // Y축으로 이동 불가. 
			Dir.Normalize();

			AddWorldPosition(Dir * 1200.f * g_DeltaTime);
		}
		else {
			SetWorldPosition(m_MapCenterPoint);
			m_MoveToCenter = false;
		}
	}

	// TargetPlatform 앞으로 이동하는 액션
	if (m_MoveToPlatform) {
		Vector3 TargetPos = m_Scene->FindObject(m_TargetPlatformName)->GetWorldPos();
		TargetPos.y = 0.f;

		Vector3 MyPos = GetWorldPos();
		MyPos.y = 0.f;

		float Dist = abs(TargetPos.Distance(MyPos));

		if (Dist >= 1000.f) {
			Vector3 Dir = TargetPos- m_Mesh->GetWorldPos();

			Dir.y = 0.f; // Y축으로 이동 불가. 
			Dir.Normalize();

			AddWorldPosition(Dir * 800.f * g_DeltaTime);
		}
		else {
			m_MoveToPlatform = false;
		}
	}

	if (m_AttackHorizEnd) {
		m_AttackHorizEndTimer += g_DeltaTime;

		if (m_AttackHorizEndTimer >= 3.f) {
			m_AttackHorizEndTimer = 0.f;

			m_AttackHorizEnd = false;
			
			ChasePlayer();
			MoveToCenter();
			ChangeAnim_Idle();
		}
	}
}

void CRoboSponge::PostUpdate(float DeltaTime)
{
	CBossMonster::PostUpdate(DeltaTime);
}

CRoboSponge* CRoboSponge::Clone() const
{
	return new CRoboSponge(*this);
}

void CRoboSponge::Save(FILE* File)
{
	CBossMonster::Save(File);
}

void CRoboSponge::Load(FILE* File)
{
	CBossMonster::Load(File);
}

void CRoboSponge::AttackWords()
{
	if (m_AttackWordsCount == 0) {
		// 좌측(플레이어기준) 플랫폼 탐색
		m_TargetPlatformName;




		Vector3 DestPos = m_Scene->FindObject(m_TargetPlatformName)->GetWorldPos();
		DestPos.y += 300.f;

		// KAE 생성
		CRoboSponge_AttackWords* Kae = m_Scene->CreateObject<CRoboSponge_AttackWords>("AttakWords_KAE");
		Kae->SetWorldRotationY(GetWorldRot().y);
		Kae->SetDestPos(DestPos);

	}
	else if (m_AttackWordsCount == 1) {
		// 목표 플랫폼 탐색
		m_TargetPlatformName = "Platform1";

		Vector3 DestPos = m_Scene->FindObject(m_TargetPlatformName)->GetWorldPos();
		DestPos.y += 300.f;

		// RAE 생성
		CRoboSponge_AttackWords* Rah = m_Scene->CreateObject<CRoboSponge_AttackWords>("AttakWords_RAH");
		Rah->SetWorldRotationY(GetWorldRot().y);
		Rah->SetWordType(EAttackWordType::RAH);
		Rah->SetDestPos(DestPos);


	}
	else if (m_AttackWordsCount == 2) {
		// 우측(플레이어 기준) 플랫폼 탐색
		m_TargetPlatformName = "Platform2";

		Vector3 DestPos = m_Scene->FindObject(m_TargetPlatformName)->GetWorldPos();
		DestPos.y += 300.f;

		// TAE 생성
		CRoboSponge_AttackWords* Tae = m_Scene->CreateObject<CRoboSponge_AttackWords>("AttakWords_TAE");
		Tae->SetWorldRotationY(GetWorldRot().y);
		Tae->SetWordType(EAttackWordType::TAE);
		Tae->SetDestPos(DestPos);

	}
	else if (m_AttackWordsCount >= 3) {
		m_AttackWordsCount = 0;
		m_AttackWords = false;

		return;
	}

	m_AttackWordsCount++;
}

void CRoboSponge::AttackVertric()
{
	m_ChasePlayer = false;

	// 세로 공격
	

	if (m_LR == ELRCheck::End) {
		m_LR = (ELRCheck)(rand() % (int)ELRCheck::End);
	}

	if (m_LR == ELRCheck::Left) {
		ChangeAnim_AttackVerticLStart();
	}
	else if (m_LR == ELRCheck::Right) {
		ChangeAnim_AttackVerticRStart();
	}


}

void CRoboSponge::AttackHoriz()
{
	m_ChasePlayer = false;

	// 가로 공격
	m_LR = (ELRCheck)(rand() % (int)ELRCheck::End);

	if (m_LR == ELRCheck::Left) {
		ChangeAnim_AttackHorizL();
	}
	else if (m_LR == ELRCheck::Right) {
		ChangeAnim_AttackHorizR();
	}
}

void CRoboSponge::AttackHorizEnd()
{
	m_AttackHorizEnd = true;
}

void CRoboSponge::MoveToPlatform()
{
	m_MoveToPlatform = true;

	float minDist = 100000.f;

	for (int i = 0; i < 12; i++) {
		std::string PlatformName = "Platform" + std::to_string(i);
		CGameObject* Platform = m_Scene->FindObject(PlatformName);

		const Vector3& PlayerPos = m_Scene->GetPlayerObject()->GetWorldPos();

		float Dist = Platform->GetWorldPos().Distance(PlayerPos);

		if (minDist > Dist) {
			minDist = Dist;
			m_TargetPlatformName = PlatformName;
			m_TargetPlatformIdx = i;
		}
	}

	LookPlayer();
}

void CRoboSponge::MoveToCenter()
{
	// 제자리로 이동
	m_MoveToCenter = true;

	m_LR = ELRCheck::End;
	m_TargetPlatformName = "";
	m_TargetPlatformIdx = 0;
}

void CRoboSponge::ChasePlayer()
{
	m_ChasePlayer = true;
	
	ChangeAnim_Idle();
}

void CRoboSponge::RollTargetPlatform()
{
	CCBL_Platform* Platform = (CCBL_Platform*)m_Scene->FindObject(m_TargetPlatformName);

	// 문자 공격에 의한 것이라면 전방 회전
	// 문자 공격에 의한 것은 문자 클래스에서 직접 처리

	// 기타 손에 의한 공격이라면 좌/우 회전
	if (m_LR == ELRCheck::Left) {
		Platform->Roll(ERollDir::Right);
	}
	else {
		Platform->Roll(ERollDir::Left);
	}
}

void CRoboSponge::FindTargetPlatform()
{
	float minDist = 100000.f;

	for (int i = 0; i < 12; i++) {
		std::string PlatformName = "Platform" + std::to_string(i);
		CGameObject* Platform = m_Scene->FindObject(PlatformName);

		float Dist = GetWorldPos().Distance(Platform->GetWorldPos());

		if (minDist > Dist) {
			minDist = Dist;
			m_TargetPlatformName = PlatformName;
			m_TargetPlatformIdx = i;
		}
	}
}

void CRoboSponge::LookPlayer()
{
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (!Player)
		return;

	Vector3 PlayerPos = Player->GetWorldPos();

	float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);
}

void CRoboSponge::Damaged()
{
	MoveToCenter();
	ChangeAnim_Hit1();

	m_BossData->CurHP -= 1;
}

void CRoboSponge::Groggy()
{
	// 그로기 처리
	ChangeAnim_Hit2();
}

void CRoboSponge::CreateAnim()
{
	if (m_Animation)
		return;


	m_Animation = m_Mesh->SetAnimation<CAnimation>("RoboSpongeAnimation");

	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_L", "Robo_Sponge_Attack_Horiz_L", 55.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_L_Pose", "Robo_Sponge_Attack_Horiz_L_Pose", 0.1f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_R", "Robo_Sponge_Attack_Horiz_R", 55.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_R_Pose", "Robo_Sponge_Attack_Horiz_R_Pose", 0.1f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_L_Loop", "Robo_Sponge_Attack_Vertic_L_Loop", 22.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_L_Start", "Robo_Sponge_Attack_Vertic_L_Start", 30.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_R_Loop", "Robo_Sponge_Attack_Vertic_R_Loop", 22.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_R_Start", "Robo_Sponge_Attack_Vertic_R_Start", 30.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Words_Loop", "Robo_Sponge_Attack_Words_Loop", 14.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Words_Start", "Robo_Sponge_Attack_Words_Start", 12.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Hit1", "Robo_Sponge_Hit1", 40.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Hit2", "Robo_Sponge_Hit2", 42.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Hit2_Pose", "Robo_Sponge_Hit2_Pose", 56.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Idle", "Robo_Sponge_Idle", 28.f / 60.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Vertic_L_Hold", "Robo_Sponge_Vertic_L_Hold", 9.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Vertic_R_Hold", "Robo_Sponge_Vertic_R_Hold", 9.f / 60.f, 1.f, false);
	m_Animation->AddAnimation("Robo_Sponge_Victory", "Robo_Sponge_Victory", 55.f / 60.f, 1.f, false);

	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Horiz_L", this, &CRoboSponge::ChangeAnim_AttackHorizLPose);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Horiz_L_Pose", this, &CRoboSponge::AttackHorizEnd);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Horiz_R", this, &CRoboSponge::ChangeAnim_AttackHorizRPose);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Horiz_R_Pose", this, &CRoboSponge::AttackHorizEnd);

	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_L_Start", this, &CRoboSponge::ChangeAnim_AttackVerticLLoop);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_L_Loop", this, &CRoboSponge::ChangeAnim_VerticLHold);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_R_Start", this, &CRoboSponge::ChangeAnim_AttackVerticRLoop);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_R_Loop", this, &CRoboSponge::ChangeAnim_VerticRHold);

	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Words_Start", this, &CRoboSponge::ChangeAnim_AttackWordsLoop);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Words_Start", this, &CRoboSponge::AttackWords);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Words_Loop", this, &CRoboSponge::AttackWords);

	m_Animation->SetCurrentEndFunction("Robo_Sponge_Hit2", this, &CRoboSponge::ChangeAnim_Hit2Pose);

	m_Animation->SetCurrentAnimation("Robo_Sponge_Idle");
}

void CRoboSponge::ChangeAnim_AttackHorizL()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Horiz_L");
}

void CRoboSponge::ChangeAnim_AttackHorizLPose()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Horiz_L_Pose");
}

void CRoboSponge::ChangeAnim_AttackHorizR()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Horiz_R");
}

void CRoboSponge::ChangeAnim_AttackHorizRPose()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Horiz_R_Pose");
}

void CRoboSponge::ChangeAnim_AttackVerticLLoop()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Vertic_L_Loop");
}

void CRoboSponge::ChangeAnim_AttackVerticLStart()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Vertic_L_Start");
}

void CRoboSponge::ChangeAnim_AttackVerticRLoop()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Vertic_R_Loop");
}

void CRoboSponge::ChangeAnim_AttackVerticRStart()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Vertic_R_Start");
}

void CRoboSponge::ChangeAnim_AttackWordsLoop()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Words_Loop");
}

void CRoboSponge::ChangeAnim_AttackWordsStart()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Attack_Words_Start");
}

void CRoboSponge::ChangeAnim_Hit1()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Hit1");
}

void CRoboSponge::ChangeAnim_Hit2()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Hit2");
}

void CRoboSponge::ChangeAnim_Hit2Pose()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Hit2_Pose");
}

void CRoboSponge::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Idle");
}

void CRoboSponge::ChangeAnim_VerticLHold()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Vertic_L_Hold");

	RollTargetPlatform();
}

void CRoboSponge::ChangeAnim_VerticRHold()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Vertic_R_Hold");

	RollTargetPlatform();
}

void CRoboSponge::ChangeAnim_Victory()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Victory");
}