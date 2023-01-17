#include "EditorGroup.h"

CEditorGroup::CEditorGroup()
{
}

CEditorGroup::~CEditorGroup()
{
    size_t  Size = m_vecWidget.size();

    for (size_t i = 0; i < Size; ++i)
    {
        SAFE_DELETE(m_vecWidget[i]);
    }

    m_vecWidget.clear();
}

bool CEditorGroup::Init()
{
    return true;
}

void CEditorGroup::Render()
{
    ImGui::BeginGroup();

    size_t  Size = m_vecWidget.size();

    for (size_t i = 0; i < Size; ++i)
    {
        m_vecWidget[i]->Render();
    }

    ImGui::EndGroup();
}
