
#include "Player2D.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/NavigationAgent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Bullet.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation2D.h"

CPlayer2D::CPlayer2D()
{
	SetTypeID<CPlayer2D>();

	m_ObjectTypeName = "Player2D";
}

CPlayer2D::CPlayer2D(const CPlayer2D& Obj)	:
	CGameObject(Obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("sprite");
	m_RightChild = (CSceneComponent*)FindComponent("RightChild");
	m_SpriteChild = (CSpriteComponent*)FindComponent("spriteChild");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_Arm = (CTargetArm*)FindComponent("Arm");
	m_Body = (CColliderOBB2D*)FindComponent("Body");
	m_NavAgent = (CNavigationAgent*)FindComponent("NavAgent");
}

CPlayer2D::~CPlayer2D()
{
}

void CPlayer2D::Start()
{
	CGameObject::Start();

	if (m_Scene)
		m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	CInput::GetInst()->AddBindFunction<CPlayer2D>("Rotation", Input_Type::Push,
		this, &CPlayer2D::RotationInv, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer2D>("RotationInv", Input_Type::Push,
		this, &CPlayer2D::Rotation, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer2D>("MoveUp", Input_Type::Push,
		this, &CPlayer2D::MoveUp, m_Scene);
	CInput::GetInst()->AddBindFunction<CPlayer2D>("MoveDown", Input_Type::Push,
		this, &CPlayer2D::MoveDown, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer2D>("MoveClick", Input_Type::Down,
		this, &CPlayer2D::MoveClick, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer2D>("Fire", Input_Type::Down,
		this, &CPlayer2D::Fire, m_Scene);

	CAnimation2D* Anim = m_Sprite->SetAnimation<CAnimation2D>("PlayerAnim");

	Anim->AddAnimation("Run", "PlayerRun");
	Anim->SetLoop("Run", true);

	Anim->AddAnimation("Idle", "PlayerIdle");
	Anim->SetLoop("Idle", true);

	Anim->SetCurrentAnimation("Idle");

}

bool CPlayer2D::Init()
{
	CGameObject::Init();

	m_Sprite = CreateComponent<CSpriteComponent>("sprite");
	m_RightChild = CreateComponent<CSceneComponent>("RightChild");
	m_SpriteChild = CreateComponent<CSpriteComponent>("spriteChild");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Arm = CreateComponent<CTargetArm>("Arm");
	m_Body = CreateComponent<CColliderOBB2D>("Body");
	m_NavAgent = CreateComponent<CNavigationAgent>("NavAgent");

	SetRootComponent(m_Body);

	m_Body->SetCollisionProfile("Player");

	m_Body->AddChild(m_Sprite);

	m_Sprite->AddChild(m_RightChild);

	m_Sprite->GetMaterial(0)->SetBaseColorUnsignedChar(255, 255, 0, 255);

	m_Sprite->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);

	Resolution RS = CDevice::GetInst()->GetResolution();

	float Width = (float)RS.Width / 2.f;
	float Height = (float)RS.Height / 2.f;

	m_Arm->SetTargetOffset(Vector3(-Width, -Height, 0.f));

	m_RightChild->AddChild(m_SpriteChild);

	m_Body->SetWorldPosition(500.f, 500.f);

	m_Sprite->SetRelativeScale(100.f, 100.f);
	m_Sprite->SetPivot(0.5f, 0.5f);
	m_Sprite->SetInheritRotZ(true);
	//m_Sprite->SetRelativeRotationZ(30.f);

	CMaterial* Material = m_Sprite->GetMaterial(0);

	Material->SetOpacity(0.5f);
	//Material->SetRenderState("DepthDisable");

	m_RightChild->SetRelativePosition(150.f, 0.f);
	m_RightChild->SetInheritRotZ(true);

	m_SpriteChild->SetRelativeScale(50.f, 50.f);
	m_SpriteChild->SetRelativePosition(100.f, 0.f);
	m_SpriteChild->SetInheritRotZ(true);

	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//m_Sprite->AddRelativeRotationZ(180.f * DeltaTime);

	//m_RightChild->AddRelativeRotationZ(360.f * DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer2D* CPlayer2D::Clone() const
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPlayer2D::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CPlayer2D::MoveUp()
{
	m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * 300.f * g_DeltaTime);
}

void CPlayer2D::MoveDown()
{
	m_Body->AddWorldPosition(m_Body->GetWorldAxis(AXIS_Y) * -300.f * g_DeltaTime);
}

void CPlayer2D::Rotation()
{
	m_Body->AddWorldRotationZ(360.f * g_DeltaTime);
}

void CPlayer2D::RotationInv()
{
	m_Body->AddWorldRotationZ(-360.f * g_DeltaTime);
}

void CPlayer2D::Fire()
{
	CBullet* Bullet = m_Scene->CreateObject<CBullet>("Bullet");

	Bullet->SetWorldPosition(GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	Bullet->SetCollisionProfileName("PlayerAttack");
}

void CPlayer2D::MoveClick()
{
	const Vector2&	MousePos = CInput::GetInst()->GetMouseWorldPos();

	if (m_NavAgent)
		m_NavAgent->Move(MousePos);
}
