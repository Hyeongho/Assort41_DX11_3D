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
#include "../../Player.h"


CRoboSponge::CRoboSponge()
{
	SetTypeID<CRoboSponge>();

	m_ObjectTypeName = "Robo_Sponge";

	m_AttackWordsCount = 0;
	m_TargetPlatformName = "";
	m_MoveToPlatForm = false;
}

CRoboSponge::CRoboSponge(const CRoboSponge& Obj) : CBossMonster(Obj)
{

	m_AttackWordsCount = Obj.m_AttackWordsCount;
	m_TargetPlatformName = Obj.m_TargetPlatformName;

}

CRoboSponge::~CRoboSponge()
{
}

void CRoboSponge::Start()
{
	CBossMonster::Start();

#ifdef _DEBUG
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F1", Input_Type::Up, this, &CRoboSponge::ChangeAnim_AttackHorizL, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F2", Input_Type::Up, this, &CRoboSponge::ChangeAnim_AttackHorizR, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F3", Input_Type::Up, this, &CRoboSponge::ChangeAnim_AttackVerticLStart, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F4", Input_Type::Up, this, &CRoboSponge::ChangeAnim_AttackVerticRStart, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F5", Input_Type::Up, this, &CRoboSponge::ChangeAnim_AttackWordsStart, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F6", Input_Type::Up, this, &CRoboSponge::ChangeAnim_Hit1, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F7", Input_Type::Up, this, &CRoboSponge::ChangeAnim_Hit2, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge>("F8", Input_Type::Up, this, &CRoboSponge::ChangeAnim_Idle, m_Scene);
	CInput::GetInst()->AddBindFunction<CRoboSponge>("F9", Input_Type::Up, this, &CRoboSponge::MoveToPlayer, m_Scene);
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

	for (int i = 0; i < 12; i++) {
		const std::string& KnobName = "HitMarker" + std::to_string(i + 1);

		CRoboSponge_Knob* Knob = m_Scene->CreateObject<CRoboSponge_Knob>(KnobName);

		AddChildToSocket(KnobName, Knob);
	}

    return true;
}

void CRoboSponge::Update(float DeltaTime)
{
	CBossMonster::Update(DeltaTime);


	// 항상 플레이어를 바라보게 한다.
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

	if (!Player)
		return;

	Vector3 PlayerPos = Player->GetWorldPos();

	float Degree = atan2(GetWorldPos().z - PlayerPos.z, GetWorldPos().x - PlayerPos.x);
	Degree = fabs(Degree * 180.f / PI - 180.f) - 90.f;

	SetWorldRotationY(Degree);

	char sz[255];
	memset(sz, NULL, sizeof(sz));
	sprintf_s(sz, "Degree : %f, Rot : %f\n", Degree, GetWorldRot().y);
	OutputDebugStringA(sz);
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
	// 문자 공격

	m_AttackWordsCount;
	// 

	if (m_AttackWordsCount == 0) {
		// KAE 생성

	}
	else if (m_AttackWordsCount == 1) {
		// RAE 생성
	}
	else if (m_AttackWordsCount == 2) {
		// TAE 생성
	}
	else if (m_AttackWordsCount == 3) {
		// 문자 낙하

	}
	else {
		m_AttackWordsCount = 0;
	}



}

void CRoboSponge::AttackVertric()
{
	// 세로 공격
}

void CRoboSponge::AttackHoriz()
{
	// 가로 공격
	if (m_LR == ELRCheck::Left) {
		ChangeAnim_AttackHorizL();
	}
	else if (m_LR == ELRCheck::Right) {
		ChangeAnim_AttackHorizR();
	}
}

void CRoboSponge::MoveToPlayer()
{
	// 플레이어 앞의 플랫폼으로 이동

	// 이동 후, 플레이어가 있는 플랫폼을 탐지

	// 플레이어가 플랫폼 기준으로 좌, 우 어디에 있는지 탐지

	float YRot = GetWorldRot().y;
	std::string TargetPlatformName = "";

	for (int i = 0; i < 12; i++) {
		float Radian = i * 30.f * PI / 180.f;

		std::string PlatformName = "Platform" + std::to_string(i);

		CGameObject* Platform = m_Scene->FindObject(PlatformName);
		float PYRot = Platform->GetWorldRot().y;

		if (YRot == -PYRot) {
			m_MoveToPlatForm = true;
			m_TargetPlatformPos = Platform->GetWorldPos();
		}
	}



}

void CRoboSponge::MoveToCenter()
{
	// 제자리로 이동
	// Map의 CenterPos를 찾아 해당 구역으로 이동 후 공격 대기

}

void CRoboSponge::Damaged()
{
	// 피해를 받았을 때 처리
}

void CRoboSponge::Groggy()
{
	// 그로기 처리
}

void CRoboSponge::CreateAnim()
{
	if (m_Animation)
		return;


	m_Animation = m_Mesh->SetAnimation<CAnimation>("RoboSpongeAnimation");

	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_L", "Robo_Sponge_Attack_Horiz_L", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_L_Pose", "Robo_Sponge_Attack_Horiz_L_Pose", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_R", "Robo_Sponge_Attack_Horiz_R", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Horiz_R_Pose", "Robo_Sponge_Attack_Horiz_R_Pose", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_L_Loop", "Robo_Sponge_Attack_Vertic_L_Loop", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_L_Start", "Robo_Sponge_Attack_Vertic_L_Start", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_R_Loop", "Robo_Sponge_Attack_Vertic_R_Loop", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Vertic_R_Start", "Robo_Sponge_Attack_Vertic_R_Start", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Words_Loop", "Robo_Sponge_Attack_Words_Loop", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Attack_Words_Start", "Robo_Sponge_Attack_Words_Start", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Hit1", "Robo_Sponge_Hit1", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Hit2", "Robo_Sponge_Hit2", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Hit2_Pose", "Robo_Sponge_Hit2_Pose", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Idle", "Robo_Sponge_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Vertic_L_Hold", "Robo_Sponge_Vertic_L_Hold", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Vertic_R_Hold", "Robo_Sponge_Vertic_R_Hold", 1.f, 1.f, true);
	m_Animation->AddAnimation("Robo_Sponge_Victory", "Robo_Sponge_Victory", 1.f, 1.f, true);

	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Horiz_L", this, &CRoboSponge::ChangeAnim_AttackHorizLPose);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Horiz_R", this, &CRoboSponge::ChangeAnim_AttackHorizRPose);

	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_L_Start", this, &CRoboSponge::ChangeAnim_AttackVerticLLoop);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_L_Loop", this, &CRoboSponge::ChangeAnim_VerticLHold);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_R_Start", this, &CRoboSponge::ChangeAnim_AttackVerticRLoop);
	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Vertic_R_Loop", this, &CRoboSponge::ChangeAnim_VerticRHold);

	m_Animation->SetCurrentEndFunction("Robo_Sponge_Attack_Words_Start", this, &CRoboSponge::ChangeAnim_AttackWordsLoop);

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
}

void CRoboSponge::ChangeAnim_VerticRHold()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Vertic_R_Hold");
}

void CRoboSponge::ChangeAnim_Victory()
{
	m_Animation->ChangeAnimation("Robo_Sponge_Victory");
}