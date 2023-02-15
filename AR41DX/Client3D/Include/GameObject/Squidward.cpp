#include "Squidward.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"

CSquidward::CSquidward()
{
	SetTypeID<CSquidward>();

	m_ObjectTypeName = "Squidward";
}

CSquidward::CSquidward(const CSquidward& Obj) : CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
}

CSquidward::~CSquidward()
{
}

void CSquidward::Start()
{
    CGameObject::Start();

    CInput::GetInst()->AddBindFunction<CSquidward>("F1", Input_Type::Up, this, &CSquidward::ChangeAnim_Angry_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F2", Input_Type::Up, this, &CSquidward::ChangeAnim_Annoyed_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F3", Input_Type::Up, this, &CSquidward::ChangeAnim_Happy_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F4", Input_Type::Up, this, &CSquidward::ChangeAnim_Hurt_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F5", Input_Type::Up, this, &CSquidward::ChangeAnim_Sarcastic_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F6", Input_Type::Up, this, &CSquidward::ChangeAnim_Talk, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F7", Input_Type::Up, this, &CSquidward::ChangeAnim_Talk_Idle, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F8", Input_Type::Up, this, &CSquidward::ChangeAnim_Idle, m_Scene);
}

bool CSquidward::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Squidward");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("SquidwardAnimation");

    m_Animation->AddAnimation("Squidward_Angry_Loop", "Squidward_Angry_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Angry_Start", "Squidward_Angry_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Annoyed_Loop", "Squidward_Annoyed_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Annoyed_Start", "Squidward_Annoyed_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Happy_Loop", "Squidward_Happy_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Happy_Start", "Squidward_Happy_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Hurt_Loop", "Squidward_Hurt_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Hurt_Start", "Squidward_Hurt_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Idle", "Squidward_Idle", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Sarcastic_Loop", "Squidward_Sarcastic_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Sarcastic_Start", "Squidward_Sarcastic_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Talk", "Squidward_Talk", 1.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Talk_Idle", "Squidward_Talk_Idle", 1.f, 1.f, true);

    m_Animation->SetCurrentEndFunction("Squidward_Angry_Start", this, &CSquidward::ChangeAnim_Angry_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Annoyed_Start", this, &CSquidward::ChangeAnim_Annoyed_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Happy_Start", this, &CSquidward::ChangeAnim_Happy_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Hurt_Start", this, &CSquidward::ChangeAnim_Hurt_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Sarcastic_Start", this, &CSquidward::ChangeAnim_Sarcastic_Loop);

    m_Animation->SetCurrentAnimation("Squidward_Idle");

    return true;
}

void CSquidward::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CSquidward::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CSquidward* CSquidward::Clone() const
{
    return new CSquidward(*this);
}

void CSquidward::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CSquidward::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CSquidward::ChangeAnim_Angry_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Angry_Loop");
}

void CSquidward::ChangeAnim_Angry_Start()
{
    m_Animation->ChangeAnimation("Squidward_Angry_Start");
}

void CSquidward::ChangeAnim_Annoyed_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Annoyed_Loop");
}

void CSquidward::ChangeAnim_Annoyed_Start()
{
    m_Animation->ChangeAnimation("Squidward_Annoyed_Start");
}

void CSquidward::ChangeAnim_Happy_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Happy_Loop");
}

void CSquidward::ChangeAnim_Happy_Start()
{
    m_Animation->ChangeAnimation("Squidward_Happy_Start");
}

void CSquidward::ChangeAnim_Hurt_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Hurt_Loop");
}

void CSquidward::ChangeAnim_Hurt_Start()
{
    m_Animation->ChangeAnimation("Squidward_Hurt_Start");
}

void CSquidward::ChangeAnim_Sarcastic_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Sarcastic_Loop");
}

void CSquidward::ChangeAnim_Sarcastic_Start()
{
    m_Animation->ChangeAnimation("Squidward_Sarcastic_Start");
}

void CSquidward::ChangeAnim_Talk()
{
    m_Animation->ChangeAnimation("Squidward_Talk");
}

void CSquidward::ChangeAnim_Talk_Idle()
{
    m_Animation->ChangeAnimation("Squidward_Talk_Idle");
}

void CSquidward::ChangeAnim_Idle()
{
    m_Animation->ChangeAnimation("Squidward_Idle");
}
