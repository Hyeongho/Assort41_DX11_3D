#pragma once

#include "UI/UIWindow.h"
#include "../GameObject/BossMonster.h"

class CBossUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CBossUI();
	CBossUI(const CBossUI& Window);
	virtual ~CBossUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapBackUI;
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapHpUI;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CBossUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void CreateBossUI();

public :
	void CreateBossUI(BossData BossData);
};

