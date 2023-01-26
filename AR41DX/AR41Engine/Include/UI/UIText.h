#pragma once

#include "UIWidget.h"
#include "../Resource/Font/Font.h"

enum class Text_Align_H
{
    Left,
    Center,
    Right
};

enum class Text_Align_V
{
    Top,
    Middle,
    Bottom
};

class CUIText :
    public CUIWidget
{
    friend class CUIWindow;
    friend class CScene;

protected:
    CUIText();
    CUIText(const CUIText& Text);
    virtual ~CUIText();

protected:
    TCHAR* m_Text;
    int m_TextCount;
    int m_TextCapacity;

protected:
    CSharedPtr<CFont> m_Font;
    IDWriteTextLayout* m_Layout;
    ID2D1SolidColorBrush* m_Brush;
    ID2D1RenderTarget* m_RenderTarget;
    float       m_FontSize;
    bool        m_Transparency;
    float       m_Opacity;
    Vector4     m_Color;

    // Shadow
    bool        m_ShadowEnable;
    ID2D1SolidColorBrush* m_ShadowBrush;
    bool        m_ShadowTransparency;
    float       m_ShadowOpacity;
    Vector4     m_ShadowColor;
    Vector2     m_ShadowOffset;

    Text_Align_H    m_AlignH;
    Text_Align_V    m_AlignV;

public:
    const TCHAR* GetText()  const
    {
        return m_Text;
    }
    Text_Align_H GetAlignH()  const
    {
        return m_AlignH;
    }
    Text_Align_V GetAlignV()  const
    {
        return m_AlignV;
    }
    Vector4 GetColor()	const
    {
        return m_Color;
    }
    int GetTextCount()  const
    {
        return m_TextCount;
    }
    float GetFontSize()  const
    {
        return m_FontSize;
    }
    bool GetTransparency()  const
    {
        return m_Transparency;
    }
    Vector4 GetShadowColor()	const
    {
        return m_ShadowColor;
    }
    float GetShadowOpacity()  const
    {
        return m_ShadowOpacity;
    }
    Vector2 GetShadowOffset()	const
    {
        return m_ShadowOffset;
    }
    bool GetShadowEnable()  const
    {
        return m_ShadowEnable;
    }
    bool GetShadowTransparency()  const
    {
        return m_ShadowTransparency;
    }
public:
    void SetText(const TCHAR* Text)
    {
        int Length = lstrlen(Text);

        if (Length + 1 > m_TextCapacity)
        {
            m_TextCapacity = Length + 1;

            SAFE_DELETE_ARRAY(m_Text);

            m_Text = new TCHAR[m_TextCapacity];

            memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
        }

        lstrcpy(m_Text, Text);
        m_TextCount = Length;

        CreateLayout();
    }

    void SetText(const char* Text)
    {
        TCHAR text[MAX_PATH] = {};
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Text, (int)strlen(Text), text, 256);
        SetText(text);
    }

    void SetIntText(int text, bool needZero = false)
    {
        TCHAR number[MAX_PATH] = {};
        if (text < 10 && needZero)
        {
            swprintf_s(number, TEXT("0%d"), text);
        }
        else
        {
            swprintf_s(number, TEXT("%d"), text);
        }
        SetText(number);
    }

    void SetFloatText(float text)
    {
        TCHAR number[MAX_PATH] = {};
        swprintf_s(number, TEXT("%f"), text);
        SetText(number);
    }

    void Append(const TCHAR* Text)
    {
        int Length = lstrlen(Text) + m_TextCount;

        if (Length + 1 > m_TextCapacity)
        {
            m_TextCapacity = Length + 1;

            TCHAR* NewText = new TCHAR[m_TextCapacity];

            memset(NewText, 0, sizeof(TCHAR) * m_TextCapacity);

            lstrcpy(NewText, m_Text);

            SAFE_DELETE_ARRAY(m_Text);

            m_Text = NewText;
        }

        lstrcat(m_Text, Text);
        m_TextCount += lstrlen(Text);

        CreateLayout();
    }

    void pop_back()
    {
        if (m_TextCount > 0)
        {
            --m_TextCount;

            m_Text[m_TextCount] = 0;

            CreateLayout();
        }
    }

    void Clear()
    {
        if (m_TextCount > 0)
        {
            m_TextCount = 0;

            memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

            CreateLayout();
        }
    }

public:
    virtual void SetSize(const Vector2& Size)
    {
        CUIWidget::SetSize(Size);

        CreateLayout();
    }

    virtual void SetSize(float x, float y)
    {
        CUIWidget::SetSize(x, y);

        CreateLayout();
    }

public:
    void SetFont(const std::string& Name);
    void SetFontSize(float Size);
    void SetAlignH(Text_Align_H Align);
    void SetAlignV(Text_Align_V Align);
    void SetColor(unsigned char r, unsigned char g, unsigned char b);
    void SetColor(const Vector4& Color);
    void SetColor(unsigned int Color);
    void SetTransparency(bool Transparency);
    void SetOpacity(float Opacity);

    void SetShadowEnable(bool Enable);
    void SetShadowOffset(const Vector2& Offset);
    void SetShadowOffset(float x, float y);
    void SetShadowColor(unsigned char r, unsigned char g, unsigned char b);
    void SetShadowColor(const Vector4& Color);
    void SetShadowColor(unsigned int Color);
    void SetShadowTransparency(bool Transparency);
    void SetShadowOpacity(float Opacity);

private:
    void CreateLayout();



public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUIText* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

