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

bool CFodHamAttack::Run() // ���⿡ �ٷ� ���� ¥�� �ȴ�. 
{
	m_Fodder = dynamic_cast<CFodder*>(m_Scene->FindObject("Fodder"));

	if (!m_Fodder)
	{
		return false;
	}

	Vector3 FodderPos = m_Scene->FindObject("Fodder")->GetWorldPos();
	Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

	Vector3 Dir = FodderPos - PlayerPos;

	Dir.y = 0.f; // Y������ �̵� �Ұ�. 

	Dir.Normalize();

	// m_MoveSpeed�� Monster Ŭ�������� 100.f�� �Ǿ��ִ�. �ӵ� 2�� ������.
	m_Fodder->AddWorldPosition(Dir * 2 * 100.f * g_DeltaTime);

	return true;
}
