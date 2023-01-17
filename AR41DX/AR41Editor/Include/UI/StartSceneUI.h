#pragma once

#include "UI\UIWindow.h"

class CStartSceneUI :
    public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CStartSceneUI();
	CStartSceneUI(const CStartSceneUI& Window);
	virtual ~CStartSceneUI();

protected:
	CSharedPtr<class CUIButton>	m_Button;
	CSharedPtr<class CUIText>	m_Title;
	CSharedPtr<class CUIProgressBar>	m_ProgressBar;
	CSharedPtr<class CUINumber>	m_Number;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CStartSceneUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void StartButtonClick();
};

