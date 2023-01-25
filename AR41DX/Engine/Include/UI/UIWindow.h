#pragma once

#include "../Ref.h"
#include "UIWidget.h"

class CUIWindow :
    public CRef
{
    friend class CSceneViewport;
	friend class CScene;
	friend class CSceneManager;

protected:
    CUIWindow();
    CUIWindow(const CUIWindow& Window);
    virtual ~CUIWindow();

private:
	static std::unordered_map<std::string, CUIWindow*>	m_mapUIWindowCDO;

public:
	static void AddUIWindowCDO(const std::string& Name, CUIWindow* CDO)
	{
		m_mapUIWindowCDO.insert(std::make_pair(Name, CDO));
	}

	static CUIWindow* FindCDO(const std::string& Name)
	{
		auto	iter = m_mapUIWindowCDO.find(Name);

		if (iter == m_mapUIWindowCDO.end())
			return nullptr;

		return iter->second;
	}

protected:
	class CScene* m_Scene;
	class CSceneViewport* m_Owner;
    std::vector<CSharedPtr<CUIWidget>>	m_vecWidget;
    int     m_ZOrder;
	Vector2	m_Pos;
	Vector2	m_Size;
	bool	m_Start;
	std::string		m_WindowTypeName;

public:
	const std::string& GetWindowTypeName()	const
	{
		return m_WindowTypeName;
	}

public:
    int GetZOrder() const
    {
        return m_ZOrder;
    }

	class CSceneViewport* GetViewport()	const
	{
		return m_Owner;
	}

	int GetWidgetCount()	const
	{
		return (int)m_vecWidget.size();
	}

	const Vector2& GetPos()	const
	{
		return m_Pos;
	}

	const Vector2& GetSize()	const
	{
		return m_Size;
	}

public:
    void SetZOrder(int ZOrder)
    {
        m_ZOrder = ZOrder;
    }

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render();
    virtual CUIWindow* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
	CUIWidget* CollisionMouse(const Vector2& MousePos);
	void GetAllWidgetHierarchyName(std::vector<HierarchyWidgetName>& vecName);

private:
	static bool SortWidget(CSharedPtr<CUIWidget> Src, CSharedPtr<CUIWidget> Dest);
	static bool SortWidgetInv(CSharedPtr<CUIWidget> Src, CSharedPtr<CUIWidget> Dest);

public:
	template <typename T>
	T* FindWidget(const std::string& Name)
	{
		size_t	Size = m_vecWidget.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecWidget[i]->GetName() == Name)
				return (T*)m_vecWidget[i].Get();
		}

		return nullptr;
	}

	template <typename T>
	T* CreateWidget(const std::string& Name)
	{
		T* Widget = FindWidget<T>(Name);

		if (Widget)
			return Widget;

		Widget = new T;

		Widget->SetName(Name);
		Widget->m_Owner = this;
		Widget->m_Scene = m_Scene;

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		m_vecWidget.push_back((CUIWidget*)Widget);

		return Widget;
	}
};

