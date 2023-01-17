
#include "EditorImage.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/ResourceManager.h"

CEditorImage::CEditorImage()	:
	m_ImageStart{},
	m_ImageEnd{},
	m_BorderColor{},
	m_ImageIndex(0)
{
}

CEditorImage::~CEditorImage()
{
	if(m_Texture)
		CResourceManager::GetInst()->ReleaseTexture(m_Texture->GetName());
}

void CEditorImage::SetTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		m_Texture = CResourceManager::GetInst()->FindTexture("DefaultUI");

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CEditorImage::SetTexture(const std::string& Name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		m_Texture = CResourceManager::GetInst()->FindTexture("DefaultUI");

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CEditorImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	if (!m_Texture)
		m_Texture = CResourceManager::GetInst()->FindTexture("DefaultUI");

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CEditorImage::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;

	if (!m_Texture)
	{
		m_Texture = CResourceManager::GetInst()->FindTexture("DefaultUI");
	}

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

bool CEditorImage::Init()
{
	SetTexture("DefaultUI", TEXT("DefaultUI.png"));

	return true;
}

void CEditorImage::Render()
{
	if (m_Texture)
	{
		unsigned int	Width = m_Texture->GetWidth();
		unsigned int	Height = m_Texture->GetHeight();

		ImVec2 StartUV = ImVec2(m_ImageStart.x / Width, m_ImageStart.y / Height);
		ImVec2 EndUV = ImVec2(m_ImageEnd.x / Width, m_ImageEnd.y / Height);

		ImGui::Image(m_Texture->GetResource(m_ImageIndex), m_Size, StartUV,
			EndUV, m_Color, m_BorderColor);
	}
}
