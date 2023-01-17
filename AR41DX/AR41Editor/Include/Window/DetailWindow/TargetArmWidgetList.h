#pragma once
#include "ComponentWidgetList.h"
class CTargetArmWidgetList :
    public CComponentWidgetList
{
	friend class CEditorWindow;

protected:
	CTargetArmWidgetList();
	virtual ~CTargetArmWidgetList();

public:
	virtual bool Init();
};

