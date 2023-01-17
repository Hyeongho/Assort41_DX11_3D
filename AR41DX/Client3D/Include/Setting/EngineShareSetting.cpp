#include "EngineShareSetting.h"
#include "Engine.h"
#include "DefaultSetting.h"

void CEngineShareSetting::Setting()
{
	CEngine::GetInst()->SetEngineSetting<CDefaultSetting>();
}

