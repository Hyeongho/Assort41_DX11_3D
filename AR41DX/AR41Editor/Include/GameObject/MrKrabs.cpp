#include "MrKrabs.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"

CMrKrabs::CMrKrabs()
{
    SetTypeID<CMrKrabs>();

    m_ObjectTypeName = "MrKrabs";
}

CMrKrabs::CMrKrabs(const CMrKrabs& Obj) : CGameObject(Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
}

CMrKrabs::~CMrKrabs()
{
}

void CMrKrabs::Start()
{
    CGameObject::Start();

    CInput::GetInst()->AddBindFunction<CMrKrabs>("F1", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Angry_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CMrKrabs>("F2", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Deceptive_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CMrKrabs>("F3", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Greedy_Start, m_Scene);
    CInput::GetInst()->AddBindFunction<CMrKrabs>("F4", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Laughing, m_Scene);
    CInput::GetInst()->AddBindFunction<CMrKrabs>("F5", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Idle, m_Scene);
}

bool CMrKrabs::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("MrKrabs");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("MrKrabsAnimation");

    m_Animation->AddAnimation("MrKrabs_Angry_Loop", "MrKrabs_Angry_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Angry_Start", "MrKrabs_Angry_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Deceptive_Loop", "MrKrabs_Deceptive_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Deceptive_Start", "MrKrabs_Deceptive_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Greedy_Loop", "MrKrabs_Greedy_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Greedy_Start", "MrKrabs_Greedy_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Laughing", "MrKrabs_Laughing", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Idle", "MrKrabs_Idle", 1.f, 1.f, true);

    m_Animation->SetCurrentEndFunction("MrKrabs_Angry_Start", this, &CMrKrabs::ChangeAnim_Angry_Loop);
    m_Animation->SetCurrentEndFunction("MrKrabs_Deceptive_Start", this, &CMrKrabs::ChangeAnim_Deceptive_Loop);
    m_Animation->SetCurrentEndFunction("MrKrabs_Greedy_Start", this, &CMrKrabs::ChangeAnim_Greedy_Loop);

    m_Animation->SetCurrentAnimation("MrKrabs_Idle");

    return true;
}

void CMrKrabs::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CMrKrabs::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CMrKrabs* CMrKrabs::Clone() const
{
    return new CMrKrabs(*this);
}

void CMrKrabs::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CMrKrabs::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CMrKrabs::ChangeAnim_Angry_Loop()
{
    m_Animation->ChangeAnimation("MrKrabs_Angry_Loop");
}

void CMrKrabs::ChangeAnim_Angry_Start()
{
    m_Animation->ChangeAnimation("MrKrabs_Angry_Start");
}

void CMrKrabs::ChangeAnim_Deceptive_Loop()
{
    m_Animation->ChangeAnimation("MrKrabs_Deceptive_Loop");
}

void CMrKrabs::ChangeAnim_Deceptive_Start()
{
    m_Animation->ChangeAnimation("MrKrabs_Deceptive_Start");
}

void CMrKrabs::ChangeAnim_Greedy_Loop()
{
    m_Animation->ChangeAnimation("MrKrabs_Greedy_Loop");
}

void CMrKrabs::ChangeAnim_Greedy_Start()
{
    m_Animation->ChangeAnimation("MrKrabs_Greedy_Start");
}

void CMrKrabs::ChangeAnim_Laughing()
{
    m_Animation->ChangeAnimation("MrKrabs_Laughing");
}

void CMrKrabs::ChangeAnim_Idle()
{
    m_Animation->ChangeAnimation("MrKrabs_Idle");
}
