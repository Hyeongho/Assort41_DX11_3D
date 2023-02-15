//#include "Fodder.h"
//#include "Component/StaticMeshComponent.h"
//#include "Component/AnimationMeshComponent.h"
//#include "Input.h"
//#include "Scene/Scene.h"
//#include "Scene/CameraManager.h"
//#include "Device.h"
//#include "Resource/Material/Material.h"
//#include "Animation/Animation.h"
//#include "Engine.h"
//
//CFodder::CFodder()
//{
//	SetTypeID<CFodder>();
//
//	m_ObjectTypeName = "Fodder";
//}
//
//CFodder::CFodder(const CFodder& Obj)
//{
//}
//
//CFodder::~CFodder()
//{
//}
//
//void CFodder::Start()
//{
//	CMonster::Start();
//}
//
//bool CFodder::Init()
//{
//	if (!CMonster::Init())
//	{
//		return false;
//	}
//
//	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
//
//	SetRootComponent(m_Mesh);
//
//	m_Mesh->SetMesh("Fodder");
//	m_Mesh->SetWorldPosition(100.f, 100.f, 100.f);
//	m_Animation = m_Mesh->SetAnimation<CAnimation>("FodderAnimation");
//
//	m_Animation->AddAnimation("Fodder_Attack", "Fodder_Attack", 1.f, 1.f, true);/*
//	m_Animation->AddAnimation("Fodder_Notice", "Fodder_Notice", 1.f, 1.f, true);
//	m_Animation->AddAnimation("Fodder_Walk", "Fodder_Walk", 1.f, 1.f, true);
//	m_Animation->AddAnimation("Fodder_Dead", "Fodder_Dead", 1.f, 1.f, true);*/
//
//	return true;
//}
//
//void CFodder::Update(float DeltaTime)
//{
//	CMonster::Update(DeltaTime);
//}
//
//void CFodder::PostUpdate(float DeltaTime)
//{
//	CMonster::PostUpdate(DeltaTime);
//}
//
//CFodder* CFodder::Clone() const
//{
//	return new CFodder(*this);
//}
//
//void CFodder::Save(FILE* File)
//{
//	CMonster::Save(File);
//}
//
//void CFodder::Load(FILE* File)
//{
//	CMonster::Load(File);
//}
