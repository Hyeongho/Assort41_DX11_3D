#pragma once

#include "Editor\EditorWindow.h"

class CTransformWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CTransformWindow();
	virtual ~CTransformWindow();

private:
	class CEditorInput* m_Pos[3];
	class CEditorInput* m_Rot[3];
	class CEditorInput* m_Scale[3];
	CSharedPtr<class CSceneComponent>	m_SelectComponent;

public:
	void SetSelectComponent(class CSceneComponent* Component);
	void SetPos(const Vector3& Pos);
	void SetRotation(const Vector3& Rot);
	void SetScale(const Vector3& Scale);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void PosXCallback();
	void PosYCallback();
	void PosZCallback();
	void RotXCallback();
	void RotYCallback();
	void RotZCallback();
	void ScaleXCallback();
	void ScaleYCallback();
	void ScaleZCallback();
};

