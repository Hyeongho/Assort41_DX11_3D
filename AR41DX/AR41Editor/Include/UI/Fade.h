#pragma once

#include "UI/UIWindow.h"

enum class ENext_Scene
{
	None,
	TitleScene,
	BikiniCity,
	JellyfishField,
	JellyfishField1,
	JellyfishField2,
	KingJellyfish,
	ChumBucketLab
};

class CFade :
    public CUIWindow
{
    friend class CSceneViewport;
    friend class CScene;

protected:
    CFade();
    CFade(const CFade& Window);
    virtual ~CFade();

protected:
	CSharedPtr<class CUIImage>	m_Fade;
	float m_Opacity;

private:
	EFade_State m_State;
	ENext_Scene m_NextScene;

public:
	void SetState(EFade_State State)
	{
		m_State = State;
	}

	EFade_State GetState() const
	{
		return m_State;
	}

	void SetNextScene(ENext_Scene NextScene)
	{
		m_NextScene = NextScene;
	}

	ENext_Scene GetNextScene() const
	{
		return m_NextScene;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CFade* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

