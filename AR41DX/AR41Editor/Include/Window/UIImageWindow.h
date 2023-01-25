#pragma once
#include "Editor\EditorWindow.h"

class CUIImageWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	//�ִϸ��̼� �ɼ�
	class CEditorInput* m_PlayTime;
	class CEditorInput* m_PlayScale;
	//����Ÿ� addframeinfo�� ���������Ѵ�
	class CEditorInput* m_PosX;
	class CEditorInput* m_PosY;
	class CEditorInput* m_SizeX;
	class CEditorInput* m_SizeY;
	class CEditorSlider* m_Slider;
	CSharedPtr<class CUIImage>	m_SelectWidget;
	CSharedPtr<class CUIWindow>	m_SelectWindow;
	int m_Count;
	void ImgCreateCallback();
	void ImgChangeCallback();
	void TextureChangeCallback();
protected:
	CUIImageWindow();
	virtual ~CUIImageWindow();
public:
	void SetSelectWidget(class CUIImage* widget);
	void SetSelectWindow(class CUIWindow* window);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

