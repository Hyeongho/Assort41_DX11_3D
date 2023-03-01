#include "Attack.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "Scene/Scene.h"

CAttack::CAttack() :
	m_Scene(nullptr)
	, m_Fodder(nullptr)
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
		m_Fodder = dynamic_cast<CFodder*>(m_Scene->FindObject("Fodder"));

		if (!m_Fodder)
		{
			return false;
		}

		Vector3 FodderPos = m_Scene->FindObject("Fodder")->GetWorldPos();
		Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

		Vector3 Dir = FodderPos - PlayerPos;

		Dir.y = 0.f; // Y축으로 이동 불가. 

		Dir.Normalize();

		// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 2배 빠르게.
		m_Fodder->AddWorldPosition(Dir * 2 * 100.f * g_DeltaTime);

		return true;
	}

	if (Object->GetObjectTypeName() == "Hammer")
	{
		m_Hammer = dynamic_cast<CHammer*>(m_Scene->FindObject("Hammer"));

		if (!m_Hammer)
		{
			return false;
		}

		Vector3 HammerPos = m_Scene->FindObject("Hammer")->GetWorldPos();
		Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

		Vector3 Dir = HammerPos - PlayerPos;

		Dir.y = 0.f; // Y축으로 이동 불가. 

		Dir.Normalize();

		// m_MoveSpeed는 Monster 클래스에서 100.f로 되어있다. 속도 2배 빠르게.
		m_Hammer->AddWorldPosition(Dir * 2 * 100.f * g_DeltaTime);

		return true;
	}

	return true;
}
