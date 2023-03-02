#include "Patric.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"

CPatric::CPatric()
{
	SetTypeID<CPatric>();

	m_ObjectTypeName = "Patric";
}

CPatric::CPatric(const CPatric& Obj)
	: CGameObject(Obj)
{
}

CPatric::~CPatric()
{
}

void CPatric::Start()
{
}

bool CPatric::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Patric_Npc");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("PatricNpcAnimation");

    m_Animation->AddAnimation("Patric_Npc_Confused", "Patric_Npc_Confused", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Default", "Patric_Npc_Default", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Excited", "Patric_Npc_Excited", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Scowl_Start", "Patric_Npc_Scowl_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Scowl_Loop", "Patric_Npc_Scowl_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Talk", "Patric_Npc_Talk", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Thinking_Start", "Patric_Npc_Thinking_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Thinking_Loop", "Patric_Npc_Thinking_Loop", 1.f, 1.f, true);

    m_Animation->SetCurrentEndFunction("Patric_Npc_Scowl_Start", this, &CPatric::ChangeAnim_Scowl_Loop);
    m_Animation->SetCurrentEndFunction("Patric_Npc_Thinking_Start", this, &CPatric::ChangeAnim_Thinking_Loop);

    m_Animation->SetCurrentAnimation("Patric_Npc_Default");

    return true;
}

void CPatric::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CPatric::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CPatric* CPatric::Clone() const
{
    return new CPatric(*this);
}

void CPatric::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CPatric::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CPatric::ChangeAnim_Confused()
{
    m_Animation->ChangeAnimation("Patric_Npc_Confused");
}

void CPatric::ChangeAnim_Default()
{
    m_Animation->ChangeAnimation("Patric_Npc_Default");
}

void CPatric::ChangeAnim_Excited()
{
    m_Animation->ChangeAnimation("Patric_Npc_Excited");
}

void CPatric::ChangeAnim_Scowl_Start()
{
    m_Animation->ChangeAnimation("Patric_Npc_Scowl_Start");
}

void CPatric::ChangeAnim_Scowl_Loop()
{
    m_Animation->ChangeAnimation("Patric_Npc_Scowl_Loop");
}

void CPatric::ChangeAnim_Talk()
{
    m_Animation->ChangeAnimation("Patric_Npc_Talk");
}

void CPatric::ChangeAnim_Thinking_Start()
{
    m_Animation->ChangeAnimation("Patric_Npc_Thinking_Start");
}

void CPatric::ChangeAnim_Thinking_Loop()
{
    m_Animation->ChangeAnimation("Patric_Npc_Thinking_Loop");
}
