#pragma once

#include "Editor\EditorGroup.h"

class CComponentWidgetList :
    public CEditorGroup
{
	friend class CEditorWindow;
	friend class CDetailWindow;

protected:
	CComponentWidgetList();
	virtual ~CComponentWidgetList();

protected:
	class CDetailWindow* m_DetailWindow;

public:
	virtual bool Init();
	virtual void Render();
};

