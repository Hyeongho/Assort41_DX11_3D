
#include "EditorText.h"

CEditorText::CEditorText()
{
}

CEditorText::~CEditorText()
{
}

void CEditorText::SetText(const std::string& Text)
{
	TCHAR	wText[256] = {};

	int Length = (int)MultiByteToWideChar(CP_ACP, 0, Text.c_str(), -1, nullptr, 0);

	MultiByteToWideChar(CP_ACP, 0, Text.c_str(), -1, wText, Length);

	char	TextUTF8[256] = {};

	Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, nullptr, nullptr);

	m_Text = Text;
	m_wText = wText;
	m_TextUTF8 = TextUTF8;
}

void CEditorText::AddText(const std::string& Text)
{
	SetText(m_Text + Text);
}

bool CEditorText::Init()
{
	return true;
}

void CEditorText::Render()
{
	ImGui::TextColored(m_Color, m_TextUTF8.c_str());
}
