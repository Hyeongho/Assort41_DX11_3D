#include "PauseUI.h"

#include "Device.h"
#include "Engine.h"
#include "Input.h"
#include "Scene/SceneManager.h"
#include "Scene/SceneViewport.h"
#include "Scene/Scene.h"
#include "UI/UIButton.h"
#include "UI/UIText.h"
#include "UI/UIImage.h"
#include "UI/UITextButton.h"

#include "../Scene/TitleSceneInfo.h"

CPauseUI::CPauseUI() :
	m_NowUIMode(EUIPauseMode::PauseMain),
	m_MapSelected(EUIMapList::End),
	m_PauseSelected(EUIPauseList::OptionSound),
	m_SoundSelected(EUISoundList::SFX),
	m_CameraSelected(EUICameraList::End),
	m_SaveSelected(EUISaveList::First)
{
	m_WindowTypeName = "PauseUI";
}

CPauseUI::CPauseUI(const CPauseUI& Window) :
	CUIWindow(Window),
	m_NowUIMode(EUIPauseMode::PauseMain),
	m_MapSelected(EUIMapList::End),
	m_PauseSelected(EUIPauseList::OptionSound),
	m_SoundSelected(EUISoundList::SFX),
	m_CameraSelected(EUICameraList::End),
	m_SaveSelected(EUISaveList::First)
{
}

CPauseUI::~CPauseUI()
{
}

void CPauseUI::Start()
{
	CUIWindow::Start();

	// KeySetting
	{
		CInput::GetInst()->AddBindFunction<CPauseUI>("UArrow", Input_Type::Up, this, &CPauseUI::KeyUp, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("W", Input_Type::Up, this, &CPauseUI::KeyUp, m_Scene);

		CInput::GetInst()->AddBindFunction<CPauseUI>("DArrow", Input_Type::Up, this, &CPauseUI::KeyDown, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("S", Input_Type::Up, this, &CPauseUI::KeyDown, m_Scene);

		CInput::GetInst()->AddBindFunction<CPauseUI>("LArrow", Input_Type::Up, this, &CPauseUI::KeyLeft, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("A", Input_Type::Up, this, &CPauseUI::KeyLeft, m_Scene);

		CInput::GetInst()->AddBindFunction<CPauseUI>("RArrow", Input_Type::Up, this, &CPauseUI::KeyRight, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("D", Input_Type::Up, this, &CPauseUI::KeyRight, m_Scene);

		CInput::GetInst()->AddBindFunction<CPauseUI>("Space", Input_Type::Up, this, &CPauseUI::KeySpace, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("Esc", Input_Type::Up, this, &CPauseUI::KeyBack, m_Scene);

		CInput::GetInst()->AddBindFunction<CPauseUI>("LButton", Input_Type::Up, this, &CPauseUI::KeyLeftButton, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("RButton", Input_Type::Up, this, &CPauseUI::KeyRightButton, m_Scene);
	}
}

bool CPauseUI::Init()
{
	CUIWindow::Init();

	CreateBackgroundUI();
	CreateMapUI();
	CreatePauseUI();
	CreateOptionSoundUI();
	CreateOptionCameraUI();
	CreateControlUI();
	CreateSaveSelectUI();

	InActiveMapUI();
	InActiveOptionSoundUI();
	InActiveOptionCameraUI();
	InActiveSaveSelectUI();
	InActiveControlUI();

	return true;
}

void CPauseUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CPauseUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CPauseUI::Render()
{
	CUIWindow::Render();
}

CPauseUI* CPauseUI::Clone()
{
	return new CPauseUI(*this);
}

void CPauseUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CPauseUI::Load(FILE* File)
{
	CUIWindow::Load(File);
}

void CPauseUI::CreateBackgroundUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	CUIImage* Image = CreateWidget<CUIImage>("BackUI_Background");
	Image->SetTexture("PuaseUIBackground", TEXT("UI/Pause/UI_bg.tga"));
	Image->SetSize((float)RS.Width, (float)RS.Height);
	Image->SetPos(0.f, 0.f);

	m_mapBackUI.insert(std::make_pair("BackUI_Background", Image));


	Image = CreateWidget<CUIImage>("BackUI_MenuBackground");
	Image->SetTexture("PauseUIMenuBackground", TEXT("UI/Pause/UI_Map_3.tga"));
	Image->SetSize((float)RS.Width, (float)RS.Height);
	Image->SetPos(0.f, 0.f);

	m_mapBackUI.insert(std::make_pair("BackUI_MenuBackground", Image));


	Image = CreateWidget<CUIImage>("BackUI_BackgroundOverlay");
	Image->SetTexture("PuaseUIBackgroundOverlay", TEXT("UI/Pause/UI_Map_bamboo_overlay.tga"));
	Image->SetSize((float)RS.Width, (float)RS.Height);
	Image->SetPos(0.f, 0.f);

	m_mapBackUI.insert(std::make_pair("BackUI_BackgroundOverlay", Image));



	float TextBannerSizeX = 300.f;
	float TextBannerSizeY = 100.f;

	Image = CreateWidget<CUIImage>("BackUI_TextBannerLeftBackground");
	Image->SetTexture("PauseUITextBannerLeftBackground", TEXT("UI/Pause/UI_Banner_side.tga"));
	Image->SetSize(TextBannerSizeX, TextBannerSizeY);
	Image->SetPos((float)RS.Width / 2.f - TextBannerSizeX, (float)RS.Height - TextBannerSizeY);

	m_mapBackUI.insert(std::make_pair("BackUI_TextBannerLeftBackground", Image));



	Image = CreateWidget<CUIImage>("BackUI_TextBannerRightBackground");
	Image->SetTexture("PauseUITextBannerRightBackground", TEXT("UI/Pause/UI_Banner_side_2.tga"));
	Image->SetSize(TextBannerSizeX, TextBannerSizeY);
	Image->SetPos((float)RS.Width / 2.f, (float)RS.Height - TextBannerSizeY);

	m_mapBackUI.insert(std::make_pair("BackUI_TextBannerRightBackground", Image));


	float FontSize = 60.f;

	CUIText* Text = CreateWidget<CUIText>("BackUI_TextBanner");
	Text->SetSize(TextBannerSizeX * 2.f, TextBannerSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetAlignV(Text_Align_V::Middle);
	Text->SetPos((float)RS.Width / 2.f - TextBannerSizeX, (float)RS.Height - FontSize * 1.5f);
	Text->SetFontSize(FontSize);
	Text->SetText(TEXT("옵션"));
	Text->SetColor(Vector4::Black);

	m_mapBackUI.insert(std::make_pair("BackUI_TextBanner", Text));
}

void CPauseUI::CreateMapUI()
{
}

void CPauseUI::CreatePauseUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float ButtonSizeX = 300.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = RS.Width / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height - 200.f;

	float ButtonYInterval = ButtonSizeY + 10.f;

	float FontSize = 30.f;


	CUIImage* Image = CreateWidget<CUIImage>("PauseUI_SelectedSplotch");
	Image->SetTexture("SelectBackSplotch", TEXT("Title/Color_splotch_2.tga"));
	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPivot(0.f, 0.3f);

	m_mapPauseUI.insert(std::make_pair("PauseUI_SelectedSplotch", Image));


	CUITextButton* TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonSoundOption");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("사운드 설정"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("사운드 설정"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("사운드 설정"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonSoundOptionHovered", false, "Sfx/Title/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonSoundOptionClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUISoundHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUISound);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonSoundOption", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonCameraOption");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("카메라 설정"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("카메라 설정"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("카메라 설정"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonCameraOptionHovered", false, "Sfx/Title/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonCameraOptionClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUICameraHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUICamera);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonCameraOption", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonControl");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("조작법"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("조작법"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("조작법"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonControlHovered", false, "Sfx/Title/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonControlClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUIControlHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUIControl);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonControl", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonSaveGame");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("게임 저장"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("게임 저장"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("게임 저장"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonSaveGameHovered", false, "Sfx/Title/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonSaveGameClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUISaveHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUISave);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonSaveGame", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonLoadGame");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("게임 불러오기"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("게임 불러오기"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("게임 불러오기"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonLoadGameHovered", false, "Sfx/Title/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonLoadGameClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUILoadHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUILoad);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonLoadGame", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonBackToTitle");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("기본 메뉴로 돌아가기"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("기본 메뉴로 돌아가기"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("기본 메뉴로 돌아가기"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonBackToTitleHovered", false, "Sfx/Title/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonBackToTitleClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUIBackToTitleHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUIBackToTitle);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonBackToTitle", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonQuit");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("종료"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("종료"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("종료"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonQuitHovered", false, "Sfx/Title/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonQuitClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUIQuitHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUIQuit);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonQuit", TextButton));

}

void CPauseUI::CreateOptionSoundUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float ButtonSizeX = 300.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = RS.Width / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height - 200.f;

	float ButtonYInterval = ButtonSizeY + 10.f;

	float FontSize = 30.f;


	CUIImage* Image = CreateWidget<CUIImage>("SoundOptionUI_SelectedSplotch");
	Image->SetTexture("SelectBackSplotch", TEXT("Title/Color_splotch_2.tga"));
	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPivot(0.f, 0.3f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_SelectedSplotch", Image));


	CUIText* Text = CreateWidget<CUIText>("SoundOptionUI_Text");

	Text->SetSize(ButtonSizeX * 2.f, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX - ButtonSizeX / 2.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("사운드 옵션"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_Text", Text));

	ButtonPosY -= ButtonYInterval;
	ButtonPosY -= 50.f;


	CUITextButton* TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonSfx");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("SFX 볼륨"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("SFX 볼륨"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("SFX 볼륨"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonSfxHovered", false, "Sfx/Title/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonSfxClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SoundOptionUISFX);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonSfx", TextButton));
	
	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonMusic");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Music 볼륨"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("Music 볼륨"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("Music 볼륨"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonMusicHovered", false, "Sfx/Title/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonMusicClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SoundOptionUIMusic);
	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonMusic", TextButton));

	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonTalk");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Talk 볼륨"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("Talk 볼륨"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("Talk 볼륨"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonTalkHovered", false, "Sfx/Title/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonTalkClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SoundOptionUITalk);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonTalk", TextButton));

	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonReset");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("볼륨 설정 초기화"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("볼륨 설정 초기화"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("볼륨 설정 초기화"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonResetHovered", false, "Sfx/Title/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonResetClick", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SoundOptionUIReset);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonReset", TextButton));
}

void CPauseUI::CreateOptionCameraUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float ButtonSizeX = 300.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = RS.Width / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height - 200.f;

	float ButtonYInterval = ButtonSizeY + 50.f;

	float FontSize = 30.f;
}

void CPauseUI::CreateControlUI()
{
}

void CPauseUI::CreateSaveSelectUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float ButtonSizeX = 300.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = RS.Width / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height - 200.f;

	float ButtonYInterval = ButtonSizeY + 50.f;

	float FontSize = 30.f;


	CUIImage* Image = CreateWidget<CUIImage>("SaveSelectUI_SelectedSplotch");
	Image->SetTexture("SelectBackSplotch", TEXT("Title/Color_splotch_2.tga"));
	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPivot(0.f, 0.3f);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_SelectedSplotch", Image));


	CUIText* Text = CreateWidget<CUIText>("SaveSelectUI_Text");

	Text->SetSize(ButtonSizeX * 2.f, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX - ButtonSizeX / 2.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("슬롯을 선택하여"));
	Text->SetColor(Vector4::Black);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_Text", Text));

	ButtonPosY -= ButtonYInterval;


	CUITextButton* TextButton = CreateWidget<CUITextButton>("SaveSelectUI_ButtonSaveSLot1");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SaveSelectUI_ButtonSaveSLot1Hovered", false, "Sfx/Title/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SaveSelectUI_ButtonSaveSLot1Click", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SaveSelectUISaveSlot1Hoverd);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::SaveSelectClick);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_ButtonSaveSLot1", TextButton));

	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SaveSelectUI_ButtonSaveSLot2");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SaveSelectUI_ButtonSaveSLot2Hovered", false, "Sfx/Title/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SaveSelectUI_ButtonSaveSLot2Click", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SaveSelectUISaveSlot2Hoverd);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::SaveSelectClick);
	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_ButtonSaveSLot2", TextButton));

	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SaveSelectUI_ButtonSaveSLot3");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("빈 슬롯"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SaveSelectUI_ButtonSaveSLot3Hovered", false, "Sfx/Title/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SaveSelectUI_ButtonSaveSLot3Click", false, "Sfx/Title/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SaveSelectUISaveSlot3Hoverd);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::SaveSelectClick);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_ButtonSaveSLot3", TextButton));
}

void CPauseUI::ActiveMapUI()
{
	for (auto iter : m_mapMapUI)
		iter.second->SetEnable(true);
}

void CPauseUI::ActivePauseUI()
{
	for (auto iter : m_mapPauseUI)
		iter.second->SetEnable(true);
}

void CPauseUI::ActiveOptionSoundUI()
{
	for (auto iter : m_mapSoundOptionUI)
		iter.second->SetEnable(true);
}

void CPauseUI::ActiveOptionCameraUI()
{
	for (auto iter : m_mapCameraOptionUI)
		iter.second->SetEnable(true);
}

void CPauseUI::ActiveSaveSelectUI()
{
	for (auto iter : m_mapSaveSelectUI)
		iter.second->SetEnable(true);
}

void CPauseUI::ActiveControlUI()
{
	for (auto iter : m_mapControlUI)
		iter.second->SetEnable(true);
}

void CPauseUI::InActiveMapUI()
{
	for (auto iter : m_mapMapUI)
		iter.second->SetEnable(false);
}

void CPauseUI::InActivePauseUI()
{
	for (auto iter : m_mapPauseUI)
		iter.second->SetEnable(false);
}

void CPauseUI::InActiveOptionSoundUI()
{
	for (auto iter : m_mapSoundOptionUI)
		iter.second->SetEnable(false);
}

void CPauseUI::InActiveOptionCameraUI()
{
	for (auto iter : m_mapCameraOptionUI)
		iter.second->SetEnable(false);
}

void CPauseUI::InActiveSaveSelectUI()
{
	for (auto iter : m_mapSaveSelectUI)
		iter.second->SetEnable(false);
}

void CPauseUI::InActiveControlUI()
{
	for (auto iter : m_mapControlUI)
		iter.second->SetEnable(false);
}

void CPauseUI::KeyLeftButton()
{
}

void CPauseUI::KeyRightButton()
{
	CSound* Sound = m_Scene->GetResource()->FindSound("UI_Backward");

	if (Sound) {
		Sound->Play();
	}

	if (m_NowUIMode == EUIPauseMode::PauseMap)
	{
		// UI 끄기


	}
	else if (m_NowUIMode == EUIPauseMode::PauseMain) 
	{
		// 맵으로 이동. 하지만 일단은 작동 안하게 설정
		return;
		m_NowUIMode = EUIPauseMode::PauseMap;
		InActivePauseUI();
		ActiveMapUI();

		CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();

		switch (m_MapSelected)
		{
		case EUIMapList::End:
			break;
		default:
			Text->SetText("지도");
			break;
		}
	}
	else
	{
		switch (m_NowUIMode)
		{
		case EUIPauseMode::OptionSound:
			InActiveOptionSoundUI();
			break;
		case EUIPauseMode::OptionCamera:
			InActiveOptionCameraUI();
			break;
		case EUIPauseMode::Control:
			InActiveControlUI();
			break;
		case EUIPauseMode::SaveGame:
		case EUIPauseMode::LoadGame:
			InActiveSaveSelectUI();
			break;
		}

		m_NowUIMode = EUIPauseMode::PauseMain;
		ActivePauseUI();

		CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
		Text->SetText("옵션");
	}
}

void CPauseUI::KeyUp()
{
}

void CPauseUI::KeyDown()
{
}

void CPauseUI::KeyLeft()
{
}

void CPauseUI::KeyRight()
{
}

void CPauseUI::KeyE()
{
}

void CPauseUI::KeySpace()
{
}

void CPauseUI::KeyBack()
{
}

void CPauseUI::PauseUISound()
{
	m_NowUIMode = EUIPauseMode::OptionSound;

	InActivePauseUI();
	ActiveOptionSoundUI();


	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("사운드 설정");
}

void CPauseUI::PauseUICamera()
{
	m_NowUIMode = EUIPauseMode::OptionCamera;

	InActivePauseUI();
	ActiveOptionCameraUI();


	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("카메라 설정");
}

void CPauseUI::PauseUIControl()
{
	m_NowUIMode = EUIPauseMode::Control;

	InActivePauseUI();
	ActiveControlUI();


	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("조작법");
}

void CPauseUI::PauseUISave()
{
	m_NowUIMode = EUIPauseMode::SaveGame;

	InActivePauseUI();
	ActiveSaveSelectUI();


	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("게임 저장");
}

void CPauseUI::PauseUILoad()
{
	m_NowUIMode = EUIPauseMode::LoadGame;

	InActivePauseUI();
	ActiveSaveSelectUI();


	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("게임 불러오기");
}

void CPauseUI::PauseUIBackToTitle()
{
	CSceneManager::GetInst()->CreateNextScene(true);
	CSceneManager::GetInst()->CreateSceneInfo<CTitleSceneInfo>(false);
}

void CPauseUI::PauseUIQuit()
{
	// 게임 종료 처리.

}

void CPauseUI::PauseUISoundHovered()
{
	m_PauseSelected = EUIPauseList::OptionSound;

	auto iterSplotch = m_mapPauseUI.find("PauseUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapPauseUI.find("PauseUI_ButtonSoundOption")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::PauseUICameraHovered()
{
	m_PauseSelected = EUIPauseList::OptionCamera;

	auto iterSplotch = m_mapPauseUI.find("PauseUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapPauseUI.find("PauseUI_ButtonCameraOption")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::PauseUIControlHovered()
{
	m_PauseSelected = EUIPauseList::Control;

	auto iterSplotch = m_mapPauseUI.find("PauseUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapPauseUI.find("PauseUI_ButtonControl")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::PauseUISaveHovered()
{
	m_PauseSelected = EUIPauseList::SaveGame;

	auto iterSplotch = m_mapPauseUI.find("PauseUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapPauseUI.find("PauseUI_ButtonSaveGame")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::PauseUILoadHovered()
{
	m_PauseSelected = EUIPauseList::LoadGame;

	auto iterSplotch = m_mapPauseUI.find("PauseUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapPauseUI.find("PauseUI_ButtonLoadGame")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::PauseUIBackToTitleHovered()
{
	m_PauseSelected = EUIPauseList::BackToTitle;

	auto iterSplotch = m_mapPauseUI.find("PauseUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapPauseUI.find("PauseUI_ButtonBackToTitle")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::PauseUIQuitHovered()
{
	m_PauseSelected = EUIPauseList::BackToTitle;

	auto iterSplotch = m_mapPauseUI.find("PauseUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapPauseUI.find("PauseUI_ButtonQuit")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::SaveSelectUISaveSlot1Hoverd()
{
	m_SaveSelected = EUISaveList::First;
}

void CPauseUI::SaveSelectUISaveSlot2Hoverd()
{
	m_SaveSelected = EUISaveList::Second;
}

void CPauseUI::SaveSelectUISaveSlot3Hoverd()
{
	m_SaveSelected = EUISaveList::Third;
}

void CPauseUI::SaveSelectClick()
{
	if (m_NowUIMode == EUIPauseMode::SaveGame)
	{
		SaveSelectSaveGame();
	}
	else if (m_NowUIMode == EUIPauseMode::LoadGame) 
	{
		SaveSelectLoadGame();
	}
}

void CPauseUI::SaveSelectSaveGame()
{
	switch (m_SaveSelected)
	{
	case EUISaveList::First:
		break;
	case EUISaveList::Second:
		break;
	case EUISaveList::Third:
		break;
	}
}

void CPauseUI::SaveSelectLoadGame()
{
	switch (m_SaveSelected)
	{
	case EUISaveList::First:
		break;
	case EUISaveList::Second:
		break;
	case EUISaveList::Third:
		break;
	}
}

void CPauseUI::SoundOptionUISFX()
{
	m_SoundSelected = EUISoundList::SFX;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonSfx")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::SoundOptionUIMusic()
{
	m_SoundSelected = EUISoundList::Music;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonMusic")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::SoundOptionUITalk()
{
	m_SoundSelected = EUISoundList::Talk;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonTalk")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::SoundOptionUIReset()
{
	m_SoundSelected = EUISoundList::Reset;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonReset")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}
