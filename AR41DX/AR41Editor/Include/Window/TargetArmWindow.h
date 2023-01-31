#pragma once
#include "Editor\EditorWindow.h"

class CTargetArmWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorComboBox* m_TargetArmAxisCB;
	class CEditorInput* m_TargetArmOffset[3];
	class CEditorInput* m_TargetDistance;
	class CEditorInput* m_WheelSensitivity;
	class CEditorInput* m_WheelTickMove;
	class CEditorInput* m_WheelZoomMin;
	class CEditorInput* m_WheelZoomMax;
	class CEditorCheckBox* m_WheelZoomInOutEnable;
	CSharedPtr<class CTargetArm>	m_SelectArm;
	int m_TargetArmCBIndex;
	void TargetArmChangeCallback();
	void TargetArmCBClickCallback(int index, const std::string& item);
protected:
	CTargetArmWindow();
	virtual ~CTargetArmWindow();
public:
	void SetSelectComponent(class CTargetArm* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

