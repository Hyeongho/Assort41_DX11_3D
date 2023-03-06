#pragma once
#include "Editor\EditorWindow.h"

class CColliderObb3dWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
	class CEditorListBox* m_ChannelList;
	class CEditorCheckBox* m_ProfileRelationCheck;
	class CEditorInput* m_ProfileName;
	class CEditorInput* m_ColiderSizeText[3];
	CSharedPtr<class CColliderOBB3D>	m_SelectObb;
	std::string			m_SelectRelationItem;
	void ChannelClickCallback(int index, const std::string& item);
	void ProfileCreateCallback();
	void ColiderChangeCallback();
	void ChannelChangeCallback();
	void RelationChangeCallback();
	void LoadChannelName();
protected:
	CColliderObb3dWindow();
	virtual ~CColliderObb3dWindow();
public:
	void SetSelectComponent(class CCollider* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

