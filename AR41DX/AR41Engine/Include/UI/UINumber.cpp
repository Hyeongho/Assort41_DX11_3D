#include "UINumber.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Sound/Sound.h"
#include "../Input.h"

CUINumber::CUINumber()
{
    m_WidgetTypeName = "UINumber";
}

CUINumber::CUINumber(const CUINumber& Number) :
    CUIWidget(Number)
{
    m_TextureInfo = Number.m_TextureInfo;
}

CUINumber::~CUINumber()
{
}

void CUINumber::SetTexture(CTexture* Texture)
{
    m_TextureInfo.Texture = Texture;
}

bool CUINumber::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
            return false;

        m_TextureInfo.Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
            return false;

        m_TextureInfo.Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

bool CUINumber::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(Name, FullPath))
            return false;

        m_TextureInfo.Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
            return false;

        m_TextureInfo.Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

bool CUINumber::SetTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
    const std::string& PathName)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTexture(Name, vecFileName, PathName))
            return false;

        m_TextureInfo.Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
            return false;

        m_TextureInfo.Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

bool CUINumber::SetTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
{
    if (m_Scene)
    {
        if (!m_Scene->GetResource()->LoadTextureFullPath(Name, vecFullPath))
            return false;

        m_TextureInfo.Texture = m_Scene->GetResource()->FindTexture(Name);
    }

    else
    {
        if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, vecFullPath))
            return false;

        m_TextureInfo.Texture = CResourceManager::GetInst()->FindTexture(Name);
    }

    return true;
}

void CUINumber::SetImageTint(const Vector4& Tint)
{
    m_TextureInfo.Tint = Tint;
}

void CUINumber::SetImageTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_TextureInfo.Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CUINumber::AddFrameData(const Vector2& Start, const Vector2& End)
{
    Animation2DFrameData    Frame;
    Frame.Start = Start;
    Frame.End = End;

    m_TextureInfo.vecFrameData.push_back(Frame);

    m_TextureInfo.FrameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.PlayTime;
}

void CUINumber::SetPlayTime(float PlayTime)
{
    m_TextureInfo.PlayTime = PlayTime;

    m_TextureInfo.FrameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.PlayTime;
}

void CUINumber::SetPlayScale(float PlayScale)
{
    m_TextureInfo.PlayScale = PlayScale;
}

void CUINumber::Start()
{
    CUIWidget::Start();
}

bool CUINumber::Init()
{
    if (!CUIWidget::Init())
        return false;

    return true;
}

void CUINumber::Update(float DeltaTime)
{
    CUIWidget::Update(DeltaTime);

    if (m_Number == 0)
    {
        m_vecNumber.clear();
        m_vecNumber.push_back(0);
    }

    else
    {
        unsigned int Number = m_Number;

        std::stack<int> NumberStack;

        while (Number > 0)
        {
            NumberStack.push(Number % 10);
            Number /= 10;
        }

        m_vecNumber.clear();

        while (!NumberStack.empty())
        {
            m_vecNumber.push_back(NumberStack.top());
            NumberStack.pop();
        }
    }
}

void CUINumber::PostUpdate(float DeltaTime)
{
    CUIWidget::PostUpdate(DeltaTime);
}

void CUINumber::Render()
{
    size_t  Size = m_vecNumber.size();

    float   Space = 0.f;

    for (size_t i = 0; i < Size; ++i)
    {
        // 상수버퍼를 채워준다.
        bool TextureEnable = m_TextureInfo.Texture ? true : false;

        m_CBuffer->SetTextureEnable(TextureEnable);

        if (TextureEnable)
        {
            if (!m_TextureInfo.vecFrameData.empty())
            {
                int TextureFrame = 0;

                if (m_TextureInfo.Texture->GetImageType() == EImageType::Frame)
                    TextureFrame = (int)m_vecNumber[i];

                m_TextureInfo.Texture->SetShader(0, (int)EShaderBufferType::Pixel, TextureFrame);

                m_AnimCBuffer->SetAnim2DEnable(true);
                m_AnimCBuffer->SetFrame(m_vecNumber[i]);
                m_AnimCBuffer->SetImageFrame(m_TextureInfo.vecFrameData[m_vecNumber[i]].Start,
                    m_TextureInfo.vecFrameData[m_vecNumber[i]].End);
                m_AnimCBuffer->SetImageSize((float)m_TextureInfo.Texture->GetWidth(),
                    (float)m_TextureInfo.Texture->GetHeight());
                m_AnimCBuffer->SetImageType((EAnimation2DType)m_TextureInfo.Texture->GetImageType());
            }

            else
            {
                m_TextureInfo.Texture->SetShader(0, (int)EShaderBufferType::Pixel, 0);
                m_AnimCBuffer->SetAnim2DEnable(false);
            }
        }

        m_Tint = m_TextureInfo.Tint;

        m_RenderPos.x += Space;

        Space = m_Size.x + 5.f;

        CUIWidget::Render();
    }
}

CUINumber* CUINumber::Clone()
{
    return new CUINumber(*this);
}

void CUINumber::Save(FILE* File)
{
    CUIWidget::Save(File);

    fwrite(&m_TextureInfo.Tint, sizeof(Vector4), 1, File);

    fwrite(&m_TextureInfo.FrameTime, sizeof(float), 1, File);
    fwrite(&m_TextureInfo.PlayTime, sizeof(float), 1, File);
    fwrite(&m_TextureInfo.PlayScale, sizeof(float), 1, File);

    int FrameCount = (int)m_TextureInfo.vecFrameData.size();
    fwrite(&FrameCount, sizeof(int), 1, File);

    if (FrameCount > 0)
        fwrite(&m_TextureInfo.vecFrameData[0], sizeof(Animation2DFrameData), FrameCount, File);

    bool    TexEnable = false;

    if (m_TextureInfo.Texture)
        TexEnable = true;

    fwrite(&TexEnable, sizeof(bool), 1, File);

    if (TexEnable)
    {
        std::string TexName = m_TextureInfo.Texture->GetName();

        int Length = (int)TexName.length();
        fwrite(&Length, sizeof(int), 1, File);
        fwrite(TexName.c_str(), 1, Length, File);

        m_TextureInfo.Texture->Save(File);
    }

    fwrite(&m_Number, sizeof(unsigned int), 1, File);
}

void CUINumber::Load(FILE* File)
{
    CUIWidget::Load(File);

    fread(&m_TextureInfo.Tint, sizeof(Vector4), 1, File);

    fread(&m_TextureInfo.FrameTime, sizeof(float), 1, File);
    fread(&m_TextureInfo.PlayTime, sizeof(float), 1, File);
    fread(&m_TextureInfo.PlayScale, sizeof(float), 1, File);

    int FrameCount = 0;
    fread(&FrameCount, sizeof(int), 1, File);

    m_TextureInfo.vecFrameData.resize((size_t)FrameCount);

    if (FrameCount > 0)
        fread(&m_TextureInfo.vecFrameData[0], sizeof(Animation2DFrameData), FrameCount, File);

    bool    TexEnable = false;

    fread(&TexEnable, sizeof(bool), 1, File);

    if (TexEnable)
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

                m_TextureInfo.Texture = m_Scene->GetResource()->FindTexture(TexName);
            }

            else
            {
                CResourceManager::GetInst()->LoadTexture(TexName, FileName, PathName);

                m_TextureInfo.Texture = CResourceManager::GetInst()->FindTexture(TexName);
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

                    m_TextureInfo.Texture = m_Scene->GetResource()->FindTexture(TexName);
                }

                else
                {
                    CResourceManager::GetInst()->LoadTexture(TexName, vecFileName, ResultPathName);

                    m_TextureInfo.Texture = CResourceManager::GetInst()->FindTexture(TexName);
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

                    m_TextureInfo.Texture = m_Scene->GetResource()->FindTexture(TexName);
                }

                else
                {
                    CResourceManager::GetInst()->LoadTextureArray(TexName, vecFileName, ResultPathName);

                    m_TextureInfo.Texture = CResourceManager::GetInst()->FindTexture(TexName);
                }

                for (int i = 0; i < TextureSRVCount; ++i)
                {
                    SAFE_DELETE_ARRAY(vecFileName[i]);
                }
            }
        }
    }

    fread(&m_Number, sizeof(unsigned int), 1, File);
}

