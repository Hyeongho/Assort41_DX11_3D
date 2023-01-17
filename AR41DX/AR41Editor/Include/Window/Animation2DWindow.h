#pragma once

#include "Editor\EditorWindow.h"

class CAnimation2DWindow :
    public CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CAnimation2DWindow();
	virtual ~CAnimation2DWindow();

private:
	class CEditorListBox* m_Animation2DClassList;
	class CEditorListBox* m_Animation2DSequenceList;
	class CEditorListBox* m_SequenceFrameList;
	class CEditorImage* m_Sequence2DImage;
	CSharedPtr<class CTexture>  m_SelectTexture;
	class CEditorInput* m_StartX;
	class CEditorInput* m_StartY;
	class CEditorInput* m_EndX;
	class CEditorInput* m_EndY;
	class CEditorInput* m_Count;
	std::vector<Animation2DFrameData>	m_vecFrame;

	std::string		m_Animation2DSelectName;
	std::string		m_Animation2DSequenceSelectName;

public:
	const std::string& GetAnimation2DSelectName()	const
	{
		return m_Animation2DSelectName;
	}

	const std::string& GetAnimation2DSequenceSelectName()	const
	{
		return m_Animation2DSequenceSelectName;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void LoadAnimation2DClassList();
	void LoadAnimation2DSequenceList();
	void CreateAnimation2D();
	void SelectTextureButton();
	void CreateAnimationSequence2D();
	void AddFrameClick();
	void AddAllFrameClick();
	void Animation2DSelectCallback(int Index, const std::string& Item);
	void SequenceSelectCallback(int Index, const std::string& Item);
	void FrameSelectCallback(int Index, const std::string& Item);
};

