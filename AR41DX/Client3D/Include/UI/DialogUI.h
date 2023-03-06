#pragma once

#include "UI/UIWindow.h"

struct DialogInfo {
	std::vector<std::wstring> vecTalker;
	std::vector<std::wstring> vecText;
	int TextIdx;
	int TextMaxIdx;

	DialogInfo() :
		TextIdx(0),
		TextMaxIdx(0)
	{}
};

class CDialogUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CDialogUI();
	CDialogUI(const CDialogUI& Window);
	virtual ~CDialogUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapDialogUI;

private :
	std::unordered_map<std::string, DialogInfo> m_mapDialogInfo;
	DialogInfo m_curDialog;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CDialogUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void CreaeteAllUI();
	void CreateDialogUI();

	void ActiveDialogUI();
	void InActiveDialogUI();

public :
	void OpenDialog() {
		KeyLeftButton();
		ActiveDialogUI();
	}

public :
	void SetDialogInfo(EMapList Map, ENpcList Npc);
	void SetCurDialog(std::string DialName);

private:
	void KeyLeftButton();	// 마우스 왼버튼 클릭
	void KeyRightButton();	// 마우스 우버튼 클릭

	
};

