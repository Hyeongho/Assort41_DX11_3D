#include "Walk.h"
#include "../GameObject/Fodder.h"
#include "../GameObject/Hammer.h"
#include "Scene/Scene.h"

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
	m_Fodder = dynamic_cast<CFodder*>(m_Scene->FindObject("Fodder"));

	if (!m_Fodder)
	{
		return false;
	}

	m_Scene->FindObject("Fodder")->AddWorldPosition(1.f, 0.f, 1.f);

	return true;
}

bool CWalk::Run(CGameObject* Object)
{
	if (Object->GetObjectTypeName() == "Fodder")
	{
	}

	if (Object->GetObjectTypeName() == "Hammer")
	{
	}

	return true;
}
