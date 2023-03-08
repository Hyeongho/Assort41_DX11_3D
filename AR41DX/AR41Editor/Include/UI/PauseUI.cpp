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
	m_NowUIMode(EUIPauseMode::Close),
	m_MapSelected(EMapList::End),
	m_PauseSelected(EUIPauseList::OptionSound),
	m_SoundSelected(EUISoundList::SFX),
	m_CameraSelected(EUICameraList::End),
	m_SaveSelected(EUISaveList::First)
{
	m_WindowTypeName = "PauseUI";
}

CPauseUI::CPauseUI(const CPauseUI& Window) :
	CUIWindow(Window),
	m_NowUIMode(EUIPauseMode::Close),
	m_MapSelected(EMapList::End),
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
		CInput::GetInst()->AddBindFunction<CPauseUI>("Esc", Input_Type::Up, this, &CPauseUI::KeyEsc, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("E", Input_Type::Up, this, &CPauseUI::KeyE, m_Scene);

		CInput::GetInst()->AddBindFunction<CPauseUI>("LButton", Input_Type::Up, this, &CPauseUI::KeyLeftButton, m_Scene);
		CInput::GetInst()->AddBindFunction<CPauseUI>("RButton", Input_Type::Up, this, &CPauseUI::KeyRightButton, m_Scene);
	}

	CreaeteAllUI();
}

bool CPauseUI::Init()
{
	CUIWindow::Init();

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

	CreaeteAllUI();
}

void CPauseUI::OpenUI()
{
	CloseUI();

	ActiveBackUI();
	ActiveMapUI();

	m_NowUIMode = EUIPauseMode::PauseMap;
}

void CPauseUI::CloseUI()
{
	InActiveBackUI();
	InActiveMapUI();
	InActivePauseUI();
	InActiveOptionSoundUI();
	InActiveOptionCameraUI();
	InActiveSaveSelectUI();
	InActiveControlUI();

	m_NowUIMode = EUIPauseMode::Close;
}

void CPauseUI::CreaeteAllUI()
{
	if (!m_mapBackUI.empty())
		return;

	CreateBackgroundUI();
	CreateMapUI();
	CreatePauseUI();
	CreateOptionSoundUI();
	CreateOptionCameraUI();
	CreateControlUI();
	CreateSaveSelectUI();

	m_NowUIMode = EUIPauseMode::Close;

	CloseUI();
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
	Resolution	RS = CDevice::GetInst()->GetResolution();

	float ButtonSizeX = 232.f;
	float ButtonSizeY = 232.f;

	float ButtonPosX = 0.f;
	float ButtonPosY = 450.f;


	float LeftPadding = 40.f;


	
	CUIButton* Button = CreateWidget<CUIButton>("MapUI_Patric");
	Button->SetTexture(EButtonState::Normal, "MapUI_Patric", TEXT("UI/Pause/Map/Partick_01.tga"));
	Button->SetTexture(EButtonState::Hovered, "MapUI_PatricColor", TEXT("UI/Pause/Map/Partick_02.tga"));
	Button->SetTexture(EButtonState::Click, "MapUI_PatricColor", TEXT("UI/Pause/Map/Partick_02.tga"));
	Button->SetPos(ButtonPosX + LeftPadding, ButtonPosY);
	Button->SetSize(232.f, 232.f);

	m_mapMapUI.insert(std::make_pair("MapUI_Patric", Button));


	ButtonPosX += 232.f;

	Button = CreateWidget<CUIButton>("MapUI_DutchMan_Grave");
	Button->SetTexture(EButtonState::Normal, "MapUI_DutchMan_Grave", TEXT("UI/Pause/Map/UI_DMG.tga"));
	Button->SetTexture(EButtonState::Hovered, "MapUI_DutchMan_GraveColor", TEXT("UI/Pause/Map/UI_DMG_color.tga"));
	Button->SetTexture(EButtonState::Click, "MapUI_DutchMan_GraveColor", TEXT("UI/Pause/Map/UI_DMG_color.tga"));
	Button->SetPos(ButtonPosX + LeftPadding, ButtonPosY);
	Button->SetSize(154.f, 221.f);
	
	m_mapMapUI.insert(std::make_pair("MapUI_DutchMan_Grave", Button));


	ButtonPosX += ButtonSizeX;

	Button = CreateWidget<CUIButton>("MapUI_Sand_Mountain");
	Button->SetTexture(EButtonState::Normal, "MapUI_Sand_Mountain", TEXT("UI/Pause/Map/UI_SM.tga"));
	Button->SetTexture(EButtonState::Hovered, "MapUI_Sand_MountainColor", TEXT("UI/Pause/Map/UI_SM_color.tga"));
	Button->SetTexture(EButtonState::Click, "MapUI_Sand_MountainColor", TEXT("UI/Pause/Map/UI_SM_color.tga"));
	Button->SetPos(ButtonPosX + LeftPadding, ButtonPosY);
	Button->SetSize(275.f, 254.f);

	m_mapMapUI.insert(std::make_pair("MapUI_Sand_Mountain", Button));


	ButtonPosX += ButtonSizeX;

	Button = CreateWidget<CUIButton>("MapUI_Goo_Lagoon");
	Button->SetTexture(EButtonState::Normal, "MapUI_Goo_Lagoon", TEXT("UI/Pause/Map/UI_GL.tga"));
	Button->SetTexture(EButtonState::Hovered, "MapUI_Goo_LagoonColor", TEXT("UI/Pause/Map/UI_GL_color.tga"));
	Button->SetTexture(EButtonState::Click, "MapUI_Goo_LagoonColor", TEXT("UI/Pause/Map/UI_GL_color.tga"));
	Button->SetPos(ButtonPosX + LeftPadding, ButtonPosY);
	Button->SetSize(ButtonSizeX, ButtonSizeY);

	m_mapMapUI.insert(std::make_pair("MapUI_Goo_Lagoon", Button));


	ButtonPosX += ButtonSizeX;

	Button = CreateWidget<CUIButton>("MapUI_Dome");
	Button->SetTexture(EButtonState::Normal, "MapUI_Dome", TEXT("UI/Pause/Map/UI_DOME.tga"));
	Button->SetTexture(EButtonState::Hovered, "MapUI_DomeColor", TEXT("UI/Pause/Map/UI_DOME_color.tga"));
	Button->SetTexture(EButtonState::Click, "MapUI_DomeColor", TEXT("UI/Pause/Map/UI_DOME_color.tga"));
	Button->SetPos(ButtonPosX + LeftPadding, ButtonPosY);
	Button->SetSize(ButtonSizeX, ButtonSizeY);

	m_mapMapUI.insert(std::make_pair("MapUI_Dome", Button));


	ButtonPosX += ButtonSizeX;

	Button = CreateWidget<CUIButton>("MapUI_KK");
	m_mapMapUI.insert(std::make_pair("MapUI_KK", Button));

	Button = CreateWidget<CUIButton>("MapUI_Chum_Bucketlab");
	m_mapMapUI.insert(std::make_pair("MapUI_Chum_Bucketlab", Button));

	Button = CreateWidget<CUIButton>("MapUI_Merma_Lair");
	m_mapMapUI.insert(std::make_pair("MapUI_Merma_Lair", Button));

	Button = CreateWidget<CUIButton>("MapUI_Bikini_Bottom");
	m_mapMapUI.insert(std::make_pair("MapUI_Bikini_Bottom", Button));

	Button = CreateWidget<CUIButton>("MapUI_Down_Town");
	m_mapMapUI.insert(std::make_pair("MapUI_Down_Town", Button));

	Button = CreateWidget<CUIButton>("MapUI_Krabs");
	m_mapMapUI.insert(std::make_pair("MapUI_Krabs", Button));

	Button = CreateWidget<CUIButton>("MapUI_Kelp_Forest");
	m_mapMapUI.insert(std::make_pair("MapUI_Kelp_Forest", Button));

	Button = CreateWidget<CUIButton>("MapUI_Rock_Bottom");
	m_mapMapUI.insert(std::make_pair("MapUI_Rock_Bottom", Button));

	Button = CreateWidget<CUIButton>("MapUI_Jelly_Fish_Field");
	m_mapMapUI.insert(std::make_pair("MapUI_Jelly_Fish_Field", Button));

	Button = CreateWidget<CUIButton>("MapUI_Industrial_Park");
	m_mapMapUI.insert(std::make_pair("MapUI_Industrial_Park", Button));




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
	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));
	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPivot(0.f, 0.3f);

	m_mapPauseUI.insert(std::make_pair("PauseUI_SelectedSplotch", Image));


	CUITextButton* TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonSoundOption");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("사운드 설정"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("사운드 설정"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("사운드 설정"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonSoundOptionHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonSoundOptionClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUISoundHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUISound);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonSoundOption", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonCameraOption");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("카메라 설정"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("카메라 설정"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("카메라 설정"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonCameraOptionHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonCameraOptionClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUICameraHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUICamera);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonCameraOption", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonControl");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("조작법"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("조작법"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("조작법"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonControlHovered", false, "Sfx/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonControlClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUIControlHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUIControl);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonControl", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonSaveGame");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("게임 저장"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("게임 저장"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("게임 저장"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonSaveGameHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonSaveGameClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUISaveHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUISave);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonSaveGame", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonLoadGame");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("게임 불러오기"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("게임 불러오기"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("게임 불러오기"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonLoadGameHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonLoadGameClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUILoadHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUILoad);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonLoadGame", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonBackToTitle");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("기본 메뉴로 돌아가기"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("기본 메뉴로 돌아가기"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("기본 메뉴로 돌아가기"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonBackToTitleHovered", false, "Sfx/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonBackToTitleClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::PauseUIBackToTitleHovered);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::PauseUIBackToTitle);

	m_mapPauseUI.insert(std::make_pair("PauseUI_ButtonBackToTitle", TextButton));

	ButtonPosY -= ButtonYInterval;

	TextButton = CreateWidget<CUITextButton>("PauseUI_ButtonQuit");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("종료"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("종료"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("종료"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "PauseUI_ButtonQuitHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "PauseUI_ButtonQuitClick", false, "Sfx/SFX_UI_Forward.ogg");
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
	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));
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

	ButtonPosX = RS.Width / 3.f - ButtonSizeX / 2.f;

	CUITextButton* TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonSfx");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("SFX 볼륨"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("SFX 볼륨"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("SFX 볼륨"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonSfxHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonSfxClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SoundOptionUISFX);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonSfx", TextButton));
	
	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonMusic");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Music 볼륨"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("Music 볼륨"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("Music 볼륨"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonMusicHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonMusicClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SoundOptionUIMusic);
	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonMusic", TextButton));

	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonTalk");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Talk 볼륨"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("Talk 볼륨"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("Talk 볼륨"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonTalkHovered", false, "Sfx/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonTalkClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SoundOptionUITalk);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonTalk", TextButton));

	ButtonPosY -= ButtonYInterval;



	ButtonPosX = RS.Width / 2.f - ButtonSizeX / 2.f;

	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonReset");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("볼륨 설정 초기화"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("볼륨 설정 초기화"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("볼륨 설정 초기화"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonResetHovered", false, "Sfx/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonResetClick", false, "Sfx/SFX_UI_Forward.ogg");
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

	float ButtonYInterval = ButtonSizeY + 10.f;

	float FontSize = 30.f;



	CUIImage* Image = CreateWidget<CUIImage>("CameraOptionUI_SelectedSplotch");
	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));
	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPivot(0.f, 0.3f);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_SelectedSplotch", Image));


	CUIText* Text = CreateWidget<CUIText>("CameraOptionUI_Text");

	Text->SetSize(ButtonSizeX * 2.f, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX - ButtonSizeX / 2.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("카메라 옵션"));
	Text->SetColor(Vector4::Black);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_Text", Text));

	ButtonPosY -= ButtonYInterval;
	ButtonPosY -= 50.f;

	ButtonPosX = RS.Width / 3.f - ButtonSizeX / 2.f;


	// 감도 조절

	CUITextButton* TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonMouseSensitive");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("감도"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("감도"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("감도"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonMouseSensitiveHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonMouseSensitiveClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUISensitive);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonMouseSensitive", TextButton));


	Text = CreateWidget<CUIText>("CameraOptionUI_TextSensitive");

	Text->SetSize(ButtonSizeX * 2.f, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX + ButtonSizeX, ButtonPosY);
	Text->SetFontSize(30.f); 
	Text->SetText(TEXT("30"));
	Text->SetColor(Vector4::Black);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_TextSensitive", Text));


	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonSensitiveMinus");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX + ButtonSizeX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("-"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("-"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("-"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonSensitiveMinusHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonSensitiveMinusClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUISensitive);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::CameraOptionUISensitiveMinus);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonSensitiveMinus", TextButton));


	//Image = CreateWidget<CUIImage>("CameraOptionUI_ImageSensitiveSlideBar");
	//Image = CreateWidget<CUIImage>("CameraOptionUI_ImageSensitiveSlideBar");


	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonSensitivePlus");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX + ButtonSizeX * 2.f, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("+"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("+"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("+"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonSensitivePlusHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonSensitivePlusClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUISensitive);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::CameraOptionUISensitivePlus);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonSensitivePlus", TextButton));


	ButtonPosY -= ButtonYInterval;


	// X 축 반전
	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonXReverse");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("X축 반전"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("X축 반전"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("X축 반전"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonXReverseHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonXReverseClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUIXReverse);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonXReverse", TextButton));



	Image = CreateWidget<CUIImage>("CameraOptionUI_XReverseSelectedSplotch");
	Image->SetTexture("ReverseBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));
	Image->SetSize(ButtonSizeX * 0.6f, ButtonSizeY * 0.8f);
	Image->SetPos(ButtonPosX + ButtonSizeX, ButtonPosY);
	Image->SetPivot(-0.4f, 0.f);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_XReverseSelectedSplotch", Image));



	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonXReverseNormal");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX + ButtonSizeX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Normal"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("Normal"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("Normal"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonXReverseNormalHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonXReverseNormalClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUIXReverse);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::CameraOptionUIXReverse_Normal);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonXReverseNormal", TextButton));


	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonXReverseReverse");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX + ButtonSizeX * 2.f, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Reverse"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("Reverse"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("Reverse"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonXReverseReverseHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonXReverseReverseClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUIXReverse);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::CameraOptionUIXReverse_Reverse);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonXReverseReverse", TextButton));


	ButtonPosY -= ButtonYInterval;



	// Y 축 반전
	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonYReverse");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Y축 반전"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("Y축 반전"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("Y축 반전"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonYReverseHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonYReverseClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUIYReverse);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonYReverse", TextButton));



	Image = CreateWidget<CUIImage>("CameraOptionUI_YReverseSelectedSplotch");
	Image->SetTexture("ReverseBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));
	Image->SetSize(ButtonSizeX * 0.6f, ButtonSizeY * 0.8f);
	Image->SetPos(ButtonPosX + ButtonSizeX, ButtonPosY);
	Image->SetPivot(-0.4f, 0.f);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_YReverseSelectedSplotch", Image));


	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonYReverseNormal");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX + ButtonSizeX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Normal"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("Normal"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("Normal"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonYReverseNormalHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonYReverseNormalClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUIYReverse);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::CameraOptionUIYReverse_Normal);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonYReverseNormal", TextButton));


	TextButton = CreateWidget<CUITextButton>("CameraOptionUI_ButtonYReverseReverse");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX + ButtonSizeX * 2.f, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("Reverse"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Hovered, TEXT("Reverse"), FontSize, Vector4::Black);
	TextButton->SetText(EButtonState::Click, TEXT("Reverse"), FontSize, Vector4::Black);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "CameraOptionUI_ButtonYReverseReverseHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "CameraOptionUI_ButtonYReverseReverseClick", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::CameraOptionUIYReverse);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::CameraOptionUIYReverse_Reverse);
	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_ButtonYReverseReverse", TextButton));
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
	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));
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
	TextButton->SetText(EButtonState::Normal, TEXT("빈 슬롯"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("빈 슬롯"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("빈 슬롯"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SaveSelectUI_ButtonSaveSLot1Hovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SaveSelectUI_ButtonSaveSLot1Click", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SaveSelectUISaveSlot1Hoverd);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::SaveSelectClick);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_ButtonSaveSLot1", TextButton));

	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SaveSelectUI_ButtonSaveSLot2");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("빈 슬롯"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("빈 슬롯"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("빈 슬롯"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SaveSelectUI_ButtonSaveSLot2Hovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SaveSelectUI_ButtonSaveSLot2Click", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SaveSelectUISaveSlot2Hoverd);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::SaveSelectClick);
	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_ButtonSaveSLot2", TextButton));

	ButtonPosY -= ButtonYInterval;


	TextButton = CreateWidget<CUITextButton>("SaveSelectUI_ButtonSaveSLot3");
	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);
	TextButton->SetText(EButtonState::Normal, TEXT("빈 슬롯"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("빈 슬롯"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("빈 슬롯"), FontSize, Vector4::White);
	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SaveSelectUI_ButtonSaveSLot3Hovered", false, "Sfx/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SaveSelectUI_ButtonSaveSLot3Click", false, "Sfx/SFX_UI_Forward.ogg");
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Hovered, this, &CPauseUI::SaveSelectUISaveSlot3Hoverd);
	TextButton->SetCallback<CPauseUI>(EButtonEventState::Click, this, &CPauseUI::SaveSelectClick);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_ButtonSaveSLot3", TextButton));
}

void CPauseUI::ActiveBackUI()
{
	for (auto iter : m_mapBackUI)
		iter.second->SetEnable(true);
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

void CPauseUI::InActiveBackUI()
{
	for (auto iter : m_mapBackUI)
		iter.second->SetEnable(false);
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
	if (m_NowUIMode == EUIPauseMode::Close)
		return;

	CSound* Sound = m_Scene->GetResource()->FindSound("UI_Backward");

	if (Sound) {
		Sound->Play();
	}

	if (m_NowUIMode == EUIPauseMode::PauseMap)
	{
		// UI 끄기
		CloseUI();

	}
	else if (m_NowUIMode == EUIPauseMode::PauseMain) 
	{
		// 맵으로 이동. 
		m_NowUIMode = EUIPauseMode::PauseMap;
		InActivePauseUI();
		ActiveMapUI();

		CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
		//BackUI_BackgroundOverlay

		CUIImage* Image = (CUIImage*)m_mapBackUI.find("BackUI_BackgroundOverlay")->second.Get();
		Image->SetTexture("PuaseUIBackgroundOverlay2", TEXT("UI/Pause/UI_Map_overlay.tga"));

		switch (m_MapSelected)
		{
		case EMapList::Patric:
			Text->SetText("뚱이");
			break;
		case EMapList::Chum_Bucketlab:
			Text->SetText("미끼식당 실험실");
			break;
		case EMapList::Bikini_Bottom:
			Text->SetText("비키니시티");
			break;
		case EMapList::Krabs:
			Text->SetText("집게사장");
			break;
		case EMapList::Rock_Bottom:
			Text->SetText("메롱 시티");
			break;
		case EMapList::Jelly_Fish_Field:
			Text->SetText("해파리 동산");
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
	if (m_NowUIMode == EUIPauseMode::PauseMap)
	{
		CUIImage* Image = (CUIImage*)m_mapBackUI.find("BackUI_BackgroundOverlay")->second.Get();
		Image->SetTexture("PuaseUIBackgroundOverlay", TEXT("UI/Pause/UI_Map_bamboo_overlay.tga"));

		CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
		Text->SetText("옵션");

		InActiveMapUI();
		ActivePauseUI();

		m_NowUIMode = EUIPauseMode::PauseMain;
	}
}

void CPauseUI::KeySpace()
{
}

void CPauseUI::KeyEsc()
{
	if (m_NowUIMode == EUIPauseMode::Close) {
		OpenUI();
	}
	else {
		KeyRightButton();
	}
}

void CPauseUI::PauseUISound()
{
	m_NowUIMode = EUIPauseMode::OptionSound;

	InActivePauseUI();
	ActiveOptionSoundUI();

	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("사운드 설정");

	// Splotch 위치 조정을 위한 Hover 액션 1회 실행
	SoundOptionUISFX();
}

void CPauseUI::PauseUICamera()
{
	m_NowUIMode = EUIPauseMode::OptionCamera;

	InActivePauseUI();
	ActiveOptionCameraUI();


	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("카메라 설정");

	// Splotch 위치 조정을 위한 Hover 액션 1회 실행

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

	// Splotch 위치 조정을 위한 Hover 액션 1회 실행
	SaveSelectUISaveSlot1Hoverd();
}

void CPauseUI::PauseUILoad()
{
	m_NowUIMode = EUIPauseMode::LoadGame;

	InActivePauseUI();
	ActiveSaveSelectUI();


	CUIText* Text = (CUIText*)m_mapBackUI.find("BackUI_TextBanner")->second.Get();
	Text->SetText("게임 불러오기");

	// Splotch 위치 조정을 위한 Hover 액션 1회 실행
	SaveSelectUISaveSlot1Hoverd();
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

	auto iterSplotch = m_mapSaveSelectUI.find("SaveSelectUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapSaveSelectUI.find("SaveSelectUI_ButtonSaveSLot1")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::SaveSelectUISaveSlot2Hoverd()
{
	m_SaveSelected = EUISaveList::Second;

	auto iterSplotch = m_mapSaveSelectUI.find("SaveSelectUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapSaveSelectUI.find("SaveSelectUI_ButtonSaveSLot2")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CPauseUI::SaveSelectUISaveSlot3Hoverd()
{
	m_SaveSelected = EUISaveList::Third;

	auto iterSplotch = m_mapSaveSelectUI.find("SaveSelectUI_SelectedSplotch");
	float PosX = CDevice::GetInst()->GetResolution().Width / 2.f - iterSplotch->second->GetSize().x / 2.f;
	float PosY = m_mapSaveSelectUI.find("SaveSelectUI_ButtonSaveSLot3")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
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
		// 게임 세이브 기능 실행

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
		// 게임 로드 기능 실행


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

void CPauseUI::CameraOptionUISensitive()
{
	m_CameraSelected = EUICameraList::Sensitive;

	auto iterSplotch = m_mapCameraOptionUI.find("CameraOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapCameraOptionUI.find("CameraOptionUI_ButtonMouseSensitive")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::CameraOptionUISensitiveMinus()
{
	CUIText* Text = (CUIText*)m_mapCameraOptionUI.find("CameraOptionUI_TextSensitive")->second.Get();

	if (wcscmp(L"0", Text->GetText()) == 0)
		return;

	// 수치 조절 후 해당 값을 Text에 SetText

	return;

	Text->SetText("");
}

void CPauseUI::CameraOptionUISensitivePlus()
{
	CUIText* Text = (CUIText*)m_mapCameraOptionUI.find("CameraOptionUI_TextSensitive")->second.Get();

	if (wcscmp(L"100", Text->GetText()) == 0)
		return;

	// 수치 조절 후 해당 값을 Text에 SetText

	return;



	Text->SetText("");
}

void CPauseUI::CameraOptionUIXReverse()
{
	m_CameraSelected = EUICameraList::XReverse;

	auto iterSplotch = m_mapCameraOptionUI.find("CameraOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapCameraOptionUI.find("CameraOptionUI_ButtonXReverse")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::CameraOptionUIXReverse_Normal()
{
	auto iterSplotch = m_mapCameraOptionUI.find("CameraOptionUI_XReverseSelectedSplotch");
	Vector2 vecPos = m_mapCameraOptionUI.find("CameraOptionUI_ButtonXReverseNormal")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::CameraOptionUIXReverse_Reverse()
{
	auto iterSplotch = m_mapCameraOptionUI.find("CameraOptionUI_XReverseSelectedSplotch");
	Vector2 vecPos = m_mapCameraOptionUI.find("CameraOptionUI_ButtonXReverseReverse")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::CameraOptionUIYReverse()
{
	m_CameraSelected = EUICameraList::YReverse;

	auto iterSplotch = m_mapCameraOptionUI.find("CameraOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapCameraOptionUI.find("CameraOptionUI_ButtonYReverse")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CPauseUI::CameraOptionUIYReverse_Normal()
{
	auto iterSplotch = m_mapCameraOptionUI.find("CameraOptionUI_YReverseSelectedSplotch");
	Vector2 vecPos = m_mapCameraOptionUI.find("CameraOptionUI_ButtonYReverseNormal")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);


}

void CPauseUI::CameraOptionUIYReverse_Reverse()
{
	auto iterSplotch = m_mapCameraOptionUI.find("CameraOptionUI_YReverseSelectedSplotch");
	Vector2 vecPos = m_mapCameraOptionUI.find("CameraOptionUI_ButtonYReverseReverse")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}
