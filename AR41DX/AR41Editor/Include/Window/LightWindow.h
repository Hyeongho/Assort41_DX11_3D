#pragma once
#include "Editor\EditorWindow.h"

class CLightWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox*	m_LightType;
	class CEditorInput* m_Color[4];
	class CEditorInput* m_Pos[3];
	class CEditorInput* m_Dir[3];
	class CEditorInput* m_Distance;
	class CEditorInput* m_AngleIn;
	class CEditorInput* m_AngleOut;
	class CEditorInput* m_Att1;
	class CEditorInput* m_Att2;
	class CEditorInput* m_Att3;
	class CEditorInput* m_Intensity;
	CSharedPtr<class CLightComponent>	m_SelectComponent;
	void LightChangeCallback();
protected:
	CLightWindow();
	virtual ~CLightWindow();
public:
	void SetSelectComponent(class CLightComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

