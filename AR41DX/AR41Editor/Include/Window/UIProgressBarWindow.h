#pragma once
#include "Editor\EditorWindow.h"

class CUIProgressBarWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_BarTypeCB;
	class CEditorComboBox* m_BarTextureTypeCB;
	//애니메이션 옵션
	class CEditorInput* m_PlayTime;
	class CEditorInput* m_PlayScale;
	//만들거면 addframeinfo도 만들어줘야한다
	class CEditorInput* m_PosX;
	class CEditorInput* m_PosY;
	class CEditorInput* m_SizeX;
	class CEditorInput* m_SizeY;
	class CEditorInput* m_MinInput;
	class CEditorInput* m_MaxInput;
	class CEditorInput* m_ValueInput;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CUIProgressBar>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	int m_BarTypeIndex;
	int m_BarTextureTypeIndex;
	int m_Count;
	void BarTypeCBCallback(int index, const std::string& item);
	void BarTextureTypeCBCallback(int index, const std::string& item);
	void BarCreateCallback();
	void BarChangeCallback();
	void TextureChangeCallback();
protected:
	CUIProgressBarWindow();
	virtual ~CUIProgressBarWindow();
public:
	void SetSelectWidget(class CUIProgressBar* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

