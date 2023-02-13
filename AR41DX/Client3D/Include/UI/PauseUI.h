#pragma once

#include "UI\UIWindow.h"
#include "TitleSceneUI.h"

enum class EUIMapList // �Ͻ����� �� ���� �޴����� ���õ� �޴��� üũ�ϱ� ���� �÷���
{
	Patric,				// ����
	DutchMan_Grave,		// �ٴٵ������� ����
	Sand_Mountain,		// �𷡻�
	Goo_Lagoon,			// ������ ȣ��
	Dome,				// �����̵�
	KK,					// ���������� ��
	Chum_Bucketlab,		// �̳��Ĵ� �����
	Merma_Lair,			// �ξ� ����ó
	Bikini_Bottom,		// ��Ű�Ͻ�Ƽ
	Down_Town,			// ��Ű�Ͻ�Ƽ ����
	Krabs,				// ���Ի���
	Kelp_Forest,		// ���� ��
	Rock_Bottom,		// �޷ս�Ƽ
	Jelly_Fish_Field,	// ���ĸ� ����
	Industrial_Park,	// ��������
	End
};

enum class EUIPauseMode // Ÿ��Ʋ �޴����� ���� �޴� ��带 üũ�ϱ� ���� �÷���
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

enum class EUIPauseList // �Ͻ����� �ɼ� ���� �޴����� ���õ� �޴��� üũ�ϱ� ���� �÷���
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
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapBackUI;				// ��׶��� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapMapUI;				// ��׶��� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapPauseUI;			// �Ͻ����� �޴� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSoundOptionUI;		// ���� �ɼ� �޴� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapCameraOptionUI;		// ī�޶� �ɼ� �޴� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapControlUI;			// ���۹� UI ���� ��
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapSaveSelectUI;		// ���̺� ���� UI ���� ��
	
private :
	EUIPauseMode	m_NowUIMode;		// ���� UI
	EUIMapList		m_MapSelected;		// �ʿ��� ���� ���õ� UI
	EUIPauseList	m_PauseSelected;	// �Ͻ����� �޴����� ���� ���õ� UI
	EUISoundList	m_SoundSelected;	// ���� �ɼ� �޴����� ���� ���õ� UI
	EUICameraList	m_CameraSelected;	// ī�޶� �ɼ� �޴����� ���� ���õ� UI
	EUISaveList		m_SaveSelected;		// ����/�ҷ����� �޴����� ���� ���õ� UI

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
	void KeyLeftButton();	// ���콺 �޹�ư Ŭ��
	void KeyRightButton();	// ���콺 ���ư Ŭ��

	void KeyUp();			// �� ȭ��ǥ/w �Է�
	void KeyDown();			// �Ʒ� ȭ��ǥ/s �Է�
	void KeyLeft();			// �� ȭ��ǥ/a �Է�
	void KeyRight();		// �� ȭ��ǥ/d �Է�
	void KeyE();			// E �Է�
	void KeySpace();		// �����̽��� �Է�
	void KeyEsc();			// Esc �Է�

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
	void SaveSelectSaveGame();		// ���� ���� ����
	void SaveSelectLoadGame();		// ���� �ҷ��� ����


private: // Option Menu UI Func
	void SoundOptionUISFX();
	void SoundOptionUIMusic();
	void SoundOptionUITalk();
	void SoundOptionUIReset();

};

