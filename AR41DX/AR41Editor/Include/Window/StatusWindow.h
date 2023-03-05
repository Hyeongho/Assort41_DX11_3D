#pragma once
#include "Editor\EditorWindow.h"

class CStatusWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Check;
	class CEditorInput* m_Frame;
	class CEditorInput* m_PosX;
	class CEditorInput* m_PosY;
	class CEditorInput* m_PosZ;
	class CEditorInput* m_SizeX;
	class CEditorInput* m_SizeY;
	class CEditorInput* m_SizeZ;
	//¸¶¿ì½º
	class CEditorInput* m_RayPos[3];
	class CEditorInput* m_RayDir[3];
	class CEditorInput* m_MousePos[2];
	class CEditorInput* m_MouseWorldPos[2];
	class CEditorInput* m_MouseUIPos[2];
	class CEditorInput* m_MouseMove[2];
	class CEditorInput* m_MouseMove2D[2];
	CSharedPtr<class CSceneComponent>	m_SelectComponent;
	CSharedPtr<class CUIWidget>	m_SelectWidget;
protected:
	CStatusWindow();
	virtual ~CStatusWindow();
public:
	void SetSelectComponent(class CSceneComponent* component);
	void SetSelectWidget(class CUIWidget* widget);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

