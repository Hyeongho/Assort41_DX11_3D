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
	//Ʈ�� �־�� ��� ����
	//�ִԿ��Ӽ� �޽��� ����ƽ�Ž��� �����ؼ� �߰������ �ҵ�
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

