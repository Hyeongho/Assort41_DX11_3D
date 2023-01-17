#pragma once
#include "EditorWidget.h"
class CEditorListBox :
    public CEditorWidget
{
	friend class CEditorWindow;
	friend class CEditorGroup;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorListBox();
	virtual ~CEditorListBox();

private:
	std::vector<std::string>	m_vecItem;
	std::vector<std::string>	m_vecItemUTF8;
	bool						m_Select;
	int							m_SelectIndex;
	int							m_PageItemCount;
	bool						m_Sort;
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

