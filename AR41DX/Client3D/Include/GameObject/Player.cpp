#include "Player.h"
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
}

bool CPlayer::Init()
{
	CGameObject::Init();

	//SetWorldScale(5.f, 5.f, 5.f);

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

void CPlayer::MoveFront()
{
	m_Animation->ChangeAnimation("Sandy_Walk");

	AddWorldPosition(GetWorldAxis(AXIS_Z) * 100.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveBack()
{
	AddWorldPosition(GetWorldAxis(AXIS_Z) * -100.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveLeft()
{
	AddWorldRotationY(180.f * CEngine::GetInst()->GetDeltaTime());
}

void CPlayer::MoveRight()
{
}

void CPlayer::Jump()
{
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
