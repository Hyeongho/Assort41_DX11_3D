
#include "EditorInput.h"

CEditorInput::CEditorInput()	:
	m_Text{},
	m_wText{},
	m_TextUTF8{},
	m_HintText{},
	m_HintwText{},
	m_HintTextUTF8{},
	m_InputType(EImGuiInputType::String),
	m_MultiLine(false),
	m_Flag(0),
	m_ValueInt(0),
	m_ValueFloat(0.f)
{
}

CEditorInput::~CEditorInput()
{
}

void CEditorInput::SetText(const char* Text)
{
	memset(m_Text, 0, 1024);
	memset(m_wText, 0, 2048);
	memset(m_TextUTF8, 0, 1024);

	strcpy_s(m_Text, Text);

	int Length = (int)MultiByteToWideChar(CP_ACP, 0, Text, -1, nullptr, 0);

	MultiByteToWideChar(CP_ACP, 0, Text, -1, m_wText, Length);

	Length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_TextUTF8, Length, nullptr, nullptr);
}

void CEditorInput::SetHintText(const char* Text)
{
	memset(m_HintText, 0, 1024);
	memset(m_HintwText, 0, 2048);
	memset(m_HintTextUTF8, 0, 1024);

	strcpy_s(m_HintText, Text);

	int Length = (int)MultiByteToWideChar(CP_ACP, 0, Text, -1, nullptr, 0);

	MultiByteToWideChar(CP_ACP, 0, Text, -1, m_HintwText, Length);

	Length = WideCharToMultiByte(CP_UTF8, 0, m_HintwText, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_UTF8, 0, m_HintwText, -1, m_HintTextUTF8, Length, nullptr, nullptr);
}

void CEditorInput::SetInt(int Value)
{
	m_ValueInt = Value;
}

void CEditorInput::SetFloat(float Value)
{
	m_ValueFloat = Value;
}

void CEditorInput::SetMultiLine(bool MultiLine)
{
	m_MultiLine = MultiLine;
}

void CEditorInput::AddFlag(ImGuiInputTextFlags_ Flag)
{
	m_Flag |= Flag;
	/*
	ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_CharsDecimal        = 1 << 0,   // Allow 0123456789.+-
	ImGuiInputTextFlags_CharsHexadecimal = 1 << 1,   // Allow 0123456789ABCDEFabcdef
	ImGuiInputTextFlags_CharsUppercase = 1 << 2,   // Turn a..z into A..Z
	ImGuiInputTextFlags_CharsNoBlank = 1 << 3,   // Filter out spaces, tabs
	ImGuiInputTextFlags_AutoSelectAll = 1 << 4,   // Select entire text when first taking mouse focus
	ImGuiInputTextFlags_EnterReturnsTrue = 1 << 5,   // Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
	ImGuiInputTextFlags_CallbackCompletion = 1 << 6,   // Callback on pressing TAB (for completion handling)
	ImGuiInputTextFlags_CallbackHistory = 1 << 7,   // Callback on pressing Up/Down arrows (for history handling)
	ImGuiInputTextFlags_CallbackAlways = 1 << 8,   // Callback on each iteration. User code may query cursor position, modify text buffer.
	ImGuiInputTextFlags_CallbackCharFilter = 1 << 9,   // Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
	ImGuiInputTextFlags_AllowTabInput = 1 << 10,  // Pressing TAB input a '\t' character into the text field
	ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
	ImGuiInputTextFlags_NoHorizontalScroll = 1 << 12,  // Disable following the cursor horizontally
	ImGuiInputTextFlags_AlwaysOverwrite = 1 << 13,  // Overwrite mode
	ImGuiInputTextFlags_ReadOnly = 1 << 14,  // Read-only mode
	ImGuiInputTextFlags_Password = 1 << 15,  // Password mode, display all characters as '*'
	ImGuiInputTextFlags_NoUndoRedo = 1 << 16,  // Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
	ImGuiInputTextFlags_CharsScientific = 1 << 17,  // Allow 0123456789.eE (Scientific notation input)
	ImGuiInputTextFlags_CallbackResize = 1 << 18,  // Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
	ImGuiInputTextFlags_CallbackEdit = 1 << 19,  // Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)
	*/
}

void CEditorInput::ReadOnly(bool Enable)
{
	if (Enable)
		m_Flag |= ImGuiInputTextFlags_ReadOnly;

	else if (m_Flag & ImGuiInputTextFlags_ReadOnly)
		m_Flag ^= ImGuiInputTextFlags_ReadOnly;
}

void CEditorInput::PasswordMode(bool Enable)
{
	if (Enable)
		m_Flag |= ImGuiInputTextFlags_Password;

	else if (m_Flag & ImGuiInputTextFlags_Password)
		m_Flag ^= ImGuiInputTextFlags_Password;
}

void CEditorInput::SetInputType(EImGuiInputType Type)
{
	m_InputType = Type;
}

bool CEditorInput::Empty() const
{
	return strlen(m_Text) == 0;
}

bool CEditorInput::Init()
{
	return true;
}

void CEditorInput::Render()
{
	ImGui::PushItemWidth(m_Size.x);

	// 입력이 되었는지 아닌지를 판단하는 변수
	bool	Input = false;

	if (m_MultiLine)
	{
		Input = ImGui::InputTextMultiline(m_NameUTF8.c_str(), m_TextUTF8, IM_ARRAYSIZE(m_TextUTF8), m_Size, m_Flag);
	}

	else
	{
		switch (m_InputType)
		{
		case EImGuiInputType::String:
			// HintText가 있을 경우와 없을 경우로 나눈다.
			if (strlen(m_HintText) > 0)
			{
				Input = ImGui::InputTextWithHint(m_Name.c_str(), m_HintTextUTF8,
					m_Text, IM_ARRAYSIZE(m_Text), m_Flag);
			}

			else
			{
				Input = ImGui::InputText(m_NameUTF8.c_str(), m_TextUTF8, IM_ARRAYSIZE(m_TextUTF8), m_Flag);
			}
			break;
		case EImGuiInputType::Int:
			Input = ImGui::InputInt(m_NameUTF8.c_str(), &m_ValueInt, 1, 10, m_Flag);
			break;
		case EImGuiInputType::Float:
			Input = ImGui::InputFloat(m_NameUTF8.c_str(), &m_ValueFloat, 0.f, 0.f, "%.5f", m_Flag);
			break;
		}
	}

	// 입력된게 있는지를 판단한다.
	if (Input)
	{
		// 문자를 입력받았을 경우 widechar와 utf8을 만들어준다.
		if (m_InputType == EImGuiInputType::String)
		{
			memset(m_Text, 0, 1024);
			memset(m_wText, 0, 2048);

			int Length = (int)MultiByteToWideChar(CP_ACP, 0, m_TextUTF8, -1, nullptr, 0);

			MultiByteToWideChar(CP_ACP, 0, m_TextUTF8, -1, m_wText, Length);

			Length = WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, nullptr, 0, nullptr, nullptr);

			WideCharToMultiByte(CP_UTF8, 0, m_wText, -1, m_Text, Length, nullptr, nullptr);
		}

		if (m_InputCallback)
			m_InputCallback();
	}


	ImGui::PopItemWidth();
}
