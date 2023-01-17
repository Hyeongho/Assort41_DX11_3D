#include "FontManager.h"
#include "../../Device.h"

CFontManager::CFontManager()    :
    m_WriteFactory(nullptr)
{
}

CFontManager::~CFontManager()
{
    {
        auto    iter = m_mapFontColor.begin();
        auto    iterEnd = m_mapFontColor.end();

        for (; iter != iterEnd; ++iter)
        {
            SAFE_RELEASE(iter->second);
        }
    }

    SAFE_RELEASE(m_WriteFactory);
}

bool CFontManager::Init()
{
    if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory),
        (IUnknown**)&m_WriteFactory)))
        return false;

    CreateFontCollection("Default", TEXT("NotoSansKR-Regular.otf"));

    LoadFont("Default", GetFontFaceName("Default"), 600, 20.f, TEXT("ko"));

    return true;
}

bool CFontManager::CreateFontCollection(const std::string& Name, const TCHAR* FileName,
    const std::string& PathName)
{
    CFontCollection* FontCollection = FindFontCollection(Name);

    if (FontCollection)
        return true;

    FontCollection = new CFontCollection;

    if (!FontCollection->CreateFontCollection(m_WriteFactory, Name, FileName, PathName))
    {
        SAFE_DELETE(FontCollection);
        return false;
    }


    m_mapFontCollection.insert(std::make_pair(Name, FontCollection));

    return true;
}

bool CFontManager::LoadFont(const std::string& Name, const TCHAR* FontName, int Weight,
    float FontSize, const TCHAR* LocalName, int Stretch)
{
    CFont* Font = FindFont(Name);

    if (Font)
        return true;

    Font = new CFont;

    if (!Font->LoadFont(m_WriteFactory, Name, FontName, Weight, FontSize, LocalName, Stretch))
    {
        SAFE_DELETE(Font);
        return false;
    }


    m_mapFont.insert(std::make_pair(Name, Font));

    return true;
}

const TCHAR* CFontManager::GetFontFaceName(const std::string& CollectionName)
{
    CFontCollection* Collection = FindFontCollection(CollectionName);

    if (!Collection)
        return TEXT("");

    return Collection->GetFontFaceName();
}

const char* CFontManager::GetFontFaceNameMultibyte(const std::string& CollectionName)
{
    CFontCollection* Collection = FindFontCollection(CollectionName);

    if (!Collection)
        return "";

    return Collection->GetFontFaceNameMultibyte();
}

bool CFontManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b,
    unsigned char a)
{
    return CreateFontColor(CreateFontColorKey(r, g, b, a));
}

bool CFontManager::CreateFontColor(float r, float g, float b, float a)
{
    return CreateFontColor(CreateFontColorKey(r, g, b, a));
}

bool CFontManager::CreateFontColor(const Vector4& Color)
{
    return CreateFontColor(CreateFontColorKey(Color));
}

bool CFontManager::CreateFontColor(unsigned int Color)
{
    ID2D1SolidColorBrush* Brush = FindFontColor(Color);

    if (Brush)
        return true;

    float r, g, b, a;
    b = (Color & 0x000000ff) / 255.f;
    g = ((Color >> 8) & 0x000000ff) / 255.f;
    r = ((Color >> 16) & 0x000000ff) / 255.f;
    a = ((Color >> 24) & 0x000000ff) / 255.f;

    if (FAILED(CDevice::GetInst()->Get2DTarget()->CreateSolidColorBrush(
        D2D1::ColorF(r, g, b, a), &Brush)))
        return false;

    m_mapFontColor.insert(std::make_pair(Color, Brush));

    return true;
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned char r, unsigned char g, 
    unsigned char b, unsigned char a)
{
    return FindFontColor(CreateFontColorKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(float r, float g, float b, float a)
{
    return FindFontColor(CreateFontColorKey(r, g, b, a));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(const Vector4& Color)
{
    return FindFontColor(CreateFontColorKey(Color));
}

ID2D1SolidColorBrush* CFontManager::FindFontColor(unsigned int Color)
{
    auto    iter = m_mapFontColor.find(Color);

    if (iter == m_mapFontColor.end())
        return nullptr;

    return iter->second;
}

unsigned int CFontManager::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b,
    unsigned char a)
{
    unsigned int Key = 0;

    Key |= a;
    Key <<= 8;

    Key |= r;
    Key <<= 8;

    Key |= g;
    Key <<= 8;

    Key |= b;

    return Key;
}

unsigned int CFontManager::CreateFontColorKey(float r, float g, float b, float a)
{
    unsigned char _r, _g, _b, _a;

    _r = (unsigned char)r * 255;
    _g = (unsigned char)g * 255;
    _b = (unsigned char)b * 255;
    _a = (unsigned char)a * 255;

    unsigned int Key = 0;

    Key |= _a;
    Key <<= 8;

    Key |= _r;
    Key <<= 8;

    Key |= _g;
    Key <<= 8;

    Key |= _b;

    return Key;
}

unsigned int CFontManager::CreateFontColorKey(const Vector4& Color)
{
    unsigned char _r, _g, _b, _a;

    _r = (unsigned char)Color.x * 255;
    _g = (unsigned char)Color.y * 255;
    _b = (unsigned char)Color.z * 255;
    _a = (unsigned char)Color.w * 255;

    unsigned int Key = 0;

    Key |= _a;
    Key <<= 8;

    Key |= _r;
    Key <<= 8;

    Key |= _g;
    Key <<= 8;

    Key |= _b;

    return Key;
}

CFont* CFontManager::FindFont(const std::string& Name)
{
    auto    iter = m_mapFont.find(Name);

    if (iter == m_mapFont.end())
        return nullptr;

    return iter->second;
}

CFontCollection* CFontManager::FindFontCollection(const std::string& Name)
{
    auto    iter = m_mapFontCollection.find(Name);

    if (iter == m_mapFontCollection.end())
        return nullptr;

    return iter->second;
}

void CFontManager::ReleaseFont(const std::string& Name)
{
    auto	iter = m_mapFont.find(Name);

    if (iter == m_mapFont.end())
        return;

    if (iter->second->GetRefCount() == 1)
        m_mapFont.erase(iter);
}

void CFontManager::ReleaseFontCollection(const std::string& Name)
{
    auto	iter = m_mapFontCollection.find(Name);

    if (iter == m_mapFontCollection.end())
        return;

    if (iter->second->GetRefCount() == 1)
        m_mapFontCollection.erase(iter);
}
