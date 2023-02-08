#pragma once
#include "Editor\EditorWindow.h"

class CRigidBodyWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorCheckBox* m_Gravity;
	class CEditorCheckBox* m_Ground;
	class CEditorInput* m_Force[3];
	class CEditorInput* m_Accel[3];
	class CEditorInput* m_AccelA[3];
	class CEditorInput* m_Velocity[3];
	class CEditorInput* m_MaxVelocity[3];
	class CEditorInput* m_MinVelocity[3];
	class CEditorInput* m_Mass;
	class CEditorInput* m_FricCoeff;
	CSharedPtr<class CRigidBody>	m_SelectRigidBody;
	void RigidBodyChangeCallback();
protected:
	CRigidBodyWindow();
	virtual ~CRigidBodyWindow();
public:
	void SetSelectComponent(class CRigidBody* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

