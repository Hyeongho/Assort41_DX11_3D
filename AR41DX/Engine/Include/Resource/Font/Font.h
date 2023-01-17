#pragma once

#include "../../Ref.h"

class CFont :
    public CRef
{
    friend class CFontManager;

private:
    CFont();
    ~CFont();

private:
    IDWriteFactory5* m_Factory;
    IDWriteTextFormat* m_Format;

public:
    bool LoadFont(IDWriteFactory5* WriteFactory, const std::string& Name, const TCHAR* FontName,
        int Weight, float FontSize, const TCHAR* LocalName, int Stretch = 5);
    IDWriteTextLayout* CreateLayout(const TCHAR* Text, float Width, float Height);
};

