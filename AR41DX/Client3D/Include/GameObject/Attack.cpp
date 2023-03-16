#include "Attack.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "Scene/SceneManager.h"

CAttack::CAttack() :
	  m_Fodder(nullptr)
	, m_Hammer(nullptr)
{
}

CAttack::~CAttack()
{
}

bool CAttack::Run()
{
	return true;
}
	

bool CAttack::Run(CGameObject* Object)
{
	if (Object->GetObjectTypeName() == "Fodder")
	{
		m_Fodder = dynamic_cast<CFodder*>(CSceneManager::GetInst()->GetScene()->FindObject("Fodder"));

		if (!m_Fodder)
		{
			return false;
		}

		if(m_Fodder->GetAttackOn() == true)
		{
			Vector3 FodderPos = CSceneManager::GetInst()->GetScene()->FindObject("Fodder")->GetWorldPos();
			Vector3 PlayerPos = CSceneManager::GetInst()->GetScene()->FindObject("Player")->GetWorldPos();

			Vector3 Dir = FodderPos - PlayerPos;

			Dir.y = 0.f; // Y������ �̵� �Ұ�. 

			Dir.Normalize();

			// m_MoveSpeed�� Monster Ŭ�������� 100.f�� �Ǿ��ִ�. �ӵ� 2�� ������.
			m_Fodder->AddWorldPosition(Dir * 2 * 100.f * g_DeltaTime);
		}
		

		return true;
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

		Dir.y = 0.f; // Y������ �̵� �Ұ�. 

		Dir.Normalize();

		// m_MoveSpeed�� Monster Ŭ�������� 100.f�� �Ǿ��ִ�. �ӵ� 2�� ������.
		m_Hammer->AddWorldPosition(Dir * 2 * 100.f * g_DeltaTime);

		return true;
	}

	return true;
}
