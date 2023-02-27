#include "RoboSpongeSceneInfo.h"

CRoboSpongeSceneInfo::CRoboSpongeSceneInfo()
{
	m_ClassTypeName = "RoboSpongeSceneInfo";
}

CRoboSpongeSceneInfo::~CRoboSpongeSceneInfo()
{
}

bool CRoboSpongeSceneInfo::Init()
{
	CSceneInfo::Init();

	return true;
}
