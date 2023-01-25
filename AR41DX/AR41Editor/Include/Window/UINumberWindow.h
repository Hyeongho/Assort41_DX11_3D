#pragma once
#include "Editor\EditorWindow.h"

class CUINumberWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorInput* m_NumberInput;
	class CEditorInput* m_SpaceInput;
	class CEditorInput* m_PlayTime;
	class CEditorInput* m_PlayScale;
	class CEditorInput* m_PosX;
	class CEditorInput* m_PosY;
	class CEditorInput* m_SizeX;
	class CEditorInput* m_SizeY;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CUINumber>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	int m_Count;
	void NumberCreateCallback();
	void NumberChangeCallback();
	void TextureChangeCallback();
protected:
	CUINumberWindow();
	virtual ~CUINumberWindow();
public:
	void SetSelectWidget(class CUINumber* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

