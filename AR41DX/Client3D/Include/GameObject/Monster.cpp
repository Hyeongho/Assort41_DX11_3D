
#include "Monster.h"
#include "Component/StaticMeshComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Input.h"
#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Device.h"
#include "Resource/Material/Material.h"
#include "Animation/Animation.h"

CMonster::CMonster() :
	m_DetectRange(false)
	, m_AttackRange(false)
	, m_MoveSpeed(100.f)
	, m_DeltaTime(0.f)
	, m_MonsterState(EMonsterState::Max)
	//, m_MonsterHP(100.f)
{
	SetTypeID<CMonster>();

	m_ObjectTypeName = "Monster";
}

CMonster::CMonster(const CMonster& Obj) : CGameObject(Obj)
, m_DetectRange(false)
, m_AttackRange(false)
, m_MoveSpeed(100.f)
, m_DeltaTime(0.f)
, m_MonsterState(EMonsterState::Max)
//, m_MonsterHP(100.f)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Rigid = (CRigidBody*)FindComponent("Rigid");
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CGameObject::Start();

}

bool CMonster::Init()
{
	CGameObject::Init();

	m_Rigid = CreateComponent<CRigidBody>("Rigid");

	//m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

	//SetRootComponent(m_Mesh);

	//m_Mesh->SetMesh("Monster");

	//m_Animation = m_Mesh->SetAnimation<CAnimation>("PlayerAnimation");

	//std::string	AnimName[2] =
	//{
	//	"PlayerWalk",
	//	"PlayerAttack"
	//};

	//int	AnimIndex = rand() % 2;
	//AnimIndex = 0;

	//m_Animation->AddAnimation(AnimName[AnimIndex], AnimName[AnimIndex],
	//	1.f, 1.f, true);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonster* CMonster::Clone() const
{
	return new CMonster(*this);
}

void CMonster::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CMonster::Load(FILE* File)
{
	CGameObject::Load(File);
}
