
#include "EditorTree.h"

CEditorTree::CEditorTree()	:
	m_Root(nullptr)
{
}

CEditorTree::~CEditorTree()
{
	SAFE_DELETE(m_Root);
}

void CEditorTree::AddItem(const std::string& Item, const std::string& ParentItem)
{
	if (!m_Root)
	{
		m_Root = new CEditorTreeItem;

		m_Root->m_Item = Item;

		TCHAR	wText[256] = {};

		int Length = (int)MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, nullptr, 0);

		MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, wText, Length);

		char	TextUTF8[256] = {};

		Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);

		WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, nullptr, nullptr);

		m_Root->m_ItemUTF8 = TextUTF8;

		m_Root->SetSelectCallback<CEditorTree>(this, &CEditorTree::NodeSelect);
	}

	else
	{
		// Root를 교체할 경우
		if (ParentItem == "")
		{
			CEditorTreeItem* NewItem = new CEditorTreeItem;
			NewItem->m_Item = Item;

			TCHAR	wText[256] = {};

			int Length = (int)MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, nullptr, 0);

			MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, wText, Length);

			char	TextUTF8[256] = {};

			Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);

			WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, nullptr, nullptr);

			NewItem->m_ItemUTF8 = TextUTF8;

			NewItem->SetSelectCallback<CEditorTree>(this, &CEditorTree::NodeSelect);

			NewItem->AddItem(m_Root);

			m_Root = NewItem;
		}

		else
		{
			// 부모 아이템을 찾는다.
			CEditorTreeItem* Parent = m_Root->FindItem(ParentItem);

			if (Parent)
				Parent->AddItem(Item);
		}
	}
}

void CEditorTree::DeleteItem(const std::string& Item)
{
	if (!m_Root)
		return;

	CEditorTreeItem* Find = m_Root->FindItem(Item);

	if (!Find)
		return;

	CEditorTreeItem* Parent = Find->m_Parent;

	// 루트노드일 경우 가지고 있는 첫번째 아이템을 루트로 지정해준다.
	if (!Parent)
	{
		// 루트노드 하나 남은것을 지운다는 의미이다.
		if (Parent->m_vecChild.empty())
		{
			SAFE_DELETE(m_Root);
		}

		else
		{
			CEditorTreeItem* Root = m_Root->m_vecChild[0];

			size_t	Size = m_Root->m_vecChild.size();

			for (size_t i = 1; i < Size; ++i)
			{
				Root->AddItem(m_Root->m_vecChild[i]);
			}

			m_Root->m_vecChild.clear();

			Root->m_Parent = nullptr;

			SAFE_DELETE(m_Root);

			m_Root = Root;
		}
	}

	// 루트가 아닐 경우
	else
	{
		size_t	Size = Find->m_vecChild.size();

		for (size_t i = 1; i < Size; ++i)
		{
			Parent->m_vecChild.push_back(Find->m_vecChild[i]);
		}

		Find->m_vecChild.clear();

		SAFE_DELETE(Find);
	}
}

void CEditorTree::Clear()
{
	if (m_Root)
	{
		m_Root->Clear();
		SAFE_DELETE(m_Root);
	}
}

bool CEditorTree::Init()
{
	return true;
}

void CEditorTree::Render()
{
	if (!m_Root)
		return;

	m_Root->Render();
}

void CEditorTree::NodeSelect(CEditorTreeItem* Node, const std::string& Item)
{
	if (m_SelectCallback)
		m_SelectCallback(Item);
}
