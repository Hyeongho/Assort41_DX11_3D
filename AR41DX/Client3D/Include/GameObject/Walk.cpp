#include "Walk.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/AnimationMeshComponent.h"

CWalk::CWalk()	:
	  m_Scene(nullptr)
	, m_Fodder(nullptr)
	, m_Hammer(nullptr)
{
}

CWalk::~CWalk()
{
}

bool CWalk::Run()
{
	return true;
}

bool CWalk::Run(CGameObject* Object)
{
	//if (!m_Fodder)
	//{
		//return false;
	//}

	//m_Fodder->AddWorldPosition(1.f, 0.f, 1.f);

	if (Object->GetObjectTypeName() == "Fodder")
	{
		m_Fodder = dynamic_cast<CFodder*>(CSceneManager::GetInst()->GetScene()->FindObject("Fodder"));

		//if (m_Fodder->GetDetectOn() == true)
		//{
		//	if (!m_Scene->FindObject("Player"))
		//		return false;

		//	Vector3 FodderPos = m_Fodder->GetWorldPos();
		//	Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

		//	Vector3 Dir = PlayerPos - FodderPos;

		//	Dir.y = 0.f; // Y축으로 이동 불가. 

		//	Dir.Normalize();

		//	// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 1.5배 빠르게.
			float FodderSpeed = m_Fodder->GetMoveSpeed();
			m_Fodder->AddWorldPosition(FodderSpeed * 1.5f * g_DeltaTime, 0.f, FodderSpeed * 1.5f * g_DeltaTime);
		//}

	}

	if (Object->GetObjectTypeName() == "Hammer")
	{
		m_Hammer = dynamic_cast<CHammer*>(CSceneManager::GetInst()->GetScene()->FindObject("Hammer"));

		//if (m_Hammer->GetDetectOn() == true)
		//{
		//	if (!m_Scene->FindObject("Player"))
		//		return false;

		//	Vector3 HammerPos = m_Hammer->GetWorldPos();
		//	Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

		//	Vector3 Dir = PlayerPos - HammerPos;

		//	Dir.y = 0.f; // Y축으로 이동 불가. 

		//	Dir.Normalize();

		//	// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 1.5배 빠르게.
		//	float HammerSpeed = m_Hammer->GetMoveSpeed();
		//	m_Hammer->AddWorldPosition(Dir * HammerSpeed * 1.5 * g_DeltaTime);
		//}
	}

	return true;
}
