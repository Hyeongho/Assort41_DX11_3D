#pragma once

#include "EditorInfo.h"

class CEditorWidget
{
	friend class CEditorWindow;
	friend class CEditorGroup;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorWidget();
	virtual ~CEditorWidget();

protected:
	std::string		m_Name;
	std::string		m_NameUTF8;
	class CEditorWindow* m_Owner;
	ImVec2			m_Size;
	ImVec4			m_Color;
	bool			m_IsHide;
public:
	const std::string& GetName()	const
	{
		return  m_Name;
	}
	bool GetIsHide()
	{
		return m_IsHide;
	}
	void SetName(const std::string& Name)
	{
		TCHAR	wName[256] = {};

		int Length = (int)MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, nullptr, 0);

		MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, wName, Length);

		char	NameUTF8[256] = {};

		Length = WideCharToMultiByte(CP_UTF8, 0, wName, -1, nullptr, 0, nullptr, nullptr);

		WideCharToMultiByte(CP_UTF8, 0, wName, -1, NameUTF8, Length, nullptr, nullptr);

		m_NameUTF8 = NameUTF8;

		m_Name = Name;
	}

	void SetHideName(const std::string& Name)
	{
		m_Name = "##" + Name;

		TCHAR	wName[256] = {};

		int Length = (int)MultiByteToWideChar(CP_ACP, 0, m_Name.c_str(), -1, nullptr, 0);

		MultiByteToWideChar(CP_ACP, 0, m_Name.c_str(), -1, wName, Length);

		char	NameUTF8[256] = {};

		Length = WideCharToMultiByte(CP_UTF8, 0, wName, -1, nullptr, 0, nullptr, nullptr);

		WideCharToMultiByte(CP_UTF8, 0, wName, -1, NameUTF8, Length, nullptr, nullptr);

		m_NameUTF8 = NameUTF8;
	}

	void SetSize(const ImVec2& Size)
	{
		m_Size = Size;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size.x = Size.x;
		m_Size.y = Size.y;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

	void SetSizeX(float x)
	{
		m_Size.x = x;
	}

	void SetSizeY(float y)
	{
		m_Size.y = y;
	}

	void SetColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a)
	{
		m_Color.x = r / 255.f;
		m_Color.y = g / 255.f;
		m_Color.z = b / 255.f;
		m_Color.w = a / 255.f;
	}

public:
	virtual bool Init() = 0;
	virtual void Render() = 0;
};

