#include "Tiki_Stone.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/RigidBody.h"
#include "Component/ColliderCube.h"
#include "Input.h"
#include "Scene/Scene.h"

CTiki_Stone::CTiki_Stone()
{
}

CTiki_Stone::CTiki_Stone(const CTiki_Stone& Obj)
	: CGameObject(Obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Animation = (CAnimation*)FindComponent("Tiki_Stone");
	m_Cube = (CColliderCube*)FindComponent("Cube");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CTiki_Stone::~CTiki_Stone()
{
}

void CTiki_Stone::Start()
{
	CGameObject::Start();

#ifdef DEBUG
	CInput::GetInst()->AddBindFunction<CTiki_Stone>("F1", Input_Type::Up, this, &CTiki_Stone::ChangeAnim_Idle, m_Scene);
	CInput::GetInst()->AddBindFunction<CTiki_Stone>("F2", Input_Type::Up, this, &CTiki_Stone::ChangeAnim_Die, m_Scene);
#endif // DEBUG
}

bool CTiki_Stone::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Rigid = CreateComponent<CRigidBody>("Rigid");
	m_Cube = CreateComponent<CColliderCube>("Cube");

	SetRootComponent(m_Mesh);

	m_Mesh->SetMesh("Tiki_Stone");

	m_Animation = m_Mesh->SetAnimation<CAnimation>("TikiStoneAnimation");

	m_Animation->AddAnimation("Tiki_Stone_Idle", "Tiki_Stone_Idle", 1.f, 1.f, true);
	m_Animation->AddAnimation("Tiki_Stone_Die", "Tiki_Stone_Die", 1.f, 1.f, false);

	m_Animation->SetCurrentEndFunction<CTiki_Stone>("Tiki_Stone_Die", this, &CTiki_Stone::Tiki_Die);

	m_Animation->SetCurrentAnimation("Tiki_Stone_Idle");

    return true;
}

void CTiki_Stone::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTiki_Stone::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTiki_Stone* CTiki_Stone::Clone() const
{
	return new CTiki_Stone(*this);
}

void CTiki_Stone::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTiki_Stone::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CTiki_Stone::ChangeAnim_Idle()
{
	m_Animation->ChangeAnimation("Tiki_Stone_Idle");
}

void CTiki_Stone::ChangeAnim_Die()
{
	m_Animation->ChangeAnimation("Tiki_Stone_Die");
}

void CTiki_Stone::Tiki_Die()
{
	Destroy();
}
