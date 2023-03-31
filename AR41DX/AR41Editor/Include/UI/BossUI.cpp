#include "BossUI.h"
#include "UI/UIImage.h"

CBossUI::CBossUI()
{
    m_WindowTypeName = "BossUI";
}

CBossUI::CBossUI(const CBossUI& Window) :
    CUIWindow(Window)
{
}

CBossUI::~CBossUI()
{
}

void CBossUI::Start()
{
    CUIWindow::Start();
}

bool CBossUI::Init()
{
    CUIWindow::Init();

    CreateBossUI();

    return true;
}

void CBossUI::Update(float DeltaTime)
{
    CUIWindow::Update(DeltaTime);
}

void CBossUI::PostUpdate(float DeltaTime)
{
    CUIWindow::PostUpdate(DeltaTime);
}

void CBossUI::Render()
{
    CUIWindow::Render();
}

CBossUI* CBossUI::Clone()
{
    return new CBossUI(*this);
}

void CBossUI::Save(FILE* File)
{
    m_vecWidget.clear();
    CUIWindow::Save(File);
}

void CBossUI::Load(FILE* File)
{
    CUIWindow::Load(File);

    CreateBossUI();
}

void CBossUI::CreateBossUI()
{
    if (!m_mapBackUI.empty())
        return;


    CUIImage* Image = CreateWidget<CUIImage>("BossUI_ImageBoss");
    m_mapBackUI.insert(std::make_pair("BossUI_ImageBoss", Image));

    Image = CreateWidget<CUIImage>("BossUI_ImageHpBack1");
    Image->SetEnable(false);
    m_mapBackUI.insert(std::make_pair("BossUI_ImageHpBack1", Image));

    Image = CreateWidget<CUIImage>("BossUI_ImageHpBack2");
    Image->SetEnable(false);
    m_mapBackUI.insert(std::make_pair("BossUI_ImageHpBack1", Image));

    Image = CreateWidget<CUIImage>("BossUI_ImageHpBack3");
    Image->SetEnable(false);
    m_mapBackUI.insert(std::make_pair("BossUI_ImageHpBack1", Image));

    Image = CreateWidget<CUIImage>("BossUI_ImageHpBack4");
    Image->SetEnable(false);
    m_mapBackUI.insert(std::make_pair("BossUI_ImageHpBack1", Image));

    Image = CreateWidget<CUIImage>("BossUI_ImageHpBackRight");
    m_mapBackUI.insert(std::make_pair("BossUI_ImageHpBackRight", Image));


    for (int i = 0; i < 12; i++) {
        std::string strImgName = "BossUI_ImageHP" + std::to_string(i + 1);

        Image = CreateWidget<CUIImage>(strImgName);
        Image->SetEnable(false);

        m_mapHpUI.insert(std::make_pair(strImgName, Image));
    }
}

void CBossUI::ActiveBossUI()
{
    for (auto iter : m_mapBackUI)
        iter.second->SetEnable(true);

    for (auto iter : m_mapHpUI)
        iter.second->SetEnable(true);
}

void CBossUI::InActiveBossUI()
{
    for (auto iter : m_mapBackUI)
        iter.second->SetEnable(false);

    for (auto iter : m_mapHpUI)
        iter.second->SetEnable(false);
}

void CBossUI::CreateBossUI(const BossData& BossData)
{
    float BossUISizeX = 200.f;
    float BossUISizeY = 200.f;
    float BossUIPosX = 10.f;
    float BossUIPosY = 10.f;

    CUIImage* Image = FindWidget<CUIImage>("BossUI_ImageBoss");

    char FileName[MAX_PATH] = "UI/Boss/";

    switch (BossData.Boss)
    {
    case EBossList::KingJellyFish:
        strcat_s(FileName, "jfk_boss_ui_1.tga");
        break;
    case EBossList::RoboSponge:
        strcat_s(FileName, "Boss_Widgets_Robo_Spongebob.tga");
        break;
    case EBossList::RoboSandy:
        strcat_s(FileName, "rbs_boss_ui_1.tga");
        break;
    case EBossList::FlyingDutchman:
        strcat_s(FileName, "Boss_Widgets_Flying_Dutchman.tga");
        break;
    }

    TCHAR	ConvertFileName[MAX_PATH] = {};

    int	ConvertLength = MultiByteToWideChar(CP_ACP, 0, FileName, -1, nullptr, 0);
    MultiByteToWideChar(CP_ACP, 0, FileName, -1, ConvertFileName, ConvertLength);

    Image->SetTexture("BossUIBossImage", ConvertFileName);
    Image->SetSize(BossUISizeX, BossUISizeY);
    Image->SetPos(BossUIPosX, BossUIPosY);


    BossUIPosX += BossUISizeX;

    BossUISizeX *= 0.35f;


    int HpBackCount = BossData.MaxHP / 3;

    for (int i = 0; i < HpBackCount; i++) {
        std::string HpBackName = "BossUI_ImageHpBack" + std::to_string(i + 1);

        Image = FindWidget<CUIImage>(HpBackName);
        Image->SetEnable(true);

        Image->SetTexture("BossUIHpBackCenter", TEXT("UI/Boss/boss_ui_2.tga"));
        Image->SetSize(BossUISizeX, BossUISizeY);

        Image->SetPos(BossUIPosX, BossUIPosY);


        float BossUIHpSizeX = BossUISizeX * 0.11f;
        float BossUIHpSizeY = BossUIHpSizeX * 3.4f;

        float BossUIHpPosX = BossUIPosX + BossUISizeX * 0.28f;
        float BossUIHpPosY = BossUIHpSizeY * 1.2f + BossUIPosY;

        for (int j = 0; j < 3; j++) {

            std::string HpName = "BossUI_ImageHP" + std::to_string(i * 3 + j + 1);

            Image = (CUIImage*)m_mapHpUI.find(HpName)->second.Get();
            Image->SetEnable(true);

            if (i == 0) {
                Image->SetTexture("BossHPGreen", TEXT("UI/Boss/boss_energy_green.tga"));
            }
            else if (i == 1) {
                Image->SetTexture("BossHPRed", TEXT("UI/Boss/boss_energy_red.tga"));
            }
            else if (i == 2) {
                Image->SetTexture("BossHPYellow", TEXT("UI/Boss/boss_energy_yellow.tga"));
            }
            else {
                Image->SetTexture("BossHPWhite", TEXT("UI/Boss/boss_energy_white.tga"));
            }

            Image->SetSize(BossUIHpSizeX, BossUIHpSizeY);
            Image->SetPos(BossUIHpPosX, BossUIHpPosY);

            BossUIHpPosX += BossUISizeX * 0.28f;
        }

        BossUIPosX += BossUISizeX;
    }

    BossUISizeX *= 0.34f;

    Image = FindWidget<CUIImage>("BossUI_ImageHpBackRight");
    Image->SetTexture("BossUIHpBackRight", TEXT("UI/Boss/boss_ui_3.tga"));

    Image->SetSize(BossUISizeX, BossUISizeY);
    Image->SetPos(BossUIPosX, BossUIPosY);

    m_mapBackUI.insert(std::make_pair("BossUI_ImageHpBackRight", Image));
}

void CBossUI::SetDamage(int Hp)
{
    std::string HpName = "BossUI_ImageHP" + std::to_string(Hp);

    m_mapHpUI.find(HpName)->second->SetEnable(false);
}
