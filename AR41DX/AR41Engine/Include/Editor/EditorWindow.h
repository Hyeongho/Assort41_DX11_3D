#pragma once

#include "EditorInfo.h"

class CEditorWindow
{
	friend class CEditorGUIManager;

protected:
	CEditorWindow();
	virtual ~CEditorWindow();

protected:
	std::string		m_Name;
	bool			m_Open;
	int				m_WindowFlag;
	std::vector<class CEditorWidget*>	m_vecWidget;

public:
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	void AddWindowFlag(ImGuiWindowFlags_ Flag)
	{
		m_WindowFlag |= Flag;
	}

public:
	void DeleteAllWidget();
	void ClearWidget();
	void AddWidget(CEditorWidget* Widget)
	{
		m_vecWidget.push_back(Widget);
	}
	void Open();
	void Close();
	class CEditorWidget* FindWidget(const std::string& Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	template <typename T>
	T* CreateWidget(const std::string& Name, float Width = 100.f, float Height = 100.f)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->m_Owner = this;
		Widget->SetSize(Width, Height);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_vecWidget.push_back(Widget);

		return Widget;
	}

	template <typename T>
	T* CreateWidgetEmpty(const std::string& Name, float Width = 100.f, float Height = 100.f)
	{
		T* Widget = new T;

		Widget->SetName(Name);
		Widget->m_Owner = this;
		Widget->SetSize(Width, Height);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		return Widget;
	}
};

