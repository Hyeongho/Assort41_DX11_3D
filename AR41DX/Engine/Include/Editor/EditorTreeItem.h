#pragma once

#include "EditorWidget.h"

/*
ImGuiTreeNodeFlags_None                 = 0,
ImGuiTreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
ImGuiTreeNodeFlags_Framed               = 1 << 1,   // Draw frame with background (e.g. for CollapsingHeader)
ImGuiTreeNodeFlags_AllowItemOverlap     = 1 << 2,   // Hit testing to allow subsequent widgets to overlap this one
ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush() when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically and temporarily open node when Logging is active (by default logging will automatically open tree nodes)
ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be open
ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Need double-click to open node
ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Only open when clicking on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single-click arrow or double-click all box to open.
ImGuiTreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arrow (use as a convenience for leaf nodes).
ImGuiTreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet instead of arrow
ImGuiTreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (even for an unframed text node) to vertically align text baseline to regular widget height. Equivalent to calling AlignTextToFramePadding().
ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the right-most edge, even if not framed. This is not the default in order to allow adding other items on the same line. In the future we may refactor the hit system to be front-to-back, allowing natural overlaps and then this can become the default.
ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the left-most and right-most edges (bypass the indented area).
ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // (WIP) Nav: left direction may move to this TreeNode() from any of its child (items submitted between TreeNode and TreePop)
//ImGuiTreeNodeFlags_NoScrollOnOpen     = 1 << 14,  // FIXME: TODO: Disable automatic scroll on TreePop() if node got just open and contents is not visible
ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog,
*/

template <typename T>
class CEditorTreeItem
{
	template <typename T>
	friend class CEditorTree;

private:
	CEditorTreeItem() :
		m_Parent(nullptr),
		m_Flag(ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow)
	{
	}

	~CEditorTreeItem()
	{
		size_t	Size = m_vecChild.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_vecChild[i]);
		}

		Size = m_vecWidget.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(m_vecWidget[i]);
		}
	}

private:
	ImGuiTreeNodeFlags	m_Flag;
	CEditorTreeItem<T>* m_Parent;
	std::vector<CEditorTreeItem<T>*>	m_vecChild;
	std::vector<CEditorWidget*>	m_vecWidget;
	std::string		m_Item;
	std::string		m_ItemUTF8;
	T				m_CustomData;
	std::function<void(CEditorTreeItem<T>*, const std::string&)>	m_SelectCallback;

public:
	T GetCustomData()	const
	{
		return m_CustomData;
	}

	void AddFlag(ImGuiTreeNodeFlags_ Flag)
	{
		m_Flag |= Flag;
	}

	void AddItem(const std::string& Item, T CustomData)
	{
		CEditorTreeItem<T>* Child = new CEditorTreeItem<T>;

		Child->m_Parent = this;
		Child->m_Item = Item;
		Child->m_SelectCallback = m_SelectCallback;
		Child->m_CustomData = CustomData;

		TCHAR	wText[256] = {};

		int Length = (int)MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, nullptr, 0);

		MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, wText, Length);

		char	TextUTF8[256] = {};

		Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);

		WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, nullptr, nullptr);

		Child->m_ItemUTF8 = TextUTF8;

		m_vecChild.push_back(Child);
	}

	void AddItem(CEditorTreeItem<T>* Item)
	{
		Item->m_Parent = this;
		Item->m_SelectCallback = m_SelectCallback;

		m_vecChild.push_back(Item);
	}

	CEditorTreeItem<T>* FindItem(const std::string& Item)
	{
		if (m_Item == Item)
			return this;

		size_t	Size = m_vecChild.size();

		for (size_t i = 0; i < Size; ++i)
		{
			CEditorTreeItem<T>* Find = m_vecChild[i]->FindItem(Item);

			if (Find)
				return Find;
		}

		return nullptr;
	}

	void Clear()
	{
		size_t	Size = m_vecChild.size();

		for (size_t i = 0; i < Size; ++i)
		{
			m_vecChild[i]->Clear();
			SAFE_DELETE(m_vecChild[i]);
		}

		m_vecChild.clear();
	}

	void Render()
	{
		ImGuiTreeNodeFlags	Flag = m_Flag;

		if (m_vecChild.empty())
			Flag |= ImGuiTreeNodeFlags_Leaf;

		bool	ItemOpen = ImGui::TreeNodeEx(m_ItemUTF8.c_str(), Flag);

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			if (m_SelectCallback)
				m_SelectCallback(this, m_Item);


		}

		size_t	WidgetCount = m_vecWidget.size();

		for (size_t i = 0; i < WidgetCount; ++i)
		{
			m_vecWidget[i]->Render();
		}

		/*if (ImGui::BeginDragDropSource())
		{
			ImGui::EndDragDropSource();
		}*/

		if (ItemOpen)
		{
			size_t	Size = m_vecChild.size();

			for (size_t i = 0; i < Size; ++i)
			{
				m_vecChild[i]->Render();
			}

			ImGui::TreePop();
		}
	}

public:
	template <typename CallbackType>
	void SetSelectCallback(CallbackType* Obj, void (CallbackType::* Func)(CEditorTreeItem<T>*, const std::string&))
	{
		m_SelectCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}

public:
	template <typename WidgetType>
	WidgetType* CreateWidget(const std::string& Name, float Width = 100.f, float Height = 100.f)
	{
		WidgetType* Widget = new WidgetType;

		Widget->SetName(Name);
		Widget->SetSize(Width, Height);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_vecWidget.push_back(Widget);

		return Widget;
	}

	template <typename WidgetType>
	WidgetType* FindWidget(const std::string& Name)
	{
		size_t	Size = m_vecWidget.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecWidget[i]->GetName() == Name)
				return (WidgetType*)m_vecWidget[i];
		}

		return nullptr;
	}

	template <typename WidgetType>
	WidgetType* FindWidgetHirearchy(const std::string& Name)
	{
		WidgetType* Widget = FindWidget<WidgetType>(Name);

		if (Widget)
			return Widget;

		size_t	Size = m_vecChild.size();

		for (size_t i = 0; i < Size; ++i)
		{
			Widget = m_vecChild[i]->FindWidgetHirearchy<WidgetType>(Name);

			if (Widget)
				return Widget;
		}

		return nullptr;
	}
};

