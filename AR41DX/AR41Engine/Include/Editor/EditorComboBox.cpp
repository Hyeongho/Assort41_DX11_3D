
#include "EditorComboBox.h"

CEditorComboBox::CEditorComboBox() :
	m_Select(false),
	m_SelectPrevViewName(false),
	m_SelectIndex(-1),
	m_PageItemCount(3),
	m_Sort(false),
	m_Flag(0)
{
}

CEditorComboBox::~CEditorComboBox()
{
}

void CEditorComboBox::AddItem(const std::string& Item)
{
	TCHAR	wText[256] = {};

	int Length = (int)MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, nullptr, 0);

	MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, wText, Length);

	char	TextUTF8[256] = {};

	Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, nullptr, nullptr);

	m_vecItem.push_back(Item);
	m_vecItemUTF8.push_back(TextUTF8);
}

void CEditorComboBox::DeleteItem(const std::string& Item)
{
	size_t	Size = m_vecItem.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecItem[i] == Item)
		{
			DeleteItem((int)i);
			break;
		}
	}
}

void CEditorComboBox::DeleteItem(int Index)
{
	auto	iter = m_vecItem.begin() + Index;

	m_vecItem.erase(iter);

	iter = m_vecItemUTF8.begin() + Index;

	m_vecItemUTF8.erase(iter);
}

void CEditorComboBox::Sort(bool _Sort)
{
	m_Sort = _Sort;

	if (m_Sort)
	{
		std::sort(m_vecItem.begin(), m_vecItem.end());
		std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
	}
}

bool CEditorComboBox::Init()
{
	SetPrevViewName(m_Name);

	return true;
}

void CEditorComboBox::Render()
{
	ImGui::PushItemWidth(m_Size.x);

	ImVec2	Size;
	Size.x = m_Size.x;
	Size.y = m_PageItemCount * ImGui::GetTextLineHeightWithSpacing();

	if (ImGui::BeginCombo(m_Name.c_str(), m_PrevViewNameUTF8.c_str(), m_Flag))
	{
		// 목록 출력
		size_t	ItemCount = m_vecItemUTF8.size();

		m_Select = false;

		for (size_t i = 0; i < ItemCount; ++i)
		{
			bool	Select = false;

			if (ImGui::Selectable(m_vecItemUTF8[i].c_str(), &Select))
			{
				m_Select = true;

				if (m_SelectIndex != (int)i)
				{
					m_SelectIndex = (int)i;

					if (m_SelectCallback)
					{
						m_SelectCallback(m_SelectIndex, m_vecItem[i]);
					}
				}

				if (m_SelectPrevViewName)
				{
					m_PrevViewName = m_vecItem[i];
					m_PrevViewNameUTF8 = m_vecItemUTF8[i];
				}
			}

			if (Select)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}
