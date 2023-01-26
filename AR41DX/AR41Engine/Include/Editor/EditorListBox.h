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

	std::string m_NullName; //지역변수 리턴한다는 경고막기위한 변수
public:
	const std::string& GetItem(int Index)	const
	{
		return m_vecItem[Index];
	}

	const std::string& GetSelectItem()	const
	{
		return m_vecItem[m_SelectIndex];
	}

	size_t GetSize()	const
	{
		return m_vecItem.size();
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
//김범중 부분검색기능 
//예- name="bg"이면 m_vecItem중에서 "bg"가 포함돼어 있는 문자열이 있으면 리턴한다
	const std::string& CheckPartitionItem(size_t index, std::string name)	const
	{
		// m_vecItem[index]의 Index i에서 length개에 해당하는 문자열을 name와 비교
		size_t vecLength = m_vecItem[index].size();
		size_t length = name.size();
		for (size_t i = 0; i < vecLength; ++i)
		{
			if (m_vecItem[index].compare(i, length, name) == 0)
			{
				return m_vecItem[index];
			}
		}
		return m_NullName;
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

