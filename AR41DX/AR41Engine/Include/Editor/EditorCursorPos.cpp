#include "EditorCursorPos.h"

CEditorCursorPos::CEditorCursorPos()
{
}

CEditorCursorPos::~CEditorCursorPos()
{
}

bool CEditorCursorPos::Init()
{
	return true;
}

void CEditorCursorPos::Render()
{
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + m_Pos.x);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_Pos.y);
}
