#pragma once
#include "EditorWidget.h"
class CEditorImage :
	public CEditorWidget
{
	friend class CEditorWindow;
	friend class CEditorGroup;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorImage();
	virtual ~CEditorImage();

private:
	CSharedPtr<class CTexture>	m_Texture;
	ImVec2		m_ImageStart;
	ImVec2		m_ImageEnd;
	ImVec4		m_BorderColor;
	int			m_ImageIndex;

public:
	class CTexture* GetTexture()    const
	{
		return m_Texture;
	}

	void SetImageIndex(int Index)
	{
		m_ImageIndex = Index;
	}

	void SetImageStart(float x, float y)
	{
		m_ImageStart.x = x;
		m_ImageStart.y = y;
	}

	void SetImageEnd(float x, float y)
	{
		m_ImageEnd.x = x;
		m_ImageEnd.y = y;
	}

	void SetBorderColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a)
	{
		m_BorderColor.x = r / 255.f;
		m_BorderColor.y = g / 255.f;
		m_BorderColor.z = b / 255.f;
		m_BorderColor.w = a / 255.f;
	}

	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(class CTexture* Texture);

public:
	virtual bool Init();
	virtual void Render();
};

