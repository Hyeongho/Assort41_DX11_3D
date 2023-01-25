#pragma once
#include "Editor\EditorWindow.h"
class CUITextWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_AlignHCB;
	class CEditorComboBox* m_AlignVCB;
	class CEditorListBox* m_FontList;
	class CEditorCheckBox* m_TransparencyCheck;
	class CEditorInput* m_PosX;
	class CEditorInput* m_PosY;
	class CEditorInput* m_SizeX;
	class CEditorInput* m_SizeY;
	class CEditorInput* m_TextInput;
	class CEditorInput* m_FontSizeInput;
	class CEditorSlider* m_Slider;
	//shadow option
	class CEditorSlider* m_ShadowSlider;
	class CEditorInput* m_ShadowOffsetX;
	class CEditorInput* m_ShadowOffsetY;
	class CEditorCheckBox* m_ShadowEnableCheck;
	class CEditorCheckBox* m_ShadowTransparencyCheck;
	CSharedPtr<class CUIText>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	std::string			m_FontItem;
	int m_AlignHIndex;
	int m_AlignVIndex;
	int m_Count;
	void AlignHCBCallback(int index, const std::string& item);
	void AlignVCBCallback(int index, const std::string& item);
	void FontListClickCallback(int index, const std::string& item);
	void TextCreateCallback();
	void TextChangeCallback();
	void LoadFontName();
protected:
	CUITextWindow();
	virtual ~CUITextWindow();
public:
	void SetSelectWidget(class CUIText* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

