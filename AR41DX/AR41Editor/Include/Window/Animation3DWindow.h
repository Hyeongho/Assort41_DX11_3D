#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CAnimation3DWindow : public CEditorWindow
{
	friend class CEditorGUIManager;
	//애니메이션 
	class CEditorInput* m_AnimationName;
	class CEditorInput* m_CurAnimName;	//현재 애니메이션 정보
	class CEditorInput* m_ChangeAnimName;	//다음 애니메이션 정보
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
	class CEditorInput* m_AnimTime;
	class CEditorInput* m_AnimFrameTime;
	class CEditorInput* m_AnimPlayTime;
	class CEditorInput* m_AnimPlayScale;
	class CEditorCheckBox*	m_AnimLoop;
	class CEditorCheckBox*	m_AnimReverse;
	//시퀸스 정보
	class CEditorInput* m_SequenceAnimName;
	class CEditorCheckBox* m_Loop;
	class CEditorCheckBox* m_End;
	class CEditorInput* m_StartTime;
	class CEditorInput* m_EndTime;
	class CEditorInput* m_TimeLength;
	class CEditorInput* m_FrameTime;
	class CEditorInput* m_PlayTime;
	class CEditorInput* m_PlayScale;
	class CEditorInput* m_StartFrame;
	class CEditorInput* m_EndFrame;
	class CEditorInput* m_FrameLength;
	class CEditorInput* m_FrameMode;
	class CEditorInput* m_ChangeFrame;
   // std::vector<BoneKeyFrame*>	m_vecKeyFrame;
	//스켈레톤
	class CEditorInput* m_SkeletoneName;
	class CEditorInput* m_BoneCount;
//얘네 할거면 트리로
	//class CEditorInput* m_BoneName;
	//class CEditorInput* m_SocketName;
	CSharedPtr<class CAnimationSequence>	m_SelectAnimationSequence;
	CSharedPtr<class CAnimation>	m_SelectAnimation;
	CSharedPtr<class CSkeleton>		m_SelectSkeleton;
	void UpdateInfo();
	void SequenceChangeInfo();
	void SequenceSaveInfo();
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
	void ChangeAnimation3D(const TCHAR* path);
};

