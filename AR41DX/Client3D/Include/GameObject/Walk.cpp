#include "Walk.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

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
	m_Fodder = dynamic_cast<CFodder*>(CSceneManager::GetInst()->GetScene()->FindObject("Fodder"));
	

	if (!m_Fodder)
	{
		return false;
	}

	m_Fodder->AddWorldPosition(1.f, 0.f, 1.f);

	return true;
}
