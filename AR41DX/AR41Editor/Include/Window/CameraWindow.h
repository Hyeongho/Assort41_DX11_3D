#pragma once
#include "Editor\EditorWindow.h"

class CCameraWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_CameraCB;
	class CEditorInput* m_CameraViewDistance;
	CSharedPtr<class CCameraComponent>	m_SelectCamera;
	int m_CameraCBIndex;
	void CameraChangeCallback();
	void CameraCBClickCallback(int index, const std::string& item);
protected:
	CCameraWindow();
	virtual ~CCameraWindow();
public:
	void SetSelectComponent(class CSceneComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

