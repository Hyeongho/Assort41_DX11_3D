//#include "Duplicatotron.h"
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
//CDuplicatotron::CDuplicatotron()
//{
//	SetTypeID<CDuplicatotron>();
//
//	m_ObjectTypeName = "Duplicatotron";
//}
//
//CDuplicatotron::CDuplicatotron(const CDuplicatotron& Obj)
//{
//}
//
//CDuplicatotron::~CDuplicatotron()
//{
//}
//
//void CDuplicatotron::Start()
//{
//	CMonster::Start();
//}
//
//bool CDuplicatotron::Init()
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
//	m_Mesh->SetMesh("Duplicatotron");
//	m_Mesh->SetWorldPosition(100.f, 100.f, 100.f);
//	m_Animation = m_Mesh->SetAnimation<CAnimation>("DuplicatotronAnimation");
//
//	m_Animation->AddAnimation("Duplicatotron_Attack", "Duplicatotron_Attack", 1.f, 1.f, true);/*
//	m_Animation->AddAnimation("Duplicatotron_Notice", "Duplicatotron_Notice", 1.f, 1.f, true);
//	m_Animation->AddAnimation("Duplicatotron_Walk", "Duplicatotron_Walk", 1.f, 1.f, true);
//	m_Animation->AddAnimation("Duplicatotron_Dead", "Duplicatotron_Dead", 1.f, 1.f, true);*/
//
//	return true;
//}
//
//void CDuplicatotron::Update(float DeltaTime)
//{
//	CMonster::Update(DeltaTime);
//}
//
//void CDuplicatotron::PostUpdate(float DeltaTime)
//{
//	CMonster::PostUpdate(DeltaTime);
//}
//
//CDuplicatotron* CDuplicatotron::Clone() const
//{
//	return new CDuplicatotron(*this);
//}
//
//void CDuplicatotron::Save(FILE* File)
//{
//	CMonster::Save(File);
//}
//
//void CDuplicatotron::Load(FILE* File)
//{
//	CMonster::Load(File);
//}
