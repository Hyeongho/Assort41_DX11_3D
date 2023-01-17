#pragma once

#include "EditorWidget.h"

class CEditorComboBox :
    public CEditorWidget
{
	friend class CEditorWindow;
	friend class CEditorGroup;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorComboBox();
	virtual ~CEditorComboBox();

private:
	std::vector<std::string>	m_vecItem;
	std::vector<std::string>	m_vecItemUTF8;
	std::string					m_PrevViewName;
	std::string					m_PrevViewNameUTF8;
	bool						m_Select;
	bool						m_SelectPrevViewName;
	int							m_SelectIndex;
	int							m_PageItemCount;
	bool						m_Sort;
	ImGuiComboFlags				m_Flag;
	std::function<void(int, const std::string&)>	m_SelectCallback;

public:
	const std::string& GetItem(int Index)	const
	{
		return m_vecItem[Index];
	}

	const std::string& GetSelectItem()	const
	{
		return m_vecItem[m_SelectIndex];
	}

	int GetSelectIndex()	const
	{
		return m_SelectIndex;
	}

public:
	void SetSelectPrevViewName(bool Enable)
	{
		m_SelectPrevViewName = Enable;
	}

	void SetPrevViewName(const std::string& Name)
	{
		TCHAR	wText[256] = {};

		int Length = (int)MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, nullptr, 0);

		MultiByteToWideChar(CP_ACP, 0, Name.c_str(), -1, wText, Length);

		char	TextUTF8[256] = {};

		Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);

		WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, nullptr, nullptr);

		m_PrevViewName = Name;
		m_PrevViewNameUTF8 = TextUTF8;
	}

	void SetSelect(int Index)
	{
		m_SelectIndex = Index;
	}

	bool CheckItem(const std::string& Item)	const
	{
		size_t	Size = m_vecItem.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecItem[i] == Item)
				return true;
		}

		return false;
	}

	void SetPageItemCount(int Count)
	{
		m_PageItemCount = Count;
	}

	void Clear()
	{
		m_vecItem.clear();
		m_vecItemUTF8.clear();
	}

	void AddFlag(ImGuiComboFlags_ Flag)
	{
		m_Flag |= Flag;
	}

	/*
	ImGuiComboFlags_None                    = 0,
    ImGuiComboFlags_PopupAlignLeft          = 1 << 0,   // Align the popup toward the left by default
    ImGuiComboFlags_HeightSmall             = 1 << 1,   // Max ~4 items visible. Tip: If you want your combo popup to be a specific size you can use SetNextWindowSizeConstraints() prior to calling BeginCombo()
    ImGuiComboFlags_HeightRegular           = 1 << 2,   // Max ~8 items visible (default)
    ImGuiComboFlags_HeightLarge             = 1 << 3,   // Max ~20 items visible
    ImGuiComboFlags_HeightLargest           = 1 << 4,   // As many fitting items as possible
    ImGuiComboFlags_NoArrowButton           = 1 << 5,   // Display on the preview box without the square arrow button
    ImGuiComboFlags_NoPreview               = 1 << 6,   // Display only a square arrow button
    ImGuiComboFlags_HeightMask_             = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest,
	*/

	void AddItem(const std::string& Item);
	void DeleteItem(const std::string& Item);
	void DeleteItem(int Index);
	void Sort(bool _Sort);

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetSelectCallback(T* Obj, void(T::* Func)(int, const std::string&))
	{
		m_SelectCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}
};

