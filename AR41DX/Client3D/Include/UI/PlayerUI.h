#pragma once
#include "UI\UIWindow.h"

class CPlayerUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	float m_AllOpacity;
	CPlayerUI();
	CPlayerUI(const CPlayerUI& Window);
	virtual ~CPlayerUI();

protected:
	CSharedPtr<class CUIImage>	m_MaxHP[5];
	CSharedPtr<class CUIImage>	m_CurHP[5];
	CSharedPtr<class CUIImage>	m_Socks;
	CSharedPtr<class CUIImage>	m_Fritter;
	CSharedPtr<class CUIImage>	m_Glittering;
	//text 3개 필요함

	CSharedPtr<class CUIText>	m_GlitterText;
	CSharedPtr<class CUIText>	m_SocksText;
	CSharedPtr<class CUIText>	m_FritterText;

public:
	void SetAllOpacity(float opacity)
	{
		m_AllOpacity = opacity;
	}

	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CPlayerUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	void SetPlayerUIOpacity(float opacity);
	void SetHp(int hp);
	void SetMaxHp(int hp);
	void SetGlitter(int i);
	void SetSocks(int i);
	void SetFritter(int i);
};

