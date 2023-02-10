#include "UIButton.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Sound/Sound.h"
#include "../Input.h"

CUIButton::CUIButton()  :
    m_State(EButtonState::Normal),
    m_MouseHovered(false)
{
    m_WidgetTypeName = "UIButton";
    SetImageTint(EButtonState::Normal, 200, 200, 200, 255);
    SetImageTint(EButtonState::Hovered, 255, 255, 255, 255);
    SetImageTint(EButtonState::Click, 150, 150, 150, 255);
    SetImageTint(EButtonState::Disable, 100, 100, 100, 255);
}

CUIButton::CUIButton(const CUIButton& Button)   :
    CUIWidget(Button)
{
    m_State = Button.m_State;
    m_MouseHovered = false;

    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        m_TextureInfo[i] = Button.m_TextureInfo[i];
    }

    for (int i = 0; i < (int)EButtonEventState::Max; ++i)
    {
        m_Sound[i] = Button.m_Sound[i];
    }
}

CUIButton::~CUIButton()
{
}

void CUIButton::SetTexture(EButtonState State, CTexture* Texture)
{
    m_TextureInfo[(int)State].Texture = Texture;
}

bool CUIButton::SetTexture(EButtonState State, const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
            return false;

        m_TextureInfo[(int)State].Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
            return false;

        m_TextureInfo[(int)State].Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

bool CUIButton::SetTextureFullPath(EButtonState State, const std::string& Name, const TCHAR* FullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
            return false;

        m_TextureInfo[(int)State].Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
            return false;

        m_TextureInfo[(int)State].Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

bool CUIButton::SetTexture(EButtonState State, const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
    const std::string& PathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(Name, vecFileName, PathName))
            return false;

        m_TextureInfo[(int)State].Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
            return false;

        m_TextureInfo[(int)State].Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

bool CUIButton::SetTextureFullPath(EButtonState State, const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(Name, vecFullPath))
            return false;

        m_TextureInfo[(int)State].Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
            return false;

        m_TextureInfo[(int)State].Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

void CUIButton::SetImageTint(EButtonState State, const Vector4& Tint)
{
    m_TextureInfo[(int)State].Tint = Tint;
}

void CUIButton::SetImageTint(EButtonState State, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_TextureInfo[(int)State].Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CUIButton::AddFrameData(EButtonState State, const Vector2& Start, const Vector2& End)
{
    Animation2DFrameData    Frame;
    Frame.Start = Start;
    Frame.End = End;

    m_TextureInfo[(int)State].vecFrameData.push_back(Frame);

    m_TextureInfo[(int)State].FrameTime = m_TextureInfo[(int)State].vecFrameData.size() / m_TextureInfo[(int)State].PlayTime;
}

void CUIButton::SetPlayTime(EButtonState State, float PlayTime)
{
    m_TextureInfo[(int)State].PlayTime = PlayTime;

    m_TextureInfo[(int)State].FrameTime = m_TextureInfo[(int)State].vecFrameData.size() / m_TextureInfo[(int)State].PlayTime;
}

void CUIButton::SetPlayScale(EButtonState State, float PlayScale)
{
    m_TextureInfo[(int)State].PlayScale = PlayScale;
}

void CUIButton::SetSound(EButtonEventState State, CSound* Sound)
{
    m_Sound[(int)State] = Sound;
}

void CUIButton::SetSound(EButtonEventState State, const std::string& Name)
{
    if (m_Scene)
        m_Sound[(int)State] = m_Scene->GetResource()->FindSound(Name);

    else
        m_Sound[(int)State] = CResourceManager::GetInst()->FindSound(Name);
}

bool CUIButton::SetSound(EButtonEventState State, const std::string& GroupName, const std::string& Name,
    bool Loop, const char* FileName, const std::string& PathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadSound(GroupName, Name, Loop, FileName, PathName))
            return false;

        m_Sound[(int)State] = m_Scene->GetResource()->FindSound(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadSound(GroupName, Name, Loop, FileName, PathName))
            return false;

        m_Sound[(int)State] = CResourceManager::GetInst()->FindSound(Name);
    }

    return true;
}

void CUIButton::Start()
{
    CUIWidget::Start();
}

bool CUIButton::Init()
{
    if (!CUIWidget::Init())
        return false;

    return true;
}

void CUIButton::Update(float DeltaTime)
{
    CUIWidget::Update(DeltaTime);

    if (m_State != EButtonState::Disable)
    {
        // 마우스가 버튼 위에 올라왔는지 체크한다.
        Vector2 MousePos = CInput::GetInst()->GetMouseUIPos();

        bool MouseHovered = true;

        if (m_RenderPos.x > MousePos.x)
            MouseHovered = false;

        else if (m_RenderPos.y > MousePos.y)
            MouseHovered = false;

        else if (m_RenderPos.x + m_Size.x < MousePos.x)
            MouseHovered = false;

        else if (m_RenderPos.y + m_Size.y < MousePos.y)
            MouseHovered = false;

        if (MouseHovered)
        {
            if (CInput::GetInst()->GetMouseLDown())
            {
                if (m_State != EButtonState::Click)
                {
                    m_TextureInfo[(int)m_State].Frame = 0;
                    m_TextureInfo[(int)m_State].Time = 0.f;
                }

                m_State = EButtonState::Click;
            }

            else if (m_State == EButtonState::Click &&
                CInput::GetInst()->GetMouseLUp())
            {
                if (m_Sound[(int)EButtonEventState::Click])
                    m_Sound[(int)EButtonEventState::Click]->Play();

                if (m_ClickCallback[(int)EButtonEventState::Click])
                    m_ClickCallback[(int)EButtonEventState::Click]();

                if (m_State != EButtonState::Hovered)
                {
                    m_TextureInfo[(int)m_State].Frame = 0;
                    m_TextureInfo[(int)m_State].Time = 0.f;
                }

                m_State = EButtonState::Hovered;
            }

            else if (m_State == EButtonState::Click && CInput::GetInst()->GetMouseLPush())
                m_State = EButtonState::Click;

            else
            {
                if (m_MouseHovered != MouseHovered)
                {
                    if (m_Sound[(int)EButtonEventState::Hovered])
                        m_Sound[(int)EButtonEventState::Hovered]->Play();
                }

                if (m_ClickCallback[(int)EButtonEventState::Hovered])
                    m_ClickCallback[(int)EButtonEventState::Hovered]();

                if (m_State != EButtonState::Hovered)
                {
                    m_TextureInfo[(int)m_State].Frame = 0;
                    m_TextureInfo[(int)m_State].Time = 0.f;
                }

                m_State = EButtonState::Hovered;
            }

            m_MouseHovered = true;
        }

        else
        {
            m_MouseHovered = false;

            if (m_State != EButtonState::Normal)
            {
                m_TextureInfo[(int)m_State].Frame = 0;
                m_TextureInfo[(int)m_State].Time = 0.f;
            }

            m_State = EButtonState::Normal;
        }
    }

    if (!m_TextureInfo[(int)m_State].vecFrameData.empty())
    {
        m_TextureInfo[(int)m_State].Time += DeltaTime;

        if (m_TextureInfo[(int)m_State].Time >= m_TextureInfo[(int)m_State].FrameTime)
        {
            m_TextureInfo[(int)m_State].Time -= m_TextureInfo[(int)m_State].FrameTime;

            ++m_TextureInfo[(int)m_State].Frame;

            if (m_TextureInfo[(int)m_State].Frame == m_TextureInfo[(int)m_State].vecFrameData.size())
                m_TextureInfo[(int)m_State].Frame = 0;
        }
    }


}

void CUIButton::PostUpdate(float DeltaTime)
{
    CUIWidget::PostUpdate(DeltaTime);
}

void CUIButton::Render()
{
    // 상수버퍼를 채워준다.
    bool TextureEnable = m_TextureInfo[(int)m_State].Texture ? true : false;

    m_CBuffer->SetTextureEnable(TextureEnable);

    if (TextureEnable)
    {
        if (!m_TextureInfo[(int)m_State].vecFrameData.empty())
        {
            int TextureFrame = 0;

            if (m_TextureInfo[(int)m_State].Texture->GetImageType() == EImageType::Frame)
                TextureFrame = m_TextureInfo[(int)m_State].Frame;

            m_TextureInfo[(int)m_State].Texture->SetShader(0, (int)EShaderBufferType::Pixel, TextureFrame);

            m_AnimCBuffer->SetAnim2DEnable(true);
            m_AnimCBuffer->SetFrame(m_TextureInfo[(int)m_State].Frame);
            m_AnimCBuffer->SetImageFrame(m_TextureInfo[(int)m_State].vecFrameData[m_TextureInfo[(int)m_State].Frame].Start,
                m_TextureInfo[(int)m_State].vecFrameData[m_TextureInfo[(int)m_State].Frame].End);
            m_AnimCBuffer->SetImageSize((float)m_TextureInfo[(int)m_State].Texture->GetWidth(),
                (float)m_TextureInfo[(int)m_State].Texture->GetHeight());
            m_AnimCBuffer->SetImageType((EAnimation2DType)m_TextureInfo[(int)m_State].Texture->GetImageType());
        }

        else
        {
            m_TextureInfo[(int)m_State].Texture->SetShader(0, (int)EShaderBufferType::Pixel, 0);
            m_AnimCBuffer->SetAnim2DEnable(false);
        }
    }

    m_Tint = m_TextureInfo[(int)m_State].Tint;

    CUIWidget::Render();
}

CUIButton* CUIButton::Clone()
{
    return new CUIButton(*this);
}

void CUIButton::Save(FILE* File)
{
    CUIWidget::Save(File);

    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        fwrite(&m_TextureInfo[i].Tint, sizeof(Vector4), 1, File);

        fwrite(&m_TextureInfo[i].FrameTime, sizeof(float), 1, File);
        fwrite(&m_TextureInfo[i].PlayTime, sizeof(float), 1, File);
        fwrite(&m_TextureInfo[i].PlayScale, sizeof(float), 1, File);

        int FrameCount = (int)m_TextureInfo[i].vecFrameData.size();
        fwrite(&FrameCount, sizeof(int), 1, File);

        if (FrameCount > 0)
            fwrite(&m_TextureInfo[i].vecFrameData[0], sizeof(Animation2DFrameData), FrameCount, File);

        bool    TextureEnable = m_TextureInfo[i].Texture ? true : false;

        fwrite(&TextureEnable, sizeof(bool), 1, File);

        if (TextureEnable)
        {
            std::string TexName = m_TextureInfo[i].Texture->GetName();

            int Length = (int)TexName.length();
            fwrite(&Length, sizeof(int), 1, File);
            fwrite(TexName.c_str(), 1, Length, File);

            m_TextureInfo[i].Texture->Save(File);
        }
    }

    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        bool    SoundEnable = m_Sound[i] ? true : false;

        fwrite(&SoundEnable, sizeof(bool), 1, File);

        if (SoundEnable)
        {
            std::string SoundName = m_Sound[i]->GetName();

            int Length = (int)SoundName.length();
            fwrite(&Length, sizeof(int), 1, File);
            fwrite(SoundName.c_str(), 1, Length, File);

            m_Sound[i]->Save(File);
        }
    }
}

void CUIButton::Load(FILE* File)
{
    CUIWidget::Load(File);

    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        fread(&m_TextureInfo[i].Tint, sizeof(Vector4), 1, File);

        fread(&m_TextureInfo[i].FrameTime, sizeof(float), 1, File);
        fread(&m_TextureInfo[i].PlayTime, sizeof(float), 1, File);
        fread(&m_TextureInfo[i].PlayScale, sizeof(float), 1, File);

        int FrameCount = 0;
        fread(&FrameCount, sizeof(int), 1, File);

        if (FrameCount > 0)
        {
            m_TextureInfo[i].vecFrameData.resize((size_t)FrameCount);
            fread(&m_TextureInfo[i].vecFrameData[0], sizeof(Animation2DFrameData), FrameCount, File);
        }

        bool    TextureEnable = false;

        fread(&TextureEnable, sizeof(bool), 1, File);

        if (TextureEnable)
        {
            char    TexName[256] = {};

            int Length = 0;

            fread(&Length, sizeof(int), 1, File);
            fread(TexName, 1, Length, File);

            EImageType  ImageType;

            fread(&ImageType, sizeof(EImageType), 1, File);

            int	TextureSRVCount = 0;

            fread(&TextureSRVCount, sizeof(int), 1, File);

            if (TextureSRVCount == 1)
            {
                TCHAR	FileName[MAX_PATH] = {};
                char	PathName[MAX_PATH] = {};

                fread(FileName, sizeof(TCHAR), MAX_PATH, File);
                fread(PathName, sizeof(char), MAX_PATH, File);

                if (m_Scene)
                {
                    m_Scene->GetResource()->LoadTexture(TexName, FileName, PathName);

                    m_TextureInfo[i].Texture = m_Scene->GetResource()->FindTexture(TexName);
                }

                else
                {
                    CResourceManager::GetInst()->LoadTexture(TexName, FileName, PathName);

                    m_TextureInfo[i].Texture = CResourceManager::GetInst()->FindTexture(TexName);
                }
            }

            else
            {
                if (ImageType == EImageType::Frame)
                {
                    std::vector<const TCHAR*>	vecFileName;
                    std::string	ResultPathName;

                    for (int i = 0; i < TextureSRVCount; ++i)
                    {
                        TCHAR* FileName = new TCHAR[MAX_PATH];
                        char	PathName[MAX_PATH] = {};

                        fread(FileName, sizeof(TCHAR), MAX_PATH, File);
                        fread(PathName, sizeof(char), MAX_PATH, File);

                        ResultPathName = PathName;

                        vecFileName.push_back(FileName);
                    }

                    if (m_Scene)
                    {
                        m_Scene->GetResource()->LoadTexture(TexName, vecFileName, ResultPathName);

                        m_TextureInfo[i].Texture = m_Scene->GetResource()->FindTexture(TexName);
                    }

                    else
                    {
                        CResourceManager::GetInst()->LoadTexture(TexName, vecFileName, ResultPathName);

                        m_TextureInfo[i].Texture = CResourceManager::GetInst()->FindTexture(TexName);
                    }

                    for (int i = 0; i < TextureSRVCount; ++i)
                    {
                        SAFE_DELETE_ARRAY(vecFileName[i]);
                    }
                }

                else
                {
                    std::vector<const TCHAR*>	vecFileName;
                    std::string	ResultPathName;

                    for (int i = 0; i < TextureSRVCount; ++i)
                    {
                        TCHAR* FileName = new TCHAR[MAX_PATH];
                        char	PathName[MAX_PATH] = {};

                        fread(FileName, sizeof(TCHAR), MAX_PATH, File);
                        fread(PathName, sizeof(char), MAX_PATH, File);

                        ResultPathName = PathName;

                        vecFileName.push_back(FileName);
                    }

                    if (m_Scene)
                    {
                        m_Scene->GetResource()->LoadTextureArray(TexName, vecFileName, ResultPathName);

                        m_TextureInfo[i].Texture = m_Scene->GetResource()->FindTexture(TexName);
                    }

                    else
                    {
                        CResourceManager::GetInst()->LoadTextureArray(TexName, vecFileName, ResultPathName);

                        m_TextureInfo[i].Texture = CResourceManager::GetInst()->FindTexture(TexName);
                    }

                    for (int i = 0; i < TextureSRVCount; ++i)
                    {
                        SAFE_DELETE_ARRAY(vecFileName[i]);
                    }
                }
            }
        }
    }

    for (int i = 0; i < (int)EButtonState::Max; ++i)
    {
        bool    SoundEnable = false;

        fread(&SoundEnable, sizeof(bool), 1, File);

        if (SoundEnable)
        {
            char    SoundName[256] = {};

            int Length = 0;

            fread(&Length, sizeof(int), 1, File);
            fread(SoundName, 1, Length, File);

            bool    Loop = false;
            fread(&Loop, sizeof(bool), 1, File);

            Length = 0;
            char    GroupName[256] = {};

            fread(&Length, sizeof(int), 1, File);
            fread(GroupName, 1, Length, File);

            char    FileName[MAX_PATH] = {};
            char    PathName[MAX_PATH] = {};
            fread(FileName, sizeof(char), MAX_PATH, File);
            fread(PathName, sizeof(char), MAX_PATH, File);

            // Group 이름과 Loop 저장해야한다
            if (m_Scene)
            {
                m_Scene->GetResource()->LoadSound(GroupName, SoundName, Loop, FileName, PathName);
                m_Sound[i] = m_Scene->GetResource()->FindSound(SoundName);
            }

            else
            {
                CResourceManager::GetInst()->LoadSound(GroupName, SoundName, Loop, FileName, PathName);
                m_Sound[i] = CResourceManager::GetInst()->FindSound(SoundName);
            }
        }
    }
}
