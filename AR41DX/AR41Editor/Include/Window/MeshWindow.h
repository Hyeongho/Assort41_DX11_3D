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
	CSharedPtr<class CMesh>	m_SelectMesh;
	//트리 있어야 드랍 가능
	//애님에ㅣ션 메쉬는 스태틱매쉬랑 구별해서 추가해줘야 할듯
	void MeshChangeCallback();
	void MeshCreateCallback();
protected:
	CMeshWindow();
	virtual ~CMeshWindow();
public:
	void SetSelectComponent(class CPrimitiveComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

