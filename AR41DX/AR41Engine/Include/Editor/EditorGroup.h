#pragma once

#include "EditorWidget.h"

class CEditorGroup :
    public CEditorWidget
{
	friend class CEditorWindow;

	template <typename T>
	friend class CEditorTreeItem;

protected:
	CEditorGroup();
	virtual ~CEditorGroup();

private:
	std::vector<CEditorWidget*>	m_vecWidget;

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	T* CreateWidget(const std::string& Name, float Width = 100.f, float Height = 100.f)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->m_Owner = m_Owner;
		Widget->SetSize(Width, Height);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_vecWidget.push_back(Widget);

		return Widget;
	}
};

