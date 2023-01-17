
#include "EditorButton.h"

CEditorButton::CEditorButton()
{
}

CEditorButton::~CEditorButton()
{
}

bool CEditorButton::Init()
{
	return true;
}

void CEditorButton::Render()
{
	m_Click = ImGui::Button(m_NameUTF8.c_str(), m_Size);

	if (m_Click && m_ClickCallback)
		m_ClickCallback();
}
