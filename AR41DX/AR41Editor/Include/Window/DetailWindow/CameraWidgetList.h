#pragma once
#include "ComponentWidgetList.h"
class CCameraWidgetList :
    public CComponentWidgetList
{
	friend class CEditorWindow;

protected:
	CCameraWidgetList();
	virtual ~CCameraWidgetList();

public:
	virtual bool Init();
};

