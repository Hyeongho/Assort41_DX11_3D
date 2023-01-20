#include "UIWindow.h"
#include "../Device.h"

std::unordered_map<std::string, CUIWindow*> CUIWindow::m_mapUIWindowCDO;

CUIWindow::CUIWindow()  :
    m_ZOrder(0),
	m_Start(false)
{
	m_WindowTypeName = "UIWindow";
	m_Size = Vector2((float)CDevice::GetInst()->GetResolution().Width,
		(float)CDevice::GetInst()->GetResolution().Height);
}

CUIWindow::CUIWindow(const CUIWindow& Window)	:
	CRef(Window)
{
    m_ZOrder = Window.m_ZOrder;
	m_Size = Window.m_Size;
	m_Start = false;
	m_WindowTypeName = Window.m_WindowTypeName;

	size_t	Size = Window.m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CUIWidget* Widget = Window.m_vecWidget[i]->Clone();

		Widget->m_Owner = this;

		m_vecWidget.push_back(Widget);
	}
}

CUIWindow::~CUIWindow()
{
}

void CUIWindow::Start()
{
	size_t	Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecWidget[i]->Start();
	}

	// ZOrder 순서에 따라 정렬한다.
}

bool CUIWindow::Init()
{
    return true;
}

void CUIWindow::Update(float DeltaTime)
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
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

void CUIWindow::PostUpdate(float DeltaTime)
{
	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
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

void CUIWindow::Render()
{
	if (m_vecWidget.size() >= 2)
		std::sort(m_vecWidget.begin(), m_vecWidget.end(), CUIWindow::SortWidget);

	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->GetActive())
		{
			iter = m_vecWidget.erase(iter);
			iterEnd = m_vecWidget.end();
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

CUIWindow* CUIWindow::Clone()
{
    return new CUIWindow(*this);
}

void CUIWindow::Save(FILE* File)
{
	fwrite(&m_ZOrder, sizeof(int), 1, File);
	fwrite(&m_Pos, sizeof(Vector2), 1, File);
	fwrite(&m_Size, sizeof(Vector2), 1, File);

	int	Count = (int)m_vecWidget.size();

	fwrite(&Count, sizeof(int), 1, File);
	
	for (int i = 0; i < Count; ++i)
	{
		std::string	TypeName = m_vecWidget[i]->GetWidgetTypeName();

		int	Length = (int)TypeName.length();
		fwrite(&Length, sizeof(int), 1, File);
		fwrite(TypeName.c_str(), 1, Length, File);

		m_vecWidget[i]->Save(File);
	}
}

void CUIWindow::Load(FILE* File)
{
	fread(&m_ZOrder, sizeof(int), 1, File);
	fread(&m_Pos, sizeof(Vector2), 1, File);
	fread(&m_Size, sizeof(Vector2), 1, File);

	int	Count = 0;

	fread(&Count, sizeof(int), 1, File);

	for (int i = 0; i < Count; ++i)
	{
		if (m_vecWidget.empty())
		{
			char	TypeName[256] = {};

			int	Length = 0;
			fread(&Length, sizeof(int), 1, File);
			fread(TypeName, 1, Length, File);

			CUIWidget* CDO = CUIWidget::FindCDO(TypeName);

			CUIWidget* Widget = CDO->Clone();

			Widget->m_Owner = this;
			Widget->m_Scene = m_Scene;

			Widget->Init();
			Widget->Load(File);

			m_vecWidget.push_back(Widget);
		}

		else
		{
			char	TypeName[256] = {};

			int	Length = 0;
			fread(&Length, sizeof(int), 1, File);
			fread(TypeName, 1, Length, File);

			m_vecWidget[i]->m_Owner = this;
			m_vecWidget[i]->m_Scene = m_Scene;

			m_vecWidget[i]->Init();
			m_vecWidget[i]->Load(File);
		}
	}

	size_t	Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecWidget[i]->m_Scene = m_Scene;
	}
}

CUIWidget* CUIWindow::CollisionMouse(const Vector2& MousePos)
{
	if (m_Pos.x > MousePos.x)
		return nullptr;

	else if (m_Pos.x + m_Size.x < MousePos.x)
		return nullptr;

	else if (m_Pos.y > MousePos.y)
		return nullptr;

	else if (m_Pos.y + m_Size.y < MousePos.y)
		return nullptr;

	if (m_vecWidget.size() >= 2)
		std::sort(m_vecWidget.begin(), m_vecWidget.end(), CUIWindow::SortWidgetInv);

	auto	iter = m_vecWidget.begin();
	auto	iterEnd = m_vecWidget.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		if ((*iter)->CollisionMouse(MousePos))
		{
			(*iter)->m_MouseHovered = true;
			return *iter;
		}

		else
			(*iter)->m_MouseHovered = false;
	}

	return nullptr;
}

bool CUIWindow::SortWidget(CSharedPtr<CUIWidget> Src, CSharedPtr<CUIWidget> Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}

bool CUIWindow::SortWidgetInv(CSharedPtr<CUIWidget> Src, CSharedPtr<CUIWidget> Dest)
{
	return Src->GetZOrder() < Dest->GetZOrder();
}
