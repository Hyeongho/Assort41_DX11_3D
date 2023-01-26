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

	std::string m_NullName; //�������� �����Ѵٴ� ��������� ����
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
//����� �κа˻���� 
//��- name="bg"�̸� m_vecItem�߿��� "bg"�� ���Եž� �ִ� ���ڿ��� ������ �����Ѵ�
	const std::string& CheckPartitionItem(size_t index, std::string name)	const
	{
		// m_vecItem[index]�� Index i���� length���� �ش��ϴ� ���ڿ��� name�� ��
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

