#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CAnimation3DWindow : public CEditorWindow
{
	friend class CEditorGUIManager;
	//�ִϸ��̼� 
	class CEditorInput* m_AnimationName;
	class CEditorInput* m_BlendScale[3];
	class CEditorInput* m_BlendPos[3];
	class CEditorInput* m_BlendRot[4];
	class CEditorInput* m_GlobalTime;
	class CEditorInput* m_SequenceProgress;
	class CEditorInput* m_ChangeTimeAcc;
	class CEditorInput* m_ChangeTime;
	class CEditorCheckBox* m_Play;
	CEditorTree<class CAnimationData*>* m_AnimTree;
	class CEditorInput* m_AnimName;
	class CEditorInput* m_SequenceAnimName;
	class CEditorInput* m_AnimTime;
	class CEditorInput* m_AnimFrameTime;
	class CEditorInput* m_AnimPlayTime;
	class CEditorInput* m_AnimPlayScale;
	class CEditorCheckBox*	m_AnimLoop;
	class CEditorCheckBox*	m_AnimReverse;
	//���� �ִϸ��̼� ����
	class CEditorInput* m_CurAnimName;
	class CEditorInput* m_CurSequenceAnimName;
	class CEditorInput* m_CurAnimTime;
	class CEditorInput* m_CurAnimFrameTime;
	class CEditorInput* m_CurAnimPlayTime;
	class CEditorInput* m_CurAnimPlayScale;
	class CEditorCheckBox*	m_CurAnimLoop;
	class CEditorCheckBox*	m_CurAnimReverse;
	//���� �ִϸ��̼� ����
	class CEditorInput* m_ChangeAnimName;
	class CEditorInput* m_ChangeSequenceAnimName;
	class CEditorInput* m_ChangeAnimTime;
	class CEditorInput* m_ChangeAnimFrameTime;
	class CEditorInput* m_ChangeAnimPlayTime;
	class CEditorInput* m_ChangeAnimPlayScale;
	class CEditorCheckBox*	m_ChangeAnimLoop;
	class CEditorCheckBox*	m_ChangeAnimReverse;
	//���̷���
	class CEditorInput* m_SkeletoneName;
	class CEditorInput* m_BoneCount;
//��� �ҰŸ� Ʈ����
	//class CEditorInputText* m_BoneName;
	//class CEditorInputText* m_SocketName;
		//std::vector<BoneKeyFrame*>	m_vecFrame;
	class CAnimationData*	m_SelectAnimationData;
	CSharedPtr<class CAnimation>	m_SelectAnimation;
	CSharedPtr<class CSkeleton>		m_SelectSkeleton;
	void UpdateInfo();
	void ChangeInfo();
	void AnimTreeCallback(CEditorTreeItem<class CAnimationData*>* node, const std::string& item);
protected:
	CAnimation3DWindow();
	virtual ~CAnimation3DWindow();
public:
	void SetSelectComponent(class CAnimationMeshComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
	bool AddItem(class CAnimationData* data, const std::string& name, const std::string& parentName = "AnimationData");
	void Clear();
	void ClearSelect();
	void ChangeAnimation3D();
};

