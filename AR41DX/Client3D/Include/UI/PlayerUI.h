#pragma once
#include "UI\UIWindow.h"

class CPlayerUI :
    public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CPlayerUI();
	CPlayerUI(const CPlayerUI& Window);
	virtual ~CPlayerUI();

protected:
	CSharedPtr<class CUIImage>	m_MaxHP;
	CSharedPtr<class CUIImage>	m_CurHP;
	CSharedPtr<class CUIImage>	m_Socks;
	CSharedPtr<class CUIImage>	m_Fritter;
	CSharedPtr<class CUIImage>	m_Glittering;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CPlayerUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

