#include "SceneViewport.h"
#include "../GameObject/GameObject.h"
#include "Scene.h"
#include "../Input.h"

CSceneViewport::CSceneViewport()
{
}

CSceneViewport::~CSceneViewport()
{
}

void CSceneViewport::LoadComplete()
{
}

void CSceneViewport::Start()
{
	size_t	Size = m_vecWindow.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecWindow[i]->Start();
	}

	// ZOrder 순서에 따라 정렬한다.
}

bool CSceneViewport::Init()
{
	return true;
}

void CSceneViewport::Update(float DeltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Update(DeltaTime);
		iter++;
	}
}

void CSceneViewport::PostUpdate(float DeltaTime)
{
	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		iter++;
	}
}

void CSceneViewport::Render()
{
	if (m_vecWindow.size() >= 2)
		std::sort(m_vecWindow.begin(), m_vecWindow.end(), CSceneViewport::SortWindow);

	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		else if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Render();
		iter++;
	}
}

void CSceneViewport::Save(FILE* File)
{
	int	Count = (int)m_vecWindow.size();

	fwrite(&Count, sizeof(int), 1, File);

	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd; iter++)
	{
		std::string	TypeName = (*iter)->GetWindowTypeName();

		int	Length = (int)TypeName.length();
		fwrite(&Length, sizeof(int), 1, File);
		fwrite(TypeName.c_str(), 1, Length, File);

		(*iter)->Save(File);
	}
}

void CSceneViewport::Load(FILE* File)
{
	int	Count = 0;

	fread(&Count, sizeof(int), 1, File);

	for (int i = 0; i < Count; ++i)
	{
		char	TypeName[256] = {};
		int	Length = 0;

		fread(&Length, sizeof(int), 1, File);
		fread(TypeName, 1, Length, File);

		CUIWindow* CDO = CUIWindow::FindCDO(TypeName);

		CUIWindow* Window = CDO->Clone();

		Window->m_Owner = this;
		Window->m_Scene = m_Owner;

		Window->Load(File);

		m_vecWindow.push_back(Window);
	}
}

bool CSceneViewport::CollisionMouse()
{
	if (m_vecWindow.size() >= 2)
		std::sort(m_vecWindow.begin(), m_vecWindow.end(), CSceneViewport::SortWindowInv);

	if (m_CollisionWidget && !m_CollisionWidget->GetActive())
		m_CollisionWidget = nullptr;

	// 마우스 위치를 얻어온다.
	Vector2	MousePos = CInput::GetInst()->GetMousePos();

	auto	iter = m_vecWindow.begin();
	auto	iterEnd = m_vecWindow.end();

	bool	Result = false;

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		CUIWidget* Widget = (*iter)->CollisionMouse(MousePos);

		if (m_CollisionWidget && m_CollisionWidget != Widget)
			m_CollisionWidget->m_MouseHovered = false;

		if (Widget)
			m_CollisionWidget = Widget;

		if (Widget)
		{
			Result = true;
			break;
		}
	}

	// 충돌이 안되었다면
	if (!Result)
	{
		if (m_CollisionWidget)
			m_CollisionWidget->m_MouseHovered = false;

		m_CollisionWidget = nullptr;
	}

	iter = m_vecWindow.begin();
	iterEnd = m_vecWindow.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWindow.erase(iter);
			iterEnd = m_vecWindow.end();
			continue;
		}

		iter++;
	}

	return Result;
}

void CSceneViewport::GetAllWindowHierarchyName(std::vector<HierarchyWindowName>& vecName)
{
	size_t size = m_vecWindow.size();
	for (size_t i = 0; i < size; ++i)
	{
		HierarchyWindowName	names;
		names.name = m_vecWindow[i]->GetName();
		names.className = m_vecWindow[i]->GetWindowTypeName();
		if (names.className == "UIWindow")
		{
			continue;
		}
		if (names.name == "")
		{
			names.name = names.className;
		}
		names.window = m_vecWindow[i];
		vecName.push_back(names);
	}
}

bool CSceneViewport::SortWindow(CSharedPtr<CUIWindow> Src, CSharedPtr<CUIWindow> Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}

bool CSceneViewport::SortWindowInv(CSharedPtr<CUIWindow> Src, CSharedPtr<CUIWindow> Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}
