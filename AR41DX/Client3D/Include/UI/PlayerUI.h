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
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapHP;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapHud;

	//CSharedPtr<class CUIImage>	m_MaxHP;
	//CSharedPtr<class CUIImage>	m_CurHP;
	//CSharedPtr<class CUIImage>	m_Socks;
	//CSharedPtr<class CUIImage>	m_Fritter;
	//CSharedPtr<class CUIImage>	m_Glittering;

private :
	float	m_Timer;  // UI�� ��Ű�� ���� Ȱ��ȭ�� ��, �󸶳� �Ǿ����� üũ�ϱ� ���� ����
	bool	m_Tabbed; // UI�� ��Ű�� ���� Ȱ��ȭ�Ǿ����� üũ�ϱ� ���� ����
	bool	m_Boss; // ���� ������ ������ üũ�ϱ� ���� ����


public:
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

};

