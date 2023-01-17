#pragma once
#include "ComponentWidgetList.h"
class CSpriteComponentWidgetList :
    public CComponentWidgetList
{
	friend class CEditorWindow;

protected:
	CSpriteComponentWidgetList();
	virtual ~CSpriteComponentWidgetList();

private:
	class CEditorInput* m_PlayTime;
	class CEditorInput* m_PlayScale;
	class CEditorInput* m_AnimationClass;
	class CEditorListBox* m_AnimationList;
	class CEditorCheckBox* m_LoopCheck;
	class CEditorCheckBox* m_ReverseCheck;
	class CEditorInput* m_SequenceName;
	class CEditorInput* m_Animation2DDataName;

	class CSpriteComponent* m_Sprite;

public:
	void SetSpriteContent(class CSpriteComponent* Sprite);
	void SetSelectAnimationSequence2DName(const std::string& Name);

public:
	virtual bool Init();

private:
	void SelectTextureButtonClick();
	void CreateAnimationButtonClick();
	void AnimationSelectCallback(int Index, const std::string& Item);
	void AddAnimation2DData();
	void DeleteAnimation2DData();
	void ModifyAnimation2DData();
};

