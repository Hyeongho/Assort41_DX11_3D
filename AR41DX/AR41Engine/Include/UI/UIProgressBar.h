#pragma once

#include "UIWidget.h"
#include "../Resource/Shader/UIProgressBarConstantBuffer.h"

class CUIProgressBar :
    public CUIWidget
{
    friend class CUIWindow;
    friend class CResourceManager;
    friend class CScene;

protected:
    CUIProgressBar();
    CUIProgressBar(const CUIProgressBar& ProgressBar);
    virtual ~CUIProgressBar();

protected:
    static CUIProgressBarConstantBuffer* m_ProgressBarCBuffer;
    UIWidgetImageInfo   m_TextureInfo[(int)EProgressBarTextureType::Max];
    EProgressBarDir     m_BarDir;
    float               m_Min;
    float               m_Max;
    float               m_Value;
    CSharedPtr<class CShader>   m_BarShader;

public:
    UIWidgetImageInfo GetTextureInfo(int index)    const
    {
        return m_TextureInfo[index];
    }
    EProgressBarDir GetBarDir()    const
    {
        return m_BarDir;
    }
    float GetMinValue()    const
    {
        return m_Min;
    }
    float GetMaxValue()    const
    {
        return m_Max;
    }
    float GetValue()    const
    {
        return m_Value;
    }
public:
    void SetTexture(EProgressBarTextureType Type, CTexture* Texture);
    bool SetTexture(EProgressBarTextureType Type, const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(EProgressBarTextureType Type, const std::string& Name, const TCHAR* FullPath);
    bool SetTexture(EProgressBarTextureType Type, const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(EProgressBarTextureType Type, const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
    void SetImageTint(EProgressBarTextureType Type, const Vector4& Tint);
    void SetImageTint(EProgressBarTextureType Type, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(EProgressBarTextureType Type, const Vector2& Start, const Vector2& End);
    void SetPlayTime(EProgressBarTextureType Type, float PlayTime);
    void SetPlayScale(EProgressBarTextureType Type, float PlayScale);
    void SetProgressBarMin(float Min);
    void SetProgressBarMax(float Max);
    void SetValue(float Value);
    void AddValue(float Value);
    void SetBarDir(EProgressBarDir Dir)
    {
        m_BarDir = Dir;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUIProgressBar* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

