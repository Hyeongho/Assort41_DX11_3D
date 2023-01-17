#include "FontCollection.h"
#include "../../PathManager.h"

CFontCollection::CFontCollection()	:
	m_Collection(nullptr),
	m_FontFaceName{},
	m_FontFaceNameMultibyte{}
{
}

CFontCollection::~CFontCollection()
{
	SAFE_RELEASE(m_Collection);
}

bool CFontCollection::CreateFontCollection(IDWriteFactory5* WriteFactory, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FileName);

	IDWriteFontFile* FontFile = nullptr;

	WriteFactory->CreateFontFileReference(FullPath, nullptr, &FontFile);

	IDWriteFontSetBuilder1* FontBuilder = nullptr;

	if (FAILED(WriteFactory->CreateFontSetBuilder(&FontBuilder)))
		return false;

	FontBuilder->AddFontFile(FontFile);

	IDWriteFontSet* FontSet = nullptr;

	if (FAILED(FontBuilder->CreateFontSet(&FontSet)))
		return false;

	if (FAILED(WriteFactory->CreateFontCollectionFromFontSet(FontSet, &m_Collection)))
		return false;

	SAFE_RELEASE(FontSet);
	SAFE_RELEASE(FontBuilder);
	SAFE_RELEASE(FontFile);

	IDWriteFontFamily* Family = nullptr;

	if (FAILED(m_Collection->GetFontFamily(0, &Family)))
		return false;

	IDWriteLocalizedStrings* LocalizedName = nullptr;

	if (FAILED(Family->GetFamilyNames(&LocalizedName)))
		return false;

	if (FAILED(LocalizedName->GetString(0, m_FontFaceName, 64)))
		return false;

	int	Length = WideCharToMultiByte(CP_ACP, 0, m_FontFaceName, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, m_FontFaceName, -1, m_FontFaceNameMultibyte, Length, 0, 0);

	return true;
}

const TCHAR* CFontCollection::GetFontFaceName() const
{
	return m_FontFaceName;
}

const char* CFontCollection::GetFontFaceNameMultibyte() const
{
	return m_FontFaceNameMultibyte;
}
