#include "EditorSlider.h"

CEditorSlider::CEditorSlider()
	: m_Position{}
	, m_PositionMin(0.f)
	, m_PositionMax(255.f)
{
}

CEditorSlider::~CEditorSlider()
{
}

void CEditorSlider::SetText(const std::string& text)
{
	TCHAR	wText[256] = {};
	int length = (int)MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, text.c_str(), -1, wText, length);
	char	textUTF8[256] = {};
	length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_UTF8, 0, wText, -1, textUTF8, length, nullptr, nullptr);
	m_Text = text;
	m_wText = wText;
	m_TextUTF8 = textUTF8;
}

void CEditorSlider::AddText(const std::string& text)
{
	SetText(m_Text + text);
}

bool CEditorSlider::Init()
{
	m_Position[0] = m_PositionMax;
	m_Position[1] = m_PositionMax;
	m_Position[2] = m_PositionMax;
	m_Position[3] = m_PositionMax;
	return true;
}

void CEditorSlider::Render()
{
	ImGui::SliderFloat4(m_TextUTF8.c_str(), m_Position, m_PositionMin, m_PositionMax);
	if (m_SliderCallback)
	{
		m_SliderCallback();
	}
}
