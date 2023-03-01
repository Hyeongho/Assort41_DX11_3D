#include "FodHamAttack.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "Scene/Scene.h"

CFodHamAttack::CFodHamAttack() :
	  m_Scene(nullptr)
	, m_Fodder(nullptr)
	, m_Hammer(nullptr)
{
}

CFodHamAttack::~CFodHamAttack()
{
}

bool CFodHamAttack::Run() // 여기에 바로 로직 짜면 된다. 
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
