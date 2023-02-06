#pragma once
#include "EditorWidget.h"

template <typename T>
class CEditorTreeItem
{
private:
	template <typename T>
	friend class CEditorTree;
	CEditorTreeItem<T>* m_Parent;
	CEditorTreeItem<T>* m_DragItem;
	CEditorTreeItem<T>* m_HoverItem;
	std::vector<CEditorTreeItem<T>*>	m_vecChild;
	std::vector<CEditorWidget*>	m_vecWidget;
	ImGuiTreeNodeFlags	m_Flag;
	std::string		m_WindowName;
	std::string		m_Item;
	std::string		m_ItemUTF8;
	T				m_CustomData;
	bool m_UseDragDropSelf;
	bool m_UseDragDropOuter;
	std::function<void(CEditorTreeItem<T>*, const std::string&)>	m_SelectCallback;
	std::function<void(CEditorTreeItem<T>*, const std::string&)>	m_DoubleClickCallback;
	std::function<void(CEditorTreeItem<T>*, CEditorTreeItem<T>* 
		,const std::string&, const std::string&)>	m_DragAndDropCallback;
	CEditorTreeItem()
		: m_Parent(nullptr)
		, m_Flag(ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow)
		, m_UseDragDropSelf(false)
		, m_UseDragDropOuter(false)
	{
	}
	~CEditorTreeItem()
	{
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			SAFE_DELETE(m_vecChild[i]);
		}
		size = m_vecWidget.size();
		for (size_t i = 0; i < size; ++i)
		{
			SAFE_DELETE(m_vecWidget[i]);
		}
	}
public:
	T GetCustomData()	const
	{
		return m_CustomData;
	}
	const std::string& GetItemName()	const
	{
		return m_Item;
	}
	void AddFlag(ImGuiTreeNodeFlags_ flag)
	{
		m_Flag |= flag;
	}
	void AddItem(const std::string& item, T customData)
	{
		CEditorTreeItem<T>* child = new CEditorTreeItem<T>;
		child->m_Parent = this;
		child->m_Item = item;
		child->m_SelectCallback = m_SelectCallback;
		child->m_DoubleClickCallback = m_DoubleClickCallback;
		child->m_DragAndDropCallback = m_DragAndDropCallback;
		child->m_CustomData = customData;
		child->m_WindowName = child->m_Parent->m_WindowName;
		if (child->m_Parent->m_UseDragDropSelf)
		{
			child->m_UseDragDropSelf = m_UseDragDropSelf;
		}
		if (child->m_Parent->m_UseDragDropOuter)
		{
			child->m_UseDragDropOuter = m_UseDragDropOuter;
		}
		TCHAR	wText[256] = {};
		int length = (int)MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, nullptr, 0);
		MultiByteToWideChar(CP_ACP, 0, item.c_str(), -1, wText, length);
		char	textUTF8[256] = {};
		length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
		WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
		child->m_ItemUTF8 = textUTF8;
		m_vecChild.push_back(child);
	}
	void AddItem(CEditorTreeItem<T>* item)
	{
		item->m_Parent = this;
		item->m_SelectCallback = m_SelectCallback;
		item->m_DoubleClickCallback = m_DoubleClickCallback;
		item->m_DragAndDropCallback = m_DragAndDropCallback;
		m_vecChild.push_back(item);
	}
	CEditorTreeItem<T>* FindItem(const std::string& item)
	{
		if (m_Item == item)
		{
			return this;
		}
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			CEditorTreeItem<T>* find = m_vecChild[i]->FindItem(item);
			if (find)
			{
				return find;
			}
		}
		return nullptr;
	}
	int FindIndex(const std::string& item)
	{
		if(!m_Parent)
		{
			return -1;
		}
		int	size = (int)m_Parent->m_vecChild.size();
		for (int i = 0; i < size; ++i)
		{
			CEditorTreeItem<T>* find = m_Parent->m_vecChild[i]->FindItem(item);
			if (find)
			{
				return i;
			}
		}
		return -1;
	}
	void Clear()
	{
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			m_vecChild[i]->Clear();
			SAFE_DELETE(m_vecChild[i]);
		}
		m_vecChild.clear();
	}
	void Render()
	{
		ImGuiTreeNodeFlags	flag = m_Flag;
		if (m_vecChild.empty())
		{
			flag |= ImGuiTreeNodeFlags_Leaf;
			//flag |= ImGuiTreeNodeFlags_Framed;
			//flag |= ImGuiTreeNodeFlags_Selected;
		}
		bool	itemOpen = ImGui::TreeNodeEx(m_ItemUTF8.c_str(), flag);
		if (itemOpen)
		{
			if (ImGui::IsItemHovered(ImGuiHoveredFlags_::ImGuiHoveredFlags_RectOnly))
			{
				if(m_Parent)
				{
					m_Parent->m_HoverItem = this;
				}
				else
				{
					m_HoverItem = this;
				}	
			}
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				if (m_SelectCallback)
				{
					m_SelectCallback(this, m_Item);
				}
			}
// 노드의 더블클릭체크
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (m_DoubleClickCallback)
				{
					m_DoubleClickCallback(this, m_Item);
				}
			}
			if (m_UseDragDropSelf || m_UseDragDropOuter)
			{
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload(m_WindowName.c_str(), this, sizeof(this));
					ImGui::Text(m_Item.c_str());
					ImGui::EndDragDropSource();
					if (m_Parent)
					{
						m_Parent->m_DragItem = this;
					}
					else
					{
						m_DragItem = this;
					}
				}
// 내부 드롭을 허용한 경우에만 드롭체크
				if (m_UseDragDropSelf)
				{
					if (ImGui::BeginDragDropTarget())
					{
						const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_WindowName.c_str());
						if (nullptr != payload)
						{
							if (m_Parent)
							{
								if (m_Parent->m_DragItem && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
								{
									if (m_DragAndDropCallback)
									{
										m_DragAndDropCallback(m_Parent->m_DragItem, this,
											m_Parent->m_DragItem->m_Item, m_Item);
									}
									m_Parent->m_DragItem = nullptr;
								}
							}
							else   //리스트가 1개인 경우
							{
								if (m_DragItem && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
								{
									if (m_DragAndDropCallback)
									{
										m_DragAndDropCallback(m_DragItem, this, m_DragItem->m_Item, m_Item);
									}
									m_DragItem = nullptr;
								}
							}
						}
						ImGui::EndDragDropTarget();
					}
					if (m_Parent)
					{
						if (!m_Parent->m_vecChild.empty() && this == m_Parent->m_vecChild[m_Parent->m_vecChild.size() - 1] &&
							ImGui::IsMouseReleased(ImGuiMouseButton_Left))
						{
							if (m_Parent->m_DragItem)
							{
								if (m_DragAndDropCallback)
								{
									m_DragAndDropCallback(m_Parent->m_DragItem, nullptr, m_Parent->m_DragItem->m_Item, "");
								}
								m_Parent->m_DragItem = nullptr;
							}
						}
					}
					else
					{
						if (!m_vecChild.empty() && this == m_vecChild[m_vecChild.size() - 1] &&
							ImGui::IsMouseReleased(ImGuiMouseButton_Left))
						{
							if (m_DragItem)
							{
								if (m_DragAndDropCallback)
								{
									m_DragAndDropCallback(m_DragItem, nullptr, m_DragItem->m_Item, "");
								}
								m_DragItem = nullptr;
							}
						}
					}
				}
			}
			size_t	widgetCount = m_vecWidget.size();
			for (size_t i = 0; i < widgetCount; ++i)
			{
				m_vecWidget[i]->Render();
			}
			size_t	size = m_vecChild.size();
			for (size_t i = 0; i < size; ++i)
			{
				m_vecChild[i]->Render();
			}
			ImGui::TreePop();
		}
	}
	template <typename CallbackType>
	void SetSelectCallback(CallbackType* obj, void (CallbackType::* func)(CEditorTreeItem<T>*, const std::string&))
	{
		m_SelectCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
	template <typename CallbackType>
	void SetDoubleClickCallback(CallbackType* obj, void (CallbackType::* func)(CEditorTreeItem<T>*, const std::string&))
	{
		m_DoubleClickCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
	}
	template <typename CallbackType>
	void SetDragAndDropCallback(CallbackType* obj, void(CallbackType::* func)(CEditorTreeItem<T>*, CEditorTreeItem<T>*,
		const std::string&, const std::string&))
	{
		m_DragAndDropCallback = std::bind(func, obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
			, std::placeholders::_4);
	}
	template <typename WidgetType>
	WidgetType* CreateWidget(const std::string& name, float width = 100.f, float height = 100.f)
	{
		WidgetType* widget = new WidgetType;
		widget->SetName(name);
		widget->SetSize(width, height);
		if (!widget->Init())
		{
			SAFE_DELETE(widget);
			return nullptr;
		}
		m_vecWidget.push_back(widget);
		return widget;
	}
	template <typename WidgetType>
	WidgetType* FindWidget(const std::string& name)
	{
		size_t	size = m_vecWidget.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (m_vecWidget[i]->GetName() == name)
			{
				return (WidgetType*)m_vecWidget[i];
			}
		}
		return nullptr;
	}
	template <typename WidgetType>
	WidgetType* FindWidgetHirearchy(const std::string& name)
	{
		WidgetType* widget = FindWidget<WidgetType>(name);
		if (widget)
		{
			return widget;
		}
		size_t	size = m_vecChild.size();
		for (size_t i = 0; i < size; ++i)
		{
			widget = m_vecChild[i]->FindWidgetHirearchy<WidgetType>(name);
			if (widget)
			{
				return widget;
			}
		}
		return nullptr;
	}
};