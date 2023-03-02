#include "UIImage.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Sound/Sound.h"
#include "../Input.h"

CUIImage::CUIImage()
{
    m_WidgetTypeName = "UIImage";
}

CUIImage::CUIImage(const CUIImage& Image) :
    CUIWidget(Image)
{
    m_TextureInfo = Image.m_TextureInfo;
}

CUIImage::~CUIImage()
{
}

void CUIImage::SetTexture(CTexture* Texture)
{
    m_TextureInfo.Texture = Texture;
}

bool CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
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

bool CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
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

bool CUIImage::SetTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
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

bool CUIImage::SetTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath)
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

void CUIImage::SetImageTint(const Vector4& Tint)
{
    m_TextureInfo.Tint = Tint;
}

void CUIImage::SetImageTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_TextureInfo.Tint = Vector4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void CUIImage::AddFrameData(const Vector2& Start, const Vector2& End)
{
    Animation2DFrameData    Frame;
    Frame.Start = Start;
    Frame.End = End;

    m_TextureInfo.vecFrameData.push_back(Frame);

    m_TextureInfo.FrameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.PlayTime;
}

void CUIImage::SetPlayTime(float PlayTime)
{
    m_TextureInfo.PlayTime = PlayTime;

    m_TextureInfo.FrameTime = m_TextureInfo.vecFrameData.size() / m_TextureInfo.PlayTime;
}

void CUIImage::SetPlayScale(float PlayScale)
{
    m_TextureInfo.PlayScale = PlayScale;
}

void CUIImage::Start()
{
    CUIWidget::Start();
}

bool CUIImage::Init()
{
    if (!CUIWidget::Init())
        return false;

    return true;
}

void CUIImage::Update(float DeltaTime)
{
    CUIWidget::Update(DeltaTime);

    if (!m_TextureInfo.vecFrameData.empty())
    {
        m_TextureInfo.Time += DeltaTime;

        if (m_TextureInfo.Time >= m_TextureInfo.FrameTime)
        {
            m_TextureInfo.Time -= m_TextureInfo.FrameTime;

            ++m_TextureInfo.Frame;

            if (m_TextureInfo.Frame == m_TextureInfo.vecFrameData.size())
                m_TextureInfo.Frame = 0;
        }
    }


}

void CUIImage::PostUpdate(float DeltaTime)
{
    CUIWidget::PostUpdate(DeltaTime);
}

void CUIImage::Render()
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
                TextureFrame = m_TextureInfo.Frame;

            m_TextureInfo.Texture->SetShader(0, (int)EShaderBufferType::Pixel, TextureFrame);

            m_AnimCBuffer->SetAnim2DEnable(true);
            m_AnimCBuffer->SetFrame(m_TextureInfo.Frame);
            m_AnimCBuffer->SetImageFrame(m_TextureInfo.vecFrameData[m_TextureInfo.Frame].Start,
                m_TextureInfo.vecFrameData[m_TextureInfo.Frame].End);
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


    CUIWidget::Render();
}

CUIImage* CUIImage::Clone()
{
    return new CUIImage(*this);
}

void CUIImage::Save(FILE* File)
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

    FrameCount = (int)m_TextureInfo.Texture->GetName().length();
    fwrite(&FrameCount, 4, 1, File);
    fwrite(m_TextureInfo.Texture->GetName().c_str(), 1, FrameCount, File);
    m_TextureInfo.Texture->Save(File);
}

void CUIImage::Load(FILE* File)
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

