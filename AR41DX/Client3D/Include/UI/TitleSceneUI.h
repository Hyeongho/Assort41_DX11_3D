#pragma once

#include "UI\UIWindow.h"

enum class EUIMode // 타이틀 메뉴에서 현재 메뉴 모드를 체크하기 위한 플래그
{
	Main,
	Continue,
	NewGame,
	Control,
	Credit,
	Option,
	OptionSound,
	OptionCamera,
	End
};

enum class EMainList // 타이틀 메인 메뉴에서 현재 커서가 가 있는(선택된) 메뉴를 체크하기 위한 플래그
{
	Continue,
	NewGame,
	Option,
	Control,
	Credits,
	Quit,
	End
};

enum class EUIOptionList // 타이틀 옵션 메뉴에서 현재 커서가 가 있는(선택된) 메뉴를 체크하기 위한 플래그
{
	Sound,
	Camera,
	End
};

enum class EUISaveList // 타이틀 세이브 선택 메뉴에서 현재 커서가 가 있는(선택된) 메뉴를 체크하기 위한 플래그
{
	First,
	Second,
	Third,
	End
};

enum class EUISoundList // 타이틀 사운드 옵션 메뉴에서 현재 커서가 가 있는(선택된) 메뉴를 체크하기 위한 플래그
{
	SFX,
	Music,
	Talk,
	Reset,
	End
};

enum class EUICameraList // 타이틀 카메라 옵션 메뉴에서 현재 커서가 가 있는(선택된) 메뉴를 체크하기 위한 플래그
{
	End
};


class CTitleSceneUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CTitleSceneUI();
	CTitleSceneUI(const CTitleSceneUI& Window);
	virtual ~CTitleSceneUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapBackUI;				// 백그라운드 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapMainUI;				// 메인 메뉴 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSaveSelectUI;		// 세이브 선택 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapOptionUI;			// 옵션 메뉴 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSoundOptionUI;		// 사운드 옵션 메뉴 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapCameraOptionUI;		// 카메라 옵션 메뉴 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapControlUI;			// 조작법 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapCreditsUI;			// 크레딧 UI 집합 맵

private :
	EUIMode			m_NowUIMode;		// 현재 UI
	EMainList		m_MainSelected;		// 메인 메뉴에서 현재 선택된 UI
	EUIOptionList	m_OptionSelected;	// 옵션 메뉴에서 현재 선택된 UI
	EUISaveList		m_SaveSelected;		// 계속/새로하기 메뉴에서 현재 선택된 UI
	EUISoundList	m_SoundSelected;	// 사운드 옵션 메뉴에서 현재 선택된 UI
	EUICameraList	m_CameraSelected;	// 카메라 옵션 메뉴에서 현재 선택된 UI

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CTitleSceneUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void CreateBackgroundUI();
	void CreateMainUI();
	void CreateOptionUI();
	void CreateOptionSoundUI();
	void CreateOptionCameraUI();
	void CreateSaveSelectUI();
	void CreateControlUI();
	void CreateCreditsUI();

private :
	void ActiveMainUI();
	void ActiveOptionUI();
	void ActiveOptionSoundUI();
	void ActiveOptionCameraUI();
	void ActiveSaveSelectUI();
	void ActiveControlUI();
	void ActiveCreditsUI();

	void InActiveMainUI();
	void InActiveOptionUI();
	void InActiveOptionSoundUI();
	void InActiveOptionCameraUI();
	void InActiveSaveSelectUI();
	void InActiveControlUI();
	void InActiveCreditsUI();

private: // Mouse/KeyBoard Control
	void KeyLeftButton();	// 마우스 왼버튼 클릭
	void KeyRightButton();	// 마우스 우버튼 클릭
	
	void KeyUp();			// 위 화살표/w 입력
	void KeyDown();			// 아래 화살표/s 입력
	void KeyLeft();			// 좌 화살표/a 입력
	void KeyRight();		// 우 화살표/d 입력
	void KeySpace();		// 스페이스바 입력
	void KeyBack();			// Esc 입력

private : // Main Menu UI Func
	void MainUIContinue();
	void MainUINewGame();
	void MainUIOption();
	void MainUIControl();
	void MainUICredits();
	void MainUIQuit();

	void MainUIContinueHovered();
	void MainUINewGameHovered();
	void MainUIOptionHovered();
	void MainUIControlHovered();
	void MainUICreditsHovered();
	void MainUIQuitHovered();

public :
	void SaveSelectUISaveSlot1Hoverd();
	void SaveSelectUISaveSlot2Hoverd();
	void SaveSelectUISaveSlot3Hoverd();

private : // Option Menu UI Func
	void OptionUISound();
	void OptionUICamera();

	void OptionUISoundHovered();
	void OptionUICameraHovered();

	void SoundOptionUISFX();
	void SoundOptionUIMusic();
	void SoundOptionUITalk();
	void SoundOptionUIReset();

private : // Etc Menu Func
	void NewGameStart();		// 새 게임 시작
	void ContinueStart();		// 기존 게임 이어서 하기

};

