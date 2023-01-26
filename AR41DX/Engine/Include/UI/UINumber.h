#pragma once
#include "UIWidget.h"
class CUINumber :
    public CUIWidget
{
    friend class CUIWindow;
    friend class CScene;

protected:
    CUINumber();
    CUINumber(const CUINumber& Number);
    virtual ~CUINumber();

protected:
    UIWidgetImageInfo           m_TextureInfo;
    unsigned int                m_Number;
    float                       m_Space;
    std::vector<unsigned int>   m_vecNumber;
public:
    UIWidgetImageInfo GetTextureInfo()	const
    {
        return m_TextureInfo;
    }

    unsigned int GetNumber()	const
    {
        return m_Number;
    }

    void SetNumber(unsigned int Number)
    {
        m_Number = Number;
    }

    void AddNumber(unsigned int Number)
    {
        m_Number += Number;
    }

    float GetSpace()	const
    {
        return m_Space;
    }
    void SetSpace(float space)
    {
        m_Space = space;
    }
public:
    void SetTexture(CTexture* Texture);
    bool SetTexture(const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
    bool SetTexture(const std::string& Name, const std::vector<const TCHAR*>& vecFileName,
        const std::string& PathName = TEXTURE_PATH);
    bool SetTextureFullPath(const std::string& Name, const std::vector<const TCHAR*>& vecFullPath);
    void SetImageTint(const Vector4& Tint);
    void SetImageTint(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddFrameData(const Vector2& Start, const Vector2& End);
    void SetPlayTime(float PlayTime);
    void SetPlayScale(float PlayScale);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUINumber* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

