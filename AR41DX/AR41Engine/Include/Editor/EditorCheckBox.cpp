#include "EditorCheckBox.h"

CEditorCheckBox::CEditorCheckBox()  :
    m_Check(false)
{
}

CEditorCheckBox::~CEditorCheckBox()
{
}

bool CEditorCheckBox::Init()
{
    return true;
}

void CEditorCheckBox::Render()
{
    bool    Check = m_Check;

    ImGui::Checkbox(m_NameUTF8.c_str(), &Check);

    if (m_Check != Check)
    {
        m_Check = Check;

        if (m_CheckCallback)
            m_CheckCallback(Check);
    }
}
