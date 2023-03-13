#include "Chase.h"
#include "Fodder.h"
#include "Hammer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CChase::CChase()	:
	  m_Scene(nullptr)
	, m_Fodder(nullptr)
	, m_Hammer(nullptr)

{
}

CChase::~CChase()
{
}

bool CChase::Run()
{
	return true;
}

bool CChase::Run(CGameObject* Object)
{
	if (Object->GetObjectTypeName() == "Fodder")
	{
		m_Fodder = dynamic_cast<CFodder*>(CSceneManager::GetInst()->GetScene()->FindObject("Fodder"));

		if (!m_Fodder)
		{
			return false;
		}

		if(m_Fodder->GetDetectOn() == true)
		{
			//m_Fodder->Attack();

			Vector3 FodderPos = CSceneManager::GetInst()->GetScene()->FindObject("Fodder")->GetWorldPos();
			Vector3 PlayerPos = CSceneManager::GetInst()->GetScene()->FindObject("Player")->GetWorldPos();

			Vector3 Dir = PlayerPos - FodderPos;

			Dir.y = 0.f; // Y축으로 이동 불가. 

			Dir.Normalize();

			float FodderSpeed = m_Fodder->GetMoveSpeed();
			// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 2배 빠르게.
			m_Fodder->AddWorldPosition(Dir * 2 * FodderSpeed * g_DeltaTime);
		}
		
	}

	if (Object->GetObjectTypeName() == "Hammer")
	{
		m_Hammer = dynamic_cast<CHammer*>(CSceneManager::GetInst()->GetScene()->FindObject("Hammer"));

		if (!m_Hammer)
		{
			return false;
		}

		Vector3 HammerPos = CSceneManager::GetInst()->GetScene()->FindObject("Hammer")->GetWorldPos();
		Vector3 PlayerPos = CSceneManager::GetInst()->GetScene()->FindObject("Player")->GetWorldPos();

		Vector3 Dir = HammerPos - PlayerPos;

		Dir.y = 0.f; // Y축으로 이동 불가. 

		Dir.Normalize();

		// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 2배 빠르게.
		m_Hammer->AddWorldPosition(Dir * 2 * 100.f * g_DeltaTime);
	}

	return true;
}
