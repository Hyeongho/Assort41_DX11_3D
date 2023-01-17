#pragma once

#include "EngineInfo.h"

class CEngineSetting
{
	friend class CEngine;

protected:
	CEngineSetting();
	virtual ~CEngineSetting();

public:
	virtual void Init();
};

