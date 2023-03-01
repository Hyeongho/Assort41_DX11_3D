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
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapHP;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapHud;

	//CSharedPtr<class CUIImage>	m_MaxHP;
	//CSharedPtr<class CUIImage>	m_CurHP;
	//CSharedPtr<class CUIImage>	m_Socks;
	//CSharedPtr<class CUIImage>	m_Fritter;
	//CSharedPtr<class CUIImage>	m_Glittering;

private :
	float	m_Timer;  // UI°Ą ĹÇĹ°żĄ ŔÇÇŘ Č°źşČ­ľČ ČÄ, žó¸śłŞ ľÇžú´ÂÁö ĂźĹŠÇĎąâ Ŕ§ÇŃ şŻźö
	bool	m_Tabbed; // UI°Ą ĹÇĹ°żĄ ŔÇÇŘ Č°źşČ­ľÇžú´ÂÁö ĂźĹŠÇĎąâ Ŕ§ÇŃ şŻźö
	bool	m_Boss; // ÇöŔç ş¸˝şŔü ÁßŔÎÁö ĂźĹŠÇĎąâ Ŕ§ÇŃ şŻźö


public:
	CSharedPtr<class CUIImage>	m_MaxHP[5];
	CSharedPtr<class CUIImage>	m_CurHP[5];
	CSharedPtr<class CUIImage>	m_Socks;
	CSharedPtr<class CUIImage>	m_Fritter;
	CSharedPtr<class CUIImage>	m_Glittering;
	//text 3°ł ÇĘżäÇÔ

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

private :
	void CreateHpUI();
	void CreateHudUI();

	void ActiveHpUI();
	void ActiveHudUI();

	void InActiveHpUI();
	void InActiveHudUI();


private:
	void KeyTab();

	void SetPlayerUIOpacity(float opacity);
};

