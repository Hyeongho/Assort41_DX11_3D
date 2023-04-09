#pragma once

#include "UI/UIWindow.h"
#include "UI/UIImage.h"
#include "UI/UIText.h"

class CCreditUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CCreditUI();
	CCreditUI(const CCreditUI& Window);
	virtual ~CCreditUI();

protected:
	CSharedPtr<CUIImage>	m_BackUI;
	CSharedPtr<CUIText>		m_TextTitleUI;
	CSharedPtr<CUIText>		m_TextUI;

private:
	bool	m_Start;
	float	m_ScrollSpeed;
	bool	m_IsMenu;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CCreditUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	void SetIsEnd(bool isEnd = true) { m_IsMenu = !isEnd; }

public :
	void CreditPlay();	// 크레딧 UI 실행
	void CreditStop();	// 크레딧 UI 종료

private:
	void SpacePush();	// Space가 눌렸을 때의 작동
	void SpaceUp();		// Space키를 뗏을 때의 작동
	void EscPush();		// Esc가 눌렸을 때의 작동

public :
	void ActiveAllUI();
	void InActiveAllUI();

private:
	void CreateAllUI();
};

