#include "Font.h"

CFont::CFont()  :
    m_Format(nullptr)
{
}

CFont::~CFont()
{
    SAFE_RELEASE(m_Format);
}

bool CFont::LoadFont(IDWriteFactory5* WriteFactory, const std::string& Name, 
    const TCHAR* FontName, int Weight, float FontSize, const TCHAR* LocalName, int Stretch)
{
    m_Factory = WriteFactory;

    SetName(Name);

    if (FAILED(WriteFactory->CreateTextFormat(FontName, nullptr, (DWRITE_FONT_WEIGHT)Weight,
        DWRITE_FONT_STYLE_NORMAL, (DWRITE_FONT_STRETCH)Stretch, FontSize, LocalName, &m_Format)))
        return false;

    return true;
}

IDWriteTextLayout* CFont::CreateLayout(const TCHAR* Text, float Width, float Height)
{
    IDWriteTextLayout* Layout = nullptr;

    if (FAILED(m_Factory->CreateTextLayout(Text, lstrlen(Text), m_Format, Width,
        Height, &Layout)))
        return nullptr;

    return Layout;
}
