#pragma once

#include "../../Ref.h"

class CFontCollection :
    public CRef
{
    friend class CFontManager;

private:
    CFontCollection();
    ~CFontCollection();

private:
    IDWriteFontCollection1* m_Collection;
    TCHAR               m_FontFaceName[64];
    char                m_FontFaceNameMultibyte[64];

public:
    bool CreateFontCollection(IDWriteFactory5* WriteFactory, const std::string& Name, const TCHAR* FileName,
        const std::string& PathName = FONT_PATH);
    const TCHAR* GetFontFaceName()  const;
    const char* GetFontFaceNameMultibyte()  const;
};

