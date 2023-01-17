
#include "EditorSameLine.h"

CEditorSameLine::CEditorSameLine()	:
	m_OffsetX(0.f),
	m_Spacing(-1.f)
{
}

CEditorSameLine::~CEditorSameLine()
{
}

bool CEditorSameLine::Init()
{
	return true;
}

void CEditorSameLine::Render()
{
	ImGui::SameLine(m_OffsetX, m_Spacing);
}
