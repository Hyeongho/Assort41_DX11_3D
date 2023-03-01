#include "TitleSceneUI.h"

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

#include "../Scene/MainSceneInfo.h"

CTitleSceneUI::CTitleSceneUI() :
	m_NowUIMode(EUIMode::Main),
	m_MainSelected(EMainList::NewGame),
	m_OptionSelected(EUIOptionList::Camera),
	m_SaveSelected(EUISaveList::First),
	m_SoundSelected(EUISoundList::SFX),
	m_CameraSelected(EUICameraList::End)
{
	m_WindowTypeName = "TitleSceneUI";
}

CTitleSceneUI::CTitleSceneUI(const CTitleSceneUI& Window) : 
	CUIWindow(Window),
	m_NowUIMode(EUIMode::Main),
	m_MainSelected(EMainList::NewGame),
	m_OptionSelected(EUIOptionList::Camera),
	m_SaveSelected(EUISaveList::First),
	m_SoundSelected(EUISoundList::SFX),
	m_CameraSelected(EUICameraList::End)
{
}

CTitleSceneUI::~CTitleSceneUI()
{
}

void CTitleSceneUI::Start()
{
	CUIWindow::Start();
	

	// KeySetting
	{
		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("UArrow", Input_Type::Up, this, &CTitleSceneUI::KeyUp, m_Scene);
		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("W", Input_Type::Up, this, &CTitleSceneUI::KeyUp, m_Scene);

		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("DArrow", Input_Type::Up, this, &CTitleSceneUI::KeyDown, m_Scene);
		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("S", Input_Type::Up, this, &CTitleSceneUI::KeyDown, m_Scene);

		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("LArrow", Input_Type::Up, this, &CTitleSceneUI::KeyLeft, m_Scene);
		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("A", Input_Type::Up, this, &CTitleSceneUI::KeyLeft, m_Scene);

		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("RArrow", Input_Type::Up, this, &CTitleSceneUI::KeyRight, m_Scene);
		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("D", Input_Type::Up, this, &CTitleSceneUI::KeyRight, m_Scene);

		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("Space", Input_Type::Up, this, &CTitleSceneUI::KeySpace, m_Scene);
		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("Esc", Input_Type::Up, this, &CTitleSceneUI::KeyBack, m_Scene);

		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("LButton", Input_Type::Up, this, &CTitleSceneUI::KeyLeftButton, m_Scene);
		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("RButton", Input_Type::Up, this, &CTitleSceneUI::KeyRightButton, m_Scene);


		CInput::GetInst()->AddBindFunction<CTitleSceneUI>("F1", Input_Type::Up, this, &CTitleSceneUI::KeyF12, m_Scene);
	}
}

bool CTitleSceneUI::Init()
{
	CUIWindow::Init();


	CreateBackgroundUI();
	CreateMainUI();
	CreateOptionUI();
	CreateOptionSoundUI();
	CreateOptionCameraUI();
	CreateSaveSelectUI();
	CreateControlUI();
	CreateCreditsUI();

	InActiveOptionUI();
	InActiveOptionSoundUI();
	InActiveOptionCameraUI();
	InActiveSaveSelectUI();
	InActiveControlUI();
	InActiveCreditsUI();


	return true;
}

void CTitleSceneUI::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}

void CTitleSceneUI::PostUpdate(float DeltaTime)
{
	CUIWindow::PostUpdate(DeltaTime);
}

void CTitleSceneUI::Render()
{
	CUIWindow::Render();
}

CTitleSceneUI* CTitleSceneUI::Clone()
{
	return new CTitleSceneUI(*this);
}

void CTitleSceneUI::Save(FILE* File)
{
	CUIWindow::Save(File);
}

void CTitleSceneUI::Load(FILE* File)
{
	CUIWindow::Load(File);
}

void CTitleSceneUI::CreateBackgroundUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	CUIImage* Image = CreateWidget<CUIImage>("BackUI_Background");

	Image->SetTexture("UIBackground", TEXT("UI/Title/SB_BfBB-TITLESCREEN-FULL.tga"));

	Image->SetSize((float)RS.Width, (float)RS.Height);
	Image->SetPos(0.f, 0.f);

	m_mapBackUI.insert(std::make_pair("BackUI_Background", Image));



	Image = CreateWidget<CUIImage>("BackUI_BackBoard");

	Image->SetTexture("UIBackBoard", TEXT("UI/Title/Startscreen_map.tga"));

	float BackboardWidth = RS.Width * 5.f / 12.f;

	Image->SetSize(BackboardWidth, (float)RS.Height);
	Image->SetPos(RS.Width / 2.f, 0.f);

	m_mapBackUI.insert(std::make_pair("BackUI_BackBoard", Image));



	Image = CreateWidget<CUIImage>("BackUI_Logo");

	Image->SetTexture("UILogo", TEXT("UI/Title/Logo/Logo_SBSP_BB_02.tga"));

	float LogoWidthStandard = BackboardWidth / 10.f;
	float LogoHeightStandard = RS.Height / 15.f;

	float LogoWidth = BackboardWidth - LogoWidthStandard * 2.f;
	float LogoHeight = LogoWidth / 2.2f;

	float LogoPosX = RS.Width / 2.f + LogoWidthStandard;
	float LogoPosY = RS.Height - LogoHeightStandard - LogoHeight;

	Image->SetSize(LogoWidth, LogoHeight);
	Image->SetPos(LogoPosX, LogoPosY);

	m_mapBackUI.insert(std::make_pair("BackUI_Logo", Image));



	Image = CreateWidget<CUIImage>("BackUI_SmallLogo");

	Image->SetTexture("UISmallLogo", TEXT("UI/Title/Logo/Korean_logo.tga"));

	float SmallLogoWidth = LogoWidth / 2.f;
	float SmallLogoHeight = SmallLogoWidth / 2.f;

	float SmallLogoPosY = RS.Height - LogoHeightStandard - SmallLogoHeight;

	Image->SetSize(SmallLogoWidth, SmallLogoHeight);
	Image->SetPos(LogoPosX, SmallLogoPosY);

	m_mapBackUI.insert(std::make_pair("BackUI_SmallLogo", Image));



	Image = CreateWidget<CUIImage>("BackUI_FlowersLeft");

	Image->SetTexture("FlowersLeft", TEXT("UI/Title/Startscreen_flowers_left.tga"));

	float LeftFlowerHeight = RS.Height / 2.f;
	float LeftFlowerWidth = LeftFlowerHeight * 1.7f;

	Image->SetSize(LeftFlowerWidth, LeftFlowerHeight);
	Image->SetPos(0.f, 0.f);

	m_mapBackUI.insert(std::make_pair("BackUI_FlowersLeft", Image));



	Image = CreateWidget<CUIImage>("BackUI_FlowersRight");

	Image->SetTexture("FlowersRight", TEXT("UI/Title/Startscreen_flowers_right.tga"));

	float RightFlowerHeight = LeftFlowerHeight / 1.3f;
	float RightFlowerWidth = RightFlowerHeight / 1.6f;

	Image->SetSize(RightFlowerWidth, RightFlowerHeight);
	Image->SetPos(RS.Width - RightFlowerWidth, 0.f);

	m_mapBackUI.insert(std::make_pair("BackUI_FlowersRight", Image));
}

void CTitleSceneUI::CreateMainUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2 BackBoardSize = m_mapBackUI.find("BackUI_BackBoard")->second->GetSize();
	Vector2 BackBoardPos = m_mapBackUI.find("BackUI_BackBoard")->second->GetPos();

	float ButtonSizeX = BackBoardSize.x / 2.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = BackBoardPos.x + BackBoardSize.x / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height / 2.f;

	float ButtonYInterval = ButtonSizeY + 10.f;


	// 아래 이미지를 이동시켜가며 선택된 메뉴를 시각화한댜ㅏ.
	CUIImage* Image = CreateWidget<CUIImage>("MainUI_SelectedSplotch");

	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));

	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPos(ButtonPosX - ButtonSizeX * 0.1f, ButtonPosY);
	Image->SetPivot(0.f, 0.3f);

	m_mapMainUI.insert(std::make_pair("MainUI_SelectedSplotch", Image));



	float FontSize = 30.f;

	CUITextButton* TextButton = CreateWidget<CUITextButton>("MainUI_ButtonContinue");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("계속"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("계속"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("계속"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "MainUI_ButtonContinueHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "MainUI_ButtonContinueClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::MainUIContinueHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::MainUIContinue);

	m_mapMainUI.insert(std::make_pair("MainUI_ButtonContinue", TextButton));


	ButtonPosY -= ButtonYInterval;



	TextButton = CreateWidget<CUITextButton>("MainUI_ButtonNewGame");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("새 게임"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("새 게임"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("새 게임"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "MainUI_ButtonNewGameHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "MainUI_ButtonNewGameClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::MainUINewGameHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::MainUINewGame);

	m_mapMainUI.insert(std::make_pair("MainUI_ButtonNewGame", TextButton));

	ButtonPosY -= ButtonYInterval;



	TextButton = CreateWidget<CUITextButton>("MainUI_ButtonOption");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("옵션"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("옵션"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("옵션"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "MainUI_ButtonOptionHovered", false, "Sfx/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "MainUI_ButtonOptionClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::MainUIOptionHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::MainUIOption);

	m_mapMainUI.insert(std::make_pair("MainUI_ButtonOption", TextButton));

	ButtonPosY -= ButtonYInterval;



	TextButton = CreateWidget<CUITextButton>("MainUI_ButtonControl");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("조작법"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("조작법"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("조작법"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "MainUI_ButtonControlHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "MainUI_ButtonControlClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::MainUIControlHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::MainUIControl);

	m_mapMainUI.insert(std::make_pair("MainUI_ButtonControl", TextButton));

	ButtonPosY -= ButtonYInterval;



	TextButton = CreateWidget<CUITextButton>("MainUI_ButtonCredits");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("제작진"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("제작진"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("제작진"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "MainUI_ButtonCreditsHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "MainUI_ButtonCreditsClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::MainUICreditsHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::MainUICredits);

	m_mapMainUI.insert(std::make_pair("MainUI_ButtonCredits", TextButton));

	ButtonPosY -= ButtonYInterval;




	TextButton = CreateWidget<CUITextButton>("MainUI_ButtonQuit");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("종료"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("종료"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("종료"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "MainUI_ButtonQuitHovered", false, "Sfx/SFX_UI_Scroll_003.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "MainUI_ButtonQuitClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::MainUIQuitHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::MainUIQuit);

	m_mapMainUI.insert(std::make_pair("MainUI_ButtonQuit", TextButton));

	ButtonPosY -= ButtonYInterval;




	// 하단 가이드
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;

	
	float GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f - GuideSizeX * 3.f / 2.f;
	float GuidePosY = 10.f;


	Image = CreateWidget<CUIImage>("MainUI_ImageGuideForward");

	Image->SetTexture("KeyGuideMouseLeft", TEXT("UI/Key/Mouse_Left.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapMainUI.insert(std::make_pair("MainUI_ImageGuideForward", Image));



	CUIText* Text = CreateWidget<CUIText>("MainUI_TextGuideForward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("선택"));
	Text->SetColor(Vector4::Black);

	m_mapMainUI.insert(std::make_pair("MainUI_TextGuideForward", Text));
}

void CTitleSceneUI::CreateOptionUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2 BackBoardSize = m_mapBackUI.find("BackUI_BackBoard")->second->GetSize();
	Vector2 BackBoardPos = m_mapBackUI.find("BackUI_BackBoard")->second->GetPos();

	float ButtonSizeX = BackBoardSize.x / 2.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = BackBoardPos.x + BackBoardSize.x / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height / 2.f;

	float ButtonYInterval = ButtonSizeY + 10.f;


	// 아래 이미지를 이동시켜가며 선택된 메뉴를 시각화한댜ㅏ.
	CUIImage* Image = CreateWidget<CUIImage>("OptionUI_SelectedSplotch");

	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));

	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPos(ButtonPosX - ButtonSizeX * 0.1f, ButtonPosY);
	Image->SetPivot(0.f, 0.3f);

	m_mapOptionUI.insert(std::make_pair("OptionUI_SelectedSplotch", Image));



	CUIText* Text = CreateWidget<CUIText>("OptionUI_Text");

	Text->SetSize(ButtonSizeX, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("옵션"));
	Text->SetColor(Vector4::Black);

	m_mapOptionUI.insert(std::make_pair("OptionUI_Text", Text));

	ButtonPosY -= ButtonYInterval;






	float FontSize = 35.f;

	CUITextButton* TextButton = CreateWidget<CUITextButton>("OptionUI_ButtonSound");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("오디오 설정"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("오디오 설정"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("오디오 설정"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "OptionUI_ButtonSoundHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "OptionUI_ButtonSoundClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::OptionUISoundHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::OptionUISound);

	m_mapOptionUI.insert(std::make_pair("OptionUI_ButtonSound", TextButton));


	ButtonPosY -= ButtonYInterval;



	TextButton = CreateWidget<CUITextButton>("OptionUI_ButtonCamera");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("카메라 설정"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("카메라 설정"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("카메라 설정"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "OptionUI_ButtonCameraHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "OptionUI_ButtonCameraClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::OptionUICameraHovered);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::OptionUICamera);


	m_mapOptionUI.insert(std::make_pair("OptionUI_ButtonCamera", TextButton));




	// 하단 가이드 이미지
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;


	float GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f - GuideSizeX * 3.f;
	float GuidePosY = 10.f;

	Image = CreateWidget<CUIImage>("OptionUI_ImageGuideBackward");

	Image->SetTexture("KeyGuideMouseRight", TEXT("UI/Key/Mouse_Right.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapOptionUI.insert(std::make_pair("OptionUI_ImageGuideBackward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideBackward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("뒤로"));
	Text->SetColor(Vector4::Black);

	m_mapOptionUI.insert(std::make_pair("OptionUI_TextGuideBackward", Text));


	GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f + GuideSizeX / 2.f;

	Image = CreateWidget<CUIImage>("OptionUI_ImageGuideForward");

	Image->SetTexture("KeyGuideMouseLeft", TEXT("UI/Key/Mouse_Left.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapOptionUI.insert(std::make_pair("OptionUI_ImageGuideForward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideForward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("선택"));
	Text->SetColor(Vector4::Black);

	m_mapOptionUI.insert(std::make_pair("OptionUI_TextGuideForward", Text));
}

void CTitleSceneUI::CreateOptionSoundUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2 BackBoardSize = m_mapBackUI.find("BackUI_BackBoard")->second->GetSize();
	Vector2 BackBoardPos = m_mapBackUI.find("BackUI_BackBoard")->second->GetPos();

	float ButtonSizeX = BackBoardSize.x / 2.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = BackBoardPos.x + BackBoardSize.x / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height / 2.f;

	float ButtonYInterval = ButtonSizeY + 10.f;


	// 아래 이미지를 이동시켜가며 선택된 메뉴를 시각화한다.
	CUIImage* Image = CreateWidget<CUIImage>("SoundOptionUI_SelectedSplotch");

	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));

	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPos(ButtonPosX - ButtonSizeX * 0.1f, ButtonPosY);
	Image->SetPivot(0.f, 0.3f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_SelectedSplotch", Image));

	

	CUIText* Text = CreateWidget<CUIText>("SoundOptionUI_Text");

	Text->SetSize(ButtonSizeX, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("오디오 설정"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_Text", Text));

	ButtonPosY -= ButtonYInterval;



	float FontSize = 30.f;

	// SFX
	CUITextButton* TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonSfxOption");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX - ButtonSizeX / 2.f, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("SFX 볼륨"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("SFX 볼륨"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("SFX 볼륨"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonSfxOptionHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonSfxOptionClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::SoundOptionUISFX);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonSfxOption", TextButton));


	Text = CreateWidget<CUIText>("SoundOptionUI_TextSfxMinus");

	Text->SetSize(ButtonSizeY, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX + 100.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("-"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_TextSfxMinus", Text));


	Image = CreateWidget<CUIImage>("SoundOptionUI_ImageSfxSlideBar");

	Image->SetTexture("SlideBar", TEXT("UI/Option/slider_bar_2.tga"));
	Image->SetSize(140.f, 10.f);
	Image->SetPos(ButtonPosX + 140.f, ButtonPosY + 20.f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ImageSfxSlideBar", Image));;


	Image = CreateWidget<CUIImage>("SoundOptionUI_ImageSfxCursor");

	Image->SetTexture("SliderCursor", TEXT("UI/Option/slider_bar.tga"));
	Image->SetSize(30.f, 10.f);
	Image->SetAngle(90.f);
	Image->SetImageTint(Vector4(146.f / 255.f, 123.f / 255.f, 70.f / 255.f, 1.f));

	float SfxSliderPosX = 0.f;

	// 사운드 크기를 찾아와서, 슬라이더의 위치에 배정
	// ButtonPosX(0) ~ sliderbarPos+sliderbarSize(100)
	SfxSliderPosX = ButtonPosX + 140.f + 140.f;

	Image->SetPos(SfxSliderPosX, ButtonPosY + 10.f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ImageSfxCursor", Image));



	Text = CreateWidget<CUIText>("SoundOptionUI_TextSfxPlus");

	Text->SetSize(ButtonSizeY, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(RS.Width / 2.f + BackBoardSize.x - 100.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("+"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_TextSfxPlus", Text));


	ButtonPosY -= ButtonYInterval;



	// Music
	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonMusicOption");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX - ButtonSizeX / 2.f, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("Music 볼륨"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("Music 볼륨"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("Music 볼륨"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonMusicOptionHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonMusicOptionClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::SoundOptionUIMusic);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonMusicOption", TextButton));


	Text = CreateWidget<CUIText>("SoundOptionUI_TextMusicMinus");

	Text->SetSize(ButtonSizeY, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX + 100.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("-"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_TextMusicMinus", Text));


	Image = CreateWidget<CUIImage>("SoundOptionUI_ImageMusicSlideBar");

	Image->SetTexture("SlideBar", TEXT("UI/Option/slider_bar_2.tga"));
	Image->SetSize(140.f, 10.f);
	Image->SetPos(ButtonPosX + 140.f, ButtonPosY + 20.f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ImageMusicSlideBar", Image));;


	Image = CreateWidget<CUIImage>("SoundOptionUI_ImageMusicCursor");

	Image->SetTexture("SliderCursor", TEXT("UI/Option/slider_bar.tga"));
	Image->SetSize(30.f, 10.f);
	Image->SetAngle(90.f);
	Image->SetImageTint(Vector4(146.f / 255.f, 123.f / 255.f, 70.f / 255.f, 1.f));

	float MusicSliderPosX = 0.f;

	// 사운드 크기를 찾아와서, 슬라이더의 위치에 배정
	// ButtonPosX(0) ~ sliderbarPos+sliderbarSize(100)
	MusicSliderPosX = ButtonPosX + 140.f + 140.f;

	Image->SetPos(MusicSliderPosX, ButtonPosY + 10.f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ImageMusicCursor", Image));


	Text = CreateWidget<CUIText>("SoundOptionUI_TextMusicPlus");

	Text->SetSize(ButtonSizeY, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(RS.Width / 2.f + BackBoardSize.x - 100.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("+"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_TextMusicPlus", Text));

	ButtonPosY -= ButtonYInterval;


	// Talk
	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonTalkOption");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX - ButtonSizeX / 2.f, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("Talk 볼륨"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("Talk 볼륨"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("Talk 볼륨"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonTalkOptionHovered", false, "Sfx/SFX_UI_Scroll_002.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonTalkOptionClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::SoundOptionUITalk);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonTalkOption", TextButton));


	Text = CreateWidget<CUIText>("SoundOptionUI_TextTalkMinus");

	Text->SetSize(ButtonSizeY, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX + 100.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("-"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_TextTalkMinus", Text));


	Image = CreateWidget<CUIImage>("SoundOptionUI_ImageTalkSlideBar");

	Image->SetTexture("SlideBar", TEXT("UI/Option/slider_bar_2.tga"));
	Image->SetSize(140.f, 10.f);
	Image->SetPos(ButtonPosX + 140.f, ButtonPosY + 20.f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ImageTalkSlideBar", Image));;


	Image = CreateWidget<CUIImage>("SoundOptionUI_ImageTalkCursor");

	Image->SetTexture("SliderCursor", TEXT("UI/Option/slider_bar.tga"));
	Image->SetSize(30.f, 10.f);
	Image->SetAngle(90.f);
	Image->SetImageTint(Vector4(146.f / 255.f, 123.f / 255.f, 70.f / 255.f, 1.f));

	float TalkSliderPosX = 0.f;

	// 사운드 크기를 찾아와서, 슬라이더의 위치에 배정
	// ButtonPosX(0) ~ sliderbarPos+sliderbarSize(100)
	TalkSliderPosX = ButtonPosX + 140.f + 140.f;

	Image->SetPos(TalkSliderPosX, ButtonPosY + 10.f);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ImageTalkCursor", Image));


	Text = CreateWidget<CUIText>("SoundOptionUI_TextTalkPlus");

	Text->SetSize(ButtonSizeY, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(RS.Width / 2.f + BackBoardSize.x - 100.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("+"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_TextTalkPlus", Text));

	ButtonPosY -= ButtonYInterval;



	TextButton = CreateWidget<CUITextButton>("SoundOptionUI_ButtonReset");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);


	TextButton->SetText(EButtonState::Normal, TEXT("볼륨 설정 초기화"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("볼륨 설정 초기화"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("볼륨 설정 초기화"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SoundOptionUI_ButtonResetHovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SoundOptionUI_ButtonResetClick", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::SoundOptionUIReset);

	m_mapSoundOptionUI.insert(std::make_pair("SoundOptionUI_ButtonReset", TextButton));



	// 하단 가이드 이미지
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;


	float GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f - GuideSizeX * 3.f;
	float GuidePosY = 10.f;

	Image = CreateWidget<CUIImage>("OptionUI_ImageGuideBackward");

	Image->SetTexture("KeyGuideMouseRight", TEXT("UI/Key/Mouse_Right.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapSoundOptionUI.insert(std::make_pair("OptionUI_ImageGuideBackward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideBackward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("뒤로"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("OptionUI_TextGuideBackward", Text));


	GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f + GuideSizeX / 2.f;

	Image = CreateWidget<CUIImage>("OptionUI_ImageGuideForward");

	Image->SetTexture("KeyGuideMouseLeft", TEXT("UI/Key/Mouse_Left.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapSoundOptionUI.insert(std::make_pair("OptionUI_ImageGuideForward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideForward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("선택"));
	Text->SetColor(Vector4::Black);

	m_mapSoundOptionUI.insert(std::make_pair("OptionUI_TextGuideForward", Text));
}

void CTitleSceneUI::CreateOptionCameraUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2 BackBoardSize = m_mapBackUI.find("BackUI_BackBoard")->second->GetSize();
	Vector2 BackBoardPos = m_mapBackUI.find("BackUI_BackBoard")->second->GetPos();

	float ButtonSizeX = BackBoardSize.x / 2.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = BackBoardPos.x + BackBoardSize.x / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height / 2.f;

	float ButtonYInterval = ButtonSizeY + 10.f;


	// 아래 이미지를 이동시켜가며 선택된 메뉴를 시각화한다.
	CUIImage* Image = CreateWidget<CUIImage>("CameraOptionUI_SelectedSplotch");

	Image->SetTexture("SelectBackSplotch", TEXT("UI/Title/Color_splotch_2.tga"));

	Image->SetSize(ButtonSizeX * 1.2f, ButtonSizeY * 1.4f);
	Image->SetPos(ButtonPosX - ButtonSizeX * 0.1f, ButtonPosY);
	Image->SetPivot(0.f, 0.3f);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_SelectedSplotch", Image));



	CUIText* Text = CreateWidget<CUIText>("CameraOptionUI_Text");

	Text->SetSize(ButtonSizeX, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("카메라 설정"));
	Text->SetColor(Vector4::Black);

	m_mapCameraOptionUI.insert(std::make_pair("CameraOptionUI_Text", Text));

	ButtonPosY -= ButtonYInterval;




	// 하단 가이드 이미지
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;


	float GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f - GuideSizeX * 3.f;
	float GuidePosY = 10.f;

	Image = CreateWidget<CUIImage>("OptionUI_ImageGuideBackward");

	Image->SetTexture("KeyGuideMouseRight", TEXT("UI/Key/Mouse_Right.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapCameraOptionUI.insert(std::make_pair("OptionUI_ImageGuideBackward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideBackward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("뒤로"));
	Text->SetColor(Vector4::Black);

	m_mapCameraOptionUI.insert(std::make_pair("OptionUI_TextGuideBackward", Text));


	GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f + GuideSizeX / 2.f;

	Image = CreateWidget<CUIImage>("OptionUI_ImageGuideForward");

	Image->SetTexture("KeyGuideMouseLeft", TEXT("UI/Key/Mouse_Left.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapCameraOptionUI.insert(std::make_pair("OptionUI_ImageGuideForward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideForward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("선택"));
	Text->SetColor(Vector4::Black);

	m_mapCameraOptionUI.insert(std::make_pair("OptionUI_TextGuideForward", Text));
}

void CTitleSceneUI::CreateSaveSelectUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2 BackBoardSize = m_mapBackUI.find("BackUI_BackBoard")->second->GetSize();
	Vector2 BackBoardPos = m_mapBackUI.find("BackUI_BackBoard")->second->GetPos();

	float ButtonSizeX = BackBoardSize.x / 2.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = BackBoardPos.x + BackBoardSize.x / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height / 2.f;

	float ButtonYInterval = ButtonSizeY + 30.f;


	CUIText* Text = CreateWidget<CUIText>("SaveSelectUI_Text");

	Text->SetSize(ButtonSizeX * 2.f, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX - ButtonSizeX / 2.f, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("슬롯을 선택하여"));
	Text->SetColor(Vector4::Black);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_Text", Text));

	ButtonPosY -= ButtonYInterval;



	float FontSize = 30.f;

	CUITextButton* TextButton = CreateWidget<CUITextButton>("SaveSelectUI_ButtonSaveSLot1");

	TextButton->SetSize(ButtonSizeX, ButtonSizeY);
	TextButton->SetPos(ButtonPosX, ButtonPosY);

	TextButton->SetText(EButtonState::Normal, TEXT("빈 슬롯"), FontSize, Vector4::Yellow);
	TextButton->SetText(EButtonState::Hovered, TEXT("빈 슬롯"), FontSize, Vector4::White);
	TextButton->SetText(EButtonState::Click, TEXT("빈 슬롯"), FontSize, Vector4::White);

	TextButton->SetSound(EButtonEventState::Hovered, "UI", "SaveSelectUI_ButtonSaveSLot1Hovered", false, "Sfx/SFX_UI_Scroll_001.ogg");
	TextButton->SetSound(EButtonEventState::Click, "UI", "SaveSelectUI_ButtonSaveSLot1Click", false, "Sfx/SFX_UI_Forward.ogg");

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::SaveSelectUISaveSlot1Hoverd);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::GameStart);

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

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::SaveSelectUISaveSlot2Hoverd);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::GameStart);

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

	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Hovered, this, &CTitleSceneUI::SaveSelectUISaveSlot3Hoverd);
	TextButton->SetCallback<CTitleSceneUI>(EButtonEventState::Click, this, &CTitleSceneUI::GameStart);

	m_mapSaveSelectUI.insert(std::make_pair("SaveSelectUI_ButtonSaveSLot3", TextButton));

	ButtonPosY -= ButtonYInterval;





	// 하단 가이드 이미지
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;


	float GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f - GuideSizeX * 3.f;
	float GuidePosY = 10.f;

	CUIImage* Image = CreateWidget<CUIImage>("OptionUI_ImageGuideBackward");

	Image->SetTexture("KeyGuideMouseRight", TEXT("UI/Key/Mouse_Right.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapSaveSelectUI.insert(std::make_pair("OptionUI_ImageGuideBackward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideBackward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("뒤로"));
	Text->SetColor(Vector4::Black);

	m_mapSaveSelectUI.insert(std::make_pair("OptionUI_TextGuideBackward", Text));


	GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f + GuideSizeX / 2.f;

	Image = CreateWidget<CUIImage>("OptionUI_ImageGuideForward");

	Image->SetTexture("KeyGuideMouseLeft", TEXT("UI/Key/Mouse_Left.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapSaveSelectUI.insert(std::make_pair("OptionUI_ImageGuideForward", Image));



	Text = CreateWidget<CUIText>("OptionUI_TextGuideForward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("선택"));
	Text->SetColor(Vector4::Black);

	m_mapSaveSelectUI.insert(std::make_pair("OptionUI_TextGuideForward", Text));
}

void CTitleSceneUI::CreateControlUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2 BackBoardSize = m_mapBackUI.find("BackUI_BackBoard")->second->GetSize();
	Vector2 BackBoardPos = m_mapBackUI.find("BackUI_BackBoard")->second->GetPos();

	float ButtonSizeX = BackBoardSize.x / 2.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = BackBoardPos.x + BackBoardSize.x / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height / 2.f;

	float ButtonYInterval = ButtonSizeY + 10.f;


	CUIText* Text = CreateWidget<CUIText>("ControlUI_Text");

	Text->SetSize(ButtonSizeX, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("조작법"));
	Text->SetColor(Vector4::Black);

	m_mapControlUI.insert(std::make_pair("ControlUI_Text", Text));




	// 하단 가이드
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;


	float GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f - GuideSizeX * 3.f / 2.f;
	float GuidePosY = 10.f;


	CUIImage* Image = CreateWidget<CUIImage>("ControlUI_ImageGuideBackward");

	Image->SetTexture("KeyGuideMouseRight", TEXT("UI/Key/Mouse_Right.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapControlUI.insert(std::make_pair("ControlUI_ImageGuideBackward", Image));



	Text = CreateWidget<CUIText>("ControlUI_TextGuideBackward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("뒤로"));
	Text->SetColor(Vector4::Black);

	m_mapControlUI.insert(std::make_pair("ControlUI_TextGuideBackward", Text));
}

void CTitleSceneUI::CreateCreditsUI()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();

	Vector2 BackBoardSize = m_mapBackUI.find("BackUI_BackBoard")->second->GetSize();
	Vector2 BackBoardPos = m_mapBackUI.find("BackUI_BackBoard")->second->GetPos();

	float ButtonSizeX = BackBoardSize.x / 2.f;
	float ButtonSizeY = 50.f;

	float ButtonPosX = BackBoardPos.x + BackBoardSize.x / 2.f - ButtonSizeX / 2.f;
	float ButtonPosY = RS.Height / 2.f;

	float ButtonYInterval = ButtonSizeY + 10.f;


	CUIText* Text = CreateWidget<CUIText>("CreditsUI_Text");

	Text->SetSize(ButtonSizeX, ButtonSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(ButtonPosX, ButtonPosY);
	Text->SetFontSize(30.f);
	Text->SetText(TEXT("제작진"));
	Text->SetColor(Vector4::Black);

	m_mapCreditsUI.insert(std::make_pair("CreditsUI_Text", Text));




	// 하단 가이드
	float GuideSizeX = 25.f;
	float GuideSizeY = 25.f;


	float GuidePosX = BackBoardPos.x + BackBoardSize.x / 2.f - GuideSizeX * 3.f / 2.f;
	float GuidePosY = 10.f;


	CUIImage* Image = CreateWidget<CUIImage>("CreditsUI_ImageGuideBackward");

	Image->SetTexture("KeyGuideMouseRight", TEXT("UI/Key/Mouse_Right.tga"));

	Image->SetSize(GuideSizeX * 2.f / 3.f, GuideSizeY);
	Image->SetPos(GuidePosX, GuidePosY);

	m_mapCreditsUI.insert(std::make_pair("CreditsUI_ImageGuideBackward", Image));



	Text = CreateWidget<CUIText>("CreditsUI_TextGuideBackward");

	Text->SetSize(GuideSizeX * 2.f, GuideSizeY);
	Text->SetAlignH(Text_Align_H::Center);
	Text->SetPos(GuidePosX + 25.f, GuidePosY);
	Text->SetFontSize(20.f);
	Text->SetText(TEXT("뒤로"));
	Text->SetColor(Vector4::Black);

	m_mapCreditsUI.insert(std::make_pair("CreditsUI_TextGuideBackward", Text));
}

void CTitleSceneUI::ActiveMainUI()
{
	for (auto iter : m_mapMainUI)
		iter.second->SetEnable(true);
}

void CTitleSceneUI::ActiveOptionUI()
{
	for (auto iter : m_mapOptionUI)
		iter.second->SetEnable(true);
}

void CTitleSceneUI::ActiveOptionSoundUI()
{
	for (auto iter : m_mapSoundOptionUI)
		iter.second->SetEnable(true);
}

void CTitleSceneUI::ActiveOptionCameraUI()
{
	for (auto iter : m_mapCameraOptionUI)
		iter.second->SetEnable(true);
}

void CTitleSceneUI::ActiveSaveSelectUI()
{
	for (auto iter : m_mapSaveSelectUI)
		iter.second->SetEnable(true);
}

void CTitleSceneUI::ActiveControlUI()
{
	for (auto iter : m_mapControlUI)
		iter.second->SetEnable(true);
}

void CTitleSceneUI::ActiveCreditsUI()
{
	for (auto iter : m_mapCreditsUI)
		iter.second->SetEnable(true);
}

void CTitleSceneUI::InActiveMainUI()
{
	for (auto iter : m_mapMainUI)
		iter.second->SetEnable(false);
}

void CTitleSceneUI::InActiveOptionUI()
{
	for (auto iter : m_mapOptionUI)
		iter.second->SetEnable(false);
}

void CTitleSceneUI::InActiveOptionSoundUI()
{
	for (auto iter : m_mapSoundOptionUI)
		iter.second->SetEnable(false);
}

void CTitleSceneUI::InActiveOptionCameraUI()
{
	for (auto iter : m_mapCameraOptionUI)
		iter.second->SetEnable(false);
}

void CTitleSceneUI::InActiveSaveSelectUI()
{
	for (auto iter : m_mapSaveSelectUI)
		iter.second->SetEnable(false);
}

void CTitleSceneUI::InActiveControlUI()
{
	for (auto iter : m_mapControlUI)
		iter.second->SetEnable(false);
}

void CTitleSceneUI::InActiveCreditsUI()
{
	for (auto iter : m_mapCreditsUI)
		iter.second->SetEnable(false);
}

void CTitleSceneUI::KeyLeftButton()
{
}

void CTitleSceneUI::KeyRightButton()
{
	CSound* Sound = m_Scene->GetResource()->FindSound("UI_Backward");

	if (Sound) {
		Sound->Play();
	}


	if (m_NowUIMode == EUIMode::OptionSound)
	{
		m_NowUIMode = EUIMode::Option;

		InActiveOptionSoundUI();
		ActiveOptionUI();
	}
	else if (m_NowUIMode == EUIMode::OptionCamera) 
	{
		m_NowUIMode = EUIMode::Option;

		InActiveOptionCameraUI();
		ActiveOptionUI();
	}
	else {
		if (m_NowUIMode != EUIMode::Main) {
			switch (m_NowUIMode)
			{
			case EUIMode::Continue:
			case EUIMode::NewGame:
				InActiveSaveSelectUI();
				break;
			case EUIMode::Control:
				InActiveControlUI();
				break;
			case EUIMode::Credit:
				InActiveCreditsUI();
				break;
			case EUIMode::Option:
				InActiveOptionUI();
				break;
			}
			m_NowUIMode = EUIMode::Main;
			ActiveMainUI();

		}
	}
}

void CTitleSceneUI::KeyUp()
{
	switch (m_NowUIMode)
	{
	case EUIMode::Main:
		switch (m_MainSelected)
		{
		case EMainList::Continue:
			m_MainSelected = EMainList::End;
			break;
		case EMainList::NewGame:
			m_MainSelected = EMainList::Continue;
			break;
		case EMainList::Option:
			m_MainSelected = EMainList::NewGame;
			break;
		case EMainList::Control:
			m_MainSelected = EMainList::Option;
			break;
		case EMainList::Credits:
			m_MainSelected = EMainList::Control;
			break;
		case EMainList::Quit:
			m_MainSelected = EMainList::Credits;
			break;
		}

		break;
	case EUIMode::Continue:
	case EUIMode::NewGame:
		switch (m_SaveSelected)
		{
		case EUISaveList::First:
			m_SaveSelected = EUISaveList::Third;
			break;
		case EUISaveList::Second:
			m_SaveSelected = EUISaveList::First;
			break;
		case EUISaveList::Third:
			m_SaveSelected = EUISaveList::Second;
			break;
		}

		break;
	case EUIMode::Option:
		switch (m_OptionSelected)
		{
		case EUIOptionList::Sound:
			m_OptionSelected = EUIOptionList::Camera;
			break;
		case EUIOptionList::Camera:
			m_OptionSelected = EUIOptionList::Sound;
			break;
		}

		break;
	case EUIMode::OptionSound:
		switch (m_SoundSelected)
		{
		case EUISoundList::SFX:
			m_SoundSelected = EUISoundList::Reset;
			break;
		case EUISoundList::Music:
			m_SoundSelected = EUISoundList::SFX;
			break;
		case EUISoundList::Reset:
			m_SoundSelected = EUISoundList::Music;
			break;
		}

		break;
	case EUIMode::OptionCamera:
		break;
	}
}

void CTitleSceneUI::KeyDown()
{
	switch (m_NowUIMode)
	{
	case EUIMode::Main:
		switch (m_MainSelected)
		{
		case EMainList::Continue:
			m_MainSelected = EMainList::NewGame;
			break;
		case EMainList::NewGame:
			m_MainSelected = EMainList::Option;
			break;
		case EMainList::Option:
			m_MainSelected = EMainList::Control;
			break;
		case EMainList::Control:
			m_MainSelected = EMainList::Credits;
			break;
		case EMainList::Credits:
			m_MainSelected = EMainList::Quit;
			break;
		case EMainList::Quit:
			m_MainSelected = EMainList::Continue;
			break;
		}

		break;
	case EUIMode::Continue:
	case EUIMode::NewGame:
		switch (m_SaveSelected)
		{
		case EUISaveList::First:
			m_SaveSelected = EUISaveList::Second;
			break;
		case EUISaveList::Second:
			m_SaveSelected = EUISaveList::Third;
			break;
		case EUISaveList::Third:
			m_SaveSelected = EUISaveList::First;
			break;
		}

		break;
	case EUIMode::Option:
		switch (m_OptionSelected)
		{
		case EUIOptionList::Sound:
			m_OptionSelected = EUIOptionList::Camera;
			break;
		case EUIOptionList::Camera:
			m_OptionSelected = EUIOptionList::Sound;
			break;
		}

		break;
	case EUIMode::OptionSound:
		switch (m_SoundSelected)
		{
		case EUISoundList::SFX:
			m_SoundSelected = EUISoundList::Music;
			break;
		case EUISoundList::Music:
			m_SoundSelected = EUISoundList::Reset;
			break;
		case EUISoundList::Reset:
			m_SoundSelected = EUISoundList::SFX;
			break;
		}

		break;
	case EUIMode::OptionCamera:
		break;
	}
}

void CTitleSceneUI::KeyLeft()
{
	switch (m_NowUIMode)
	{
	case EUIMode::OptionSound:
		break;
	case EUIMode::OptionCamera:
		break;
	}
}

void CTitleSceneUI::KeyRight()
{
	switch (m_NowUIMode)
	{
	case EUIMode::OptionSound:
		break;
	case EUIMode::OptionCamera:
		break;
	}
}

void CTitleSceneUI::KeySpace()
{
	switch (m_NowUIMode)
	{
	case EUIMode::Main:
		switch (m_MainSelected)
		{
		case EMainList::Continue:
			// 세이브 선택 UI 활성화
			MainUIContinue();

			break;
		case EMainList::NewGame:
			// 세이브 선택 UI 활성화
			MainUINewGame();

			break;
		case EMainList::Option:
			// 옵션 선택 UI 활성화
			MainUIOption();

			break;
		case EMainList::Control:
			// 조작 UI 활성화
			MainUIControl();

			break;
		case EMainList::Credits:
			// 크레딧 UI 활성화
			MainUICredits();

			break;
		case EMainList::Quit:
			// 게임 종료
			MainUIQuit();

			break;
		}

		break;
	case EUIMode::Continue:
	case EUIMode::NewGame:
		switch (m_SaveSelected)
		{
		case EUISaveList::First:
			break;
		case EUISaveList::Second:
			break;
		case EUISaveList::Third:
			break;
		}

		break;
	case EUIMode::Option:
		switch (m_OptionSelected)
		{
		case EUIOptionList::Sound:
			m_NowUIMode = EUIMode::OptionSound;
			break;
		case EUIOptionList::Camera:
			m_NowUIMode = EUIMode::OptionCamera;
			break;
		}

		break;
	case EUIMode::OptionSound:
		switch (m_SoundSelected)
		{
		case EUISoundList::SFX:
			break;
		case EUISoundList::Music:
			break;
		case EUISoundList::Reset:
			break;
		}

		break;
	case EUIMode::OptionCamera:
		break;
	}
}

void CTitleSceneUI::KeyBack()
{
	switch (m_NowUIMode)
	{
	case EUIMode::Continue:
	case EUIMode::NewGame:
	case EUIMode::Option:
	case EUIMode::Control:
	case EUIMode::Credit:
		m_NowUIMode = EUIMode::Main;

		// UI 비활성화

		// 메인 UI 활성화

		break;
	case EUIMode::OptionSound:
	case EUIMode::OptionCamera:
		m_NowUIMode = EUIMode::Option;

		// 옵션 상세 UI 비활성화

		// 옵션 선택 UI 활성화

		break;
	}
}

void CTitleSceneUI::KeyF12()
{
	Vector2 test = CInput::GetInst()->GetMousePos();

	std::string OutputDebug = "Mouse : " + std::to_string(test.x) + ", " + std::to_string(test.y) + "\n";

	OutputDebugStringA(OutputDebug.c_str());
}

void CTitleSceneUI::MainUIContinue()
{
	m_NowUIMode = EUIMode::Continue;

	// 이어하기 UI 활성화
	InActiveMainUI();
	ActiveSaveSelectUI();

	CUIText* Text = (CUIText*)m_mapSaveSelectUI.find("SaveSelectUI_Text")->second.Get();
	Text->SetText(TEXT("슬롯을 선택하여 불러오기"));
}

void CTitleSceneUI::MainUINewGame()
{
	m_NowUIMode = EUIMode::NewGame;

	// 새 게임 UI 활성화
	InActiveMainUI();
	ActiveSaveSelectUI();

	CUIText* Text = (CUIText*)m_mapSaveSelectUI.find("SaveSelectUI_Text")->second.Get();
	Text->SetText(TEXT("슬롯을 선택하여 시작하기"));
}

void CTitleSceneUI::MainUIOption()
{
	m_NowUIMode = EUIMode::Option;

	// 옵션 UI 활성화
	InActiveMainUI();
	ActiveOptionUI();
}

void CTitleSceneUI::MainUIControl()
{
	m_NowUIMode = EUIMode::Control;

	// 컨트롤 UI 활성화
	InActiveMainUI();
	ActiveControlUI();
}

void CTitleSceneUI::MainUICredits()
{
	m_NowUIMode = EUIMode::Credit;

	// 크레딧 실행
	InActiveMainUI();
	ActiveCreditsUI();
}

void CTitleSceneUI::MainUIQuit()
{
	// 게임 종료 처리.

}

void CTitleSceneUI::MainUIContinueHovered()
{
	m_MainSelected = EMainList::Continue;

	auto iterSplotch = m_mapMainUI.find("MainUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapMainUI.find("MainUI_ButtonContinue")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::MainUINewGameHovered()
{
	m_MainSelected = EMainList::NewGame;

	auto iterSplotch = m_mapMainUI.find("MainUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapMainUI.find("MainUI_ButtonNewGame")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::MainUIOptionHovered()
{
	m_MainSelected = EMainList::Option;

	auto iterSplotch = m_mapMainUI.find("MainUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapMainUI.find("MainUI_ButtonOption")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::MainUIControlHovered()
{
	m_MainSelected = EMainList::Control;

	auto iterSplotch = m_mapMainUI.find("MainUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapMainUI.find("MainUI_ButtonControl")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::MainUICreditsHovered()
{
	m_MainSelected = EMainList::Credits;

	auto iterSplotch = m_mapMainUI.find("MainUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapMainUI.find("MainUI_ButtonCredits")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::MainUIQuitHovered()
{
	m_MainSelected = EMainList::Quit;

	auto iterSplotch = m_mapMainUI.find("MainUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapMainUI.find("MainUI_ButtonQuit")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::SaveSelectUISaveSlot1Hoverd()
{
	m_SaveSelected = EUISaveList::First;

}

void CTitleSceneUI::SaveSelectUISaveSlot2Hoverd()
{
	m_SaveSelected = EUISaveList::Second;

}

void CTitleSceneUI::SaveSelectUISaveSlot3Hoverd()
{
	m_SaveSelected = EUISaveList::Third;

}

void CTitleSceneUI::GameStart()
{
	// 현재 선택된 슬롯에 세이브 데이터가 있는지 체크.
	// 세이브 데이터가 있다면 삭제하고 시작할지 묻는다. 없다면 게임 시작한다.

	// 현재는 임의로 씬 변경하여, 메인 씬 실행하도록 작업

	if (m_NowUIMode == EUIMode::Continue)
	{
		// 이어하기 모드 상태
	}
	else if (m_NowUIMode == EUIMode::NewGame)
	{
		// 새 게임 상태
	}

	switch (m_SaveSelected)
	{
	case EUISaveList::First:
		CSceneManager::GetInst()->CreateNextScene(true);
		CSceneManager::GetInst()->CreateSceneInfo<CMainSceneInfo>(false);

		break;
	case EUISaveList::Second:
		CSceneManager::GetInst()->CreateNextScene(true);
		CSceneManager::GetInst()->CreateSceneInfo<CMainSceneInfo>(false);

		break;
	case EUISaveList::Third:
		CSceneManager::GetInst()->CreateNextScene(true);
		CSceneManager::GetInst()->CreateSceneInfo<CMainSceneInfo>(false);

		break;
	}
}

void CTitleSceneUI::OptionUISound()
{
	m_NowUIMode = EUIMode::OptionSound;

	// 사운드 옵션 UI 활성화
	InActiveOptionUI();
	ActiveOptionSoundUI();
}

void CTitleSceneUI::OptionUICamera()
{
	m_NowUIMode = EUIMode::OptionCamera;

	// 카메라 옵션 UI 활성화
	InActiveOptionUI();
	ActiveOptionCameraUI();
}

void CTitleSceneUI::OptionUISoundHovered()
{
	m_OptionSelected = EUIOptionList::Sound;

	auto iterSplotch = m_mapOptionUI.find("OptionUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapOptionUI.find("OptionUI_ButtonSound")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::OptionUICameraHovered()
{
	m_OptionSelected = EUIOptionList::Sound;

	auto iterSplotch = m_mapOptionUI.find("OptionUI_SelectedSplotch");
	float PosX = iterSplotch->second->GetPos().x;
	float PosY = m_mapOptionUI.find("OptionUI_ButtonCamera")->second->GetPos().y;
	iterSplotch->second->SetPos(PosX, PosY);
}

void CTitleSceneUI::SoundOptionUISFX()
{
	m_SoundSelected = EUISoundList::SFX;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonSfxOption")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CTitleSceneUI::SoundOptionUIMusic()
{
	m_SoundSelected = EUISoundList::Music;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonMusicOption")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CTitleSceneUI::SoundOptionUITalk()
{
	m_SoundSelected = EUISoundList::Talk;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonTalkOption")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}

void CTitleSceneUI::SoundOptionUIReset()
{
	m_SoundSelected = EUISoundList::Reset;

	auto iterSplotch = m_mapSoundOptionUI.find("SoundOptionUI_SelectedSplotch");
	Vector2 vecPos = m_mapSoundOptionUI.find("SoundOptionUI_ButtonReset")->second->GetPos();
	iterSplotch->second->SetPos(vecPos);
}
