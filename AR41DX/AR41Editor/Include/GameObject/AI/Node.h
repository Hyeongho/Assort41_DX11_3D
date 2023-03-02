#pragma once

#include "../../Client3DManager.h"
#include "Ref.h"

class CNode :
	public CRef
{
public:
	virtual bool Run() = 0;
};

