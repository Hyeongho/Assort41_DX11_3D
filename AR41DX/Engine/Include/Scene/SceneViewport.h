#pragma once

#include "../EngineInfo.h"
#include "../UI/UIWindow.h"
#include "../UI/UIWidget.h"

class CSceneViewport
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CSceneViewport();
	~CSceneViewport();

protected:
	class CScene* m_Owner;
	std::vector<CSharedPtr<CUIWindow>>	m_vecWindow;
	CSharedPtr<class CUIWidget>	m_CollisionWidget;
	bool		m_Start;
public:
	void LoadComplete();

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void Render();
	void Save(FILE* File);
	void Load(FILE* File);
	bool CollisionMouse();
	void GetAllWindowHierarchyName(std::vector<HierarchyWindowName>& vecName);

private:
	static bool SortWindow(CSharedPtr<CUIWindow> Src, CSharedPtr<CUIWindow> Dest);
	static bool SortWindowInv(CSharedPtr<CUIWindow> Src, CSharedPtr<CUIWindow> Dest);

public:
	template <typename T>
	T* FindUIWindow(const std::string& Name)
	{
		size_t	Size = m_vecWindow.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecWindow[i]->GetWindowTypeName() == Name)
				return (T*)m_vecWindow[i].Get();
		}

		return nullptr;
	}

	template <typename T>
	T* CreateUIWindow(const std::string& Name)
	{
		T* Window = FindUIWindow<T>(Name);

		if (Window)
			return Window;

		Window = new T;

		Window->SetName(Name);
		Window->m_Owner = this;
		Window->m_Scene = m_Owner;

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_vecWindow.push_back((CUIWindow*)Window);
		if (m_Start)
		{
			Window->Start();
		}
		return Window;
	}
};

