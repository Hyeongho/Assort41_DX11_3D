#include "RoboSponge_Knob.h"

#include "Animation/Animation.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Device.h"
#include "Engine.h"
#include "Input.h"
#include "Resource/Material/Material.h"
#include "Scene/Scene.h"
#include "RoboSponge.h"

CRoboSponge_Knob::CRoboSponge_Knob()
{
	SetTypeID<CRoboSponge_Knob>();

	m_ObjectTypeName = "Robo_Sponge";
}

CRoboSponge_Knob::CRoboSponge_Knob(const CRoboSponge_Knob& Obj) :
	CGameObject(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");
}

CRoboSponge_Knob::~CRoboSponge_Knob()
{
}

void CRoboSponge_Knob::Start()
{
	CGameObject::Start();

#ifdef _DEBUG
	//CInput::GetInst()->AddBindFunction<CRoboSponge_Knob>("F1", Input_Type::Up, this, &CRoboSponge_Knob::DebugF1, m_Scene);
	//CInput::GetInst()->AddBindFunction<CRoboSponge_Knob>("F2", Input_Type::Up, this, &CRoboSponge_Knob::DebugF2, m_Scene);
#endif

	m_Collider->SetCollisionCallback<CRoboSponge_Knob>(ECollision_Result::Collision, this, &CRoboSponge_Knob::Collision_PlayerAttack);
}

bool CRoboSponge_Knob::Init()
{
	CGameObject::Init();

	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
	m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");


	SetRootComponent(m_Mesh);
	m_Mesh->AddChild(m_Collider);


	m_Mesh->SetMesh("GreenKnob");


	m_Collider->SetCollisionProfile("Knob");
	m_Collider->SetBoxHalfSize(m_Mesh->GetMeshSize() / 2.f);
	m_Collider->SetInheritRotX(true);
	m_Collider->SetInheritRotY(true);
	m_Collider->SetInheritRotZ(true);
	//SetWorldRotationX(90.f);

	return true;
}

void CRoboSponge_Knob::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CRoboSponge_Knob::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CRoboSponge_Knob* CRoboSponge_Knob::Clone() const
{
	return new CRoboSponge_Knob(*this);
}

void CRoboSponge_Knob::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CRoboSponge_Knob::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CRoboSponge_Knob::BreakKnob()
{
	// Sound
	CSound* Sound = CResourceManager::GetInst()->FindSound("AttackedKnob");

	if (Sound)
		Sound->Play();

	// KnobState 변경
	m_KnobState = EKnobState::Broken;
	
	// Mesh 변경
	m_Mesh->SetMesh("BrokenKnob");

	// BossMonster 피격 애니메이션 실행
	CRoboSponge* RoboSponge = (CRoboSponge*)m_Scene->FindObject("RoboSponge");

	if (RoboSponge) {
		RoboSponge->Damaged();
	}
}

void CRoboSponge_Knob::Collision_PlayerAttack(const CollisionResult& result)
{
	const std::string& DestName = result.Dest->GetCollisionProfile()->Name;

	if (strcmp("PlayerAttack", DestName.c_str()) == 0) {
		BreakKnob();
	}
}

void CRoboSponge_Knob::DebugF1()
{
	m_Mesh->SetMesh("GreenKnob");
}

void CRoboSponge_Knob::DebugF2()
{
	m_Mesh->SetMesh("BrokenKnob");
}
