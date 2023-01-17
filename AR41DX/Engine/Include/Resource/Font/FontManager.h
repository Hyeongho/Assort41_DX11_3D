#pragma once

#include "FontCollection.h"
#include "Font.h"

class CFontManager
{
	friend class CResourceManager;

private:
	CFontManager();
	~CFontManager();

private:
	IDWriteFactory5* m_WriteFactory;
	std::unordered_map<std::string, CSharedPtr<CFontCollection>>	m_mapFontCollection;
	std::unordered_map<std::string, CSharedPtr<CFont>>	m_mapFont;
	std::unordered_map<unsigned int, ID2D1SolidColorBrush*>	m_mapFontColor;


public:
	bool Init();

public:
	bool CreateFontCollection(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = FONT_PATH);
	bool LoadFont(const std::string& Name, const TCHAR* FontName,
		int Weight, float FontSize, const TCHAR* LocalName, int Stretch = 5);

	const TCHAR* GetFontFaceName(const std::string& CollectionName);
	const char* GetFontFaceNameMultibyte(const std::string& CollectionName);

	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(const Vector4& Color);
	bool CreateFontColor(unsigned int Color);

	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& Color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int Color);

	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(const Vector4& Color);

	CFont* FindFont(const std::string& Name);
	CFontCollection* FindFontCollection(const std::string& Name);
	void ReleaseFont(const std::string& Name);
	void ReleaseFontCollection(const std::string& Name);
};

