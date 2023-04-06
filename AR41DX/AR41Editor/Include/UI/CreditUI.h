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
	void CreditPlay();
	void CreditStop();

private:
	void SpacePush();
	void SpaceUp();

public :
	void ActiveAllUI();
	void InActiveAllUI();

private:
	void CreateAllUI();
};

