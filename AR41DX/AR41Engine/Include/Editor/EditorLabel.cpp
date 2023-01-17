
#include "EditorLabel.h"

CEditorLabel::CEditorLabel()	:
	m_Align{}
{
}

CEditorLabel::~CEditorLabel()
{
}

bool CEditorLabel::Init()
{
	return true;
}

void CEditorLabel::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, m_Align);

	ImGui::PushStyleColor(ImGuiCol_Button, m_Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_Color);

	ImGui::Button(m_NameUTF8.c_str(), m_Size);

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
}
