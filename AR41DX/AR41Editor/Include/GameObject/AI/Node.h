#pragma once

#include "Ref.h"

class CNode :
	public CRef
{
public:
	virtual bool Run() = 0;
	virtual bool Run(class CGameObject* Object) = 0;
};

