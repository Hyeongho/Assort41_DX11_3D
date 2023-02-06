#pragma once
#include "Editor\EditorWindow.h"

class CMeshWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorInput* m_Name;
	class CEditorInput* m_Min[3];
	class CEditorInput* m_Max[3];
	class CEditorComboBox* m_MeshType;
	class CEditorInput* m_RenderCount;
	class CEditorInput* m_SkeletonName;
	CSharedPtr<class CPrimitiveComponent>	m_SelectComponent;
	CSharedPtr<class CAnimationMesh>	m_SelectAnimMesh;
protected:
	CMeshWindow();
	virtual ~CMeshWindow();
public:
	void SetSelectComponent(class CPrimitiveComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
	void MeshChangeCallback(const TCHAR* name);
	void SkeletonChangeCallback(const TCHAR* name);
};

