#pragma once

#ifdef __has_include
#	if __has_include( "../../Client3DManager.h")
#		include "../../Client3DManager.h"
#   else
#		include "../../EditorManager.h"
#	endif
#endif

#include "Ref.h"

class CNode :
	public CRef
{
public:
	virtual bool Run() = 0;
	virtual bool Run(class CGameObject* Object) = 0;
};

