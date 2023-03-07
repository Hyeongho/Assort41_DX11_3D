#pragma once

#include "UI\UIWindow.h"

enum class EUIMode // Ÿ��Ʋ �޴����� ���� �޴� ��带 üũ�ϱ� ���� �÷���
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

enum class EMainList // Ÿ��Ʋ ���� �޴����� ���� Ŀ���� �� �ִ�(���õ�) �޴��� üũ�ϱ� ���� �÷���
{
	Continue,
	NewGame,
	Option,
	Control,
	Credits,
	Quit,
	End
};

enum class EUIOptionList // Ÿ��Ʋ �ɼ� �޴����� ���� Ŀ���� �� �ִ�(���õ�) �޴��� üũ�ϱ� ���� �÷���
{
	Sound,
	Camera,
	End
};

enum class EUISaveList // Ÿ��Ʋ ���̺� ���� �޴����� ���� Ŀ���� �� �ִ�(���õ�) �޴��� üũ�ϱ� ���� �÷���
{
	First,
	Second,
	Third,
	End
};

enum class EUISoundList // Ÿ��Ʋ ���� �ɼ� �޴����� ���� Ŀ���� �� �ִ�(���õ�) �޴��� üũ�ϱ� ���� �÷���
{
	SFX,
	Music,
	Talk,
	Reset,
	End
};

enum class EUICameraList // Ÿ��Ʋ ī�޶� �ɼ� �޴����� ���� Ŀ���� �� �ִ�(���õ�) �޴��� üũ�ϱ� ���� �÷���
{
	Sensitive,
	XReverse,
	YReverse,
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
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapBackUI;				// ��׶��� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapMainUI;				// ���� �޴� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSaveSelectUI;		// ���̺� ���� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapOptionUI;			// �ɼ� �޴� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSoundOptionUI;		// ���� �ɼ� �޴� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapCameraOptionUI;		// ī�޶� �ɼ� �޴� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapControlUI;			// ���۹� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapCreditsUI;			// ũ���� UI ���� ��

private :
	EUIMode			m_NowUIMode;		// ���� UI
	EMainList		m_MainSelected;		// ���� �޴����� ���� ���õ� UI
	EUIOptionList	m_OptionSelected;	// �ɼ� �޴����� ���� ���õ� UI
	EUISaveList		m_SaveSelected;		// ���/�����ϱ� �޴����� ���� ���õ� UI
	EUISoundList	m_SoundSelected;	// ���� �ɼ� �޴����� ���� ���õ� UI
	EUICameraList	m_CameraSelected;	// ī�޶� �ɼ� �޴����� ���� ���õ� UI

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
	void CreaeteAllUI();
	void CreateBackgroundUI();
	void CreateMainUI();
	void CreateOptionUI();
	void CreateOptionSoundUI();
	void CreateOptionCameraUI();
	void CreateSaveSelectUI();
	void CreateControlUI();
	void CreateCreditsUI();

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
	void KeyLeftButton();	// ���콺 �޹�ư Ŭ��
	void KeyRightButton();	// ���콺 ���ư Ŭ��
	
	void KeyUp();			// �� ȭ��ǥ/w �Է�
	void KeyDown();			// �Ʒ� ȭ��ǥ/s �Է�
	void KeyLeft();			// �� ȭ��ǥ/a �Է�
	void KeyRight();		// �� ȭ��ǥ/d �Է�
	void KeySpace();		// �����̽��� �Է�
	void KeyBack();			// Esc �Է�

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

private: // SaveSelect Menu UI Func
	void SaveSelectUISaveSlot1Hoverd();
	void SaveSelectUISaveSlot2Hoverd();
	void SaveSelectUISaveSlot3Hoverd();
	void GameStart();		// ���� ����

private : // Option Menu UI Func
	void OptionUISound();
	void OptionUICamera();

	void OptionUISoundHovered();
	void OptionUICameraHovered();

	void SoundOptionUISFX();
	void SoundOptionUIMusic();
	void SoundOptionUITalk();
	void SoundOptionUIReset();
};

