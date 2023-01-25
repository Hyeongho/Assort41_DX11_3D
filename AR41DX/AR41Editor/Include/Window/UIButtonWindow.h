#pragma once
#include "Editor\EditorWindow.h"

class CUIButtonWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_ButtonStateCB;
	class CEditorComboBox* m_ButtonEventStateCB;
	class CEditorCheckBox* m_ButtonEnableCheck;
	//애니메이션 옵션
	class CEditorInput* m_PlayTime;
	class CEditorInput* m_PlayScale;
	//만들거면 addframeinfo도 만들어줘야한다
	class CEditorInput* m_PosX;
	class CEditorInput* m_PosY;
	class CEditorInput* m_SizeX;
	class CEditorInput* m_SizeY;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CUIButton>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	int m_ButtonStateIndex;
	int m_ButtonEventStateIndex;
	int m_Count;
	void StateCBCallback(int index, const std::string& item);
	void EventStateCBCallback(int index, const std::string& item);
	void ButtonCreateCallback();
	void ButtonChangeCallback();
	void TextureChangeCallback();
	void SoundChangeCallback();
protected:
	CUIButtonWindow();
	virtual ~CUIButtonWindow();
public:
	void SetSelectWidget(class CUIButton* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

