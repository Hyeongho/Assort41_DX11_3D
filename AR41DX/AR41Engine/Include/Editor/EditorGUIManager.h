#pragma once

#include "EditorInfo.h"
#include "EditorWindow.h"

class CEditorGUIManager
{
private:
	ImGuiContext* m_Context;
	std::unordered_map<std::string, CEditorWindow*>	m_mapWindow;
	ImFont* m_Font;

public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);
	void Render();

public:
	void SetFont(const std::string& Name, const char* FileName, float Size,
		int OverH = 2, int OverV = 1, float Spacing = 1.f, const std::string& PathName = FONT_PATH);
	void EnableFont();
	void DisableFont();

public:
	template <typename T>
	T* FindEditorWindow(const std::string& Name)
	{
		auto	iter = m_mapWindow.find(Name);

		if (iter == m_mapWindow.end())
			return nullptr;

		return (T*)iter->second;
	}

	template <typename T>
	T* CreateEditorWindow(const std::string& Name)
	{
		T* Window = FindEditorWindow<T>(Name);

		if (Window)
			return Window;

		Window = new T;

		Window->SetName(Name);

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_mapWindow.insert(std::make_pair(Name, Window));

		return Window;
	}

	DECLARE_SINGLE(CEditorGUIManager)
};

