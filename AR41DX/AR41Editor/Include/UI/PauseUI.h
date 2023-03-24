#pragma once

#include "UI/UIWindow.h"
#include "TitleSceneUI.h"

enum class EUIPauseMode // 타이틀 메뉴에서 현재 메뉴 모드를 체크하기 위한 플래그
{
	PauseMap,
	PauseMain,
	OptionSound,
	OptionCamera,
	Control,
	SaveGame,
	LoadGame,
	Close,
	End
};

enum class EUIPauseList // 일시중지 옵션 선택 메뉴에서 선택된 메뉴를 체크하기 위한 플래그
{
	OptionSound,
	OptionCamera,
	Control,
	SaveGame,
	LoadGame,
	BackToTitle,
	Quit,
	End
};

class CPauseUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;


protected:
	CPauseUI();
	CPauseUI(const CPauseUI& Window);
	virtual ~CPauseUI();


protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapBackUI;				// 백그라운드 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapMapUI;				// 백그라운드 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapPauseUI;			// 일시중지 메뉴 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSoundOptionUI;		// 사운드 옵션 메뉴 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapCameraOptionUI;		// 카메라 옵션 메뉴 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapControlUI;			// 조작법 UI 집합 맵
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSaveSelectUI;		// 세이브 선택 UI 집합 맵
	
private :
	EUIPauseMode	m_NowUIMode;		// 현재 UI
	EMapList		m_MapSelected;		// 맵에서 현재 선택된 UI
	EUIPauseList	m_PauseSelected;	// 일시중지 메뉴에서 현재 선택된 UI
	EUISoundList	m_SoundSelected;	// 사운드 옵션 메뉴에서 현재 선택된 UI
	EUICameraList	m_CameraSelected;	// 카메라 옵션 메뉴에서 현재 선택된 UI
	EUISaveList		m_SaveSelected;		// 저장/불러오기 메뉴에서 현재 선택된 UI

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CPauseUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	void OpenUI();
	void CloseUI();
	bool IsOpen() { return m_NowUIMode == EUIPauseMode::Close ? false : true; }

private:
	void CreaeteAllUI();
	void CreateBackgroundUI();
	void CreateMapUI();
	void CreatePauseUI();
	void CreateOptionSoundUI();
	void CreateOptionCameraUI();
	void CreateControlUI();
	void CreateSaveSelectUI();

	void ActiveBackUI();
	void ActiveMapUI();
	void ActivePauseUI();
	void ActiveOptionSoundUI();
	void ActiveOptionCameraUI();
	void ActiveSaveSelectUI();
	void ActiveControlUI();

	void InActiveBackUI();
	void InActiveMapUI();
	void InActivePauseUI();
	void InActiveOptionSoundUI();
	void InActiveOptionCameraUI();
	void InActiveSaveSelectUI();
	void InActiveControlUI();

private: // Mouse/KeyBoard Control
	void KeyLeftButton();	// 마우스 왼버튼 클릭
	void KeyRightButton();	// 마우스 우버튼 클릭

	void KeyUp();			// 위 화살표/w 입력
	void KeyDown();			// 아래 화살표/s 입력
	void KeyLeft();			// 좌 화살표/a 입력
	void KeyRight();		// 우 화살표/d 입력
	void KeyE();			// E 입력
	void KeySpace();		// 스페이스바 입력
	void KeyEsc();			// Esc 입력

private: // Main Menu UI Func
	void PauseUISound();
	void PauseUICamera();
	void PauseUIControl();
	void PauseUISave();
	void PauseUILoad();
	void PauseUIBackToTitle();
	void PauseUIQuit();

	void PauseUISoundHovered();
	void PauseUICameraHovered();
	void PauseUIControlHovered();
	void PauseUISaveHovered();
	void PauseUILoadHovered();
	void PauseUIBackToTitleHovered();
	void PauseUIQuitHovered();

private: // SaveSelect Menu UI Func
	void SaveSelectUISaveSlot1Hoverd();
	void SaveSelectUISaveSlot2Hoverd();
	void SaveSelectUISaveSlot3Hoverd();
	void SaveSelectClick();
	void SaveSelectSaveGame();		// 현재 게임 저장
	void SaveSelectLoadGame();		// 게임 불러와 시작


private: // Sound Option Menu UI Func
	void SoundOptionUISFX();
	void SoundOptionUISFXMinus();
	void SoundOptionUISFXPlus();
	void SoundOptionUIMusic();
	void SoundOptionUIMusicMinus();
	void SoundOptionUIMusicPlus();
	void SoundOptionUITalk();
	void SoundOptionUITalkMinus();
	void SoundOptionUITalkPlus();
	void SoundOptionUIReset();

private: // Camera Option Menu UI Func
	void CameraOptionUISensitive();
	void CameraOptionUISensitiveMinus();
	void CameraOptionUISensitivePlus();
	void CameraOptionUIXReverse();
	void CameraOptionUIXReverse_Normal();
	void CameraOptionUIXReverse_Reverse();
	void CameraOptionUIYReverse();
	void CameraOptionUIYReverse_Normal();
	void CameraOptionUIYReverse_Reverse();

};

