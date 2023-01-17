#pragma once
#include "ComponentWidgetList.h"
class CSceneComponentWidgetList :
    public CComponentWidgetList
{
	friend class CEditorWindow;

protected:
	CSceneComponentWidgetList();
	virtual ~CSceneComponentWidgetList();

public:
	virtual bool Init();

};

