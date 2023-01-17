#pragma once

#include "Editor\EditorWindow.h"

class CDetailWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CDetailWindow();
	virtual ~CDetailWindow();

protected:
	std::vector<class CComponentWidgetList*>	m_vecComponentWidgetList;
	CSharedPtr<class CSceneComponent> m_SelectComponent;

public:
	class CSceneComponent* GetSelectComponent()	const
	{
		return m_SelectComponent;
	}

	class CComponentWidgetList* GetComponentWidgetList(const std::string& Name);

public:
	void SetSelectComponent(class CSceneComponent* Component);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void ChangeWidget(class CSceneComponent* Component);

private:
	void LoadButtonClick();
	void CreateEditorWidgetList(ESceneComponentType Type);
};

