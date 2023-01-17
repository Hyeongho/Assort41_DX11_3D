﻿
#include "EditorWindow.h"
#include "EditorWidget.h"
#include "EditorGUIManager.h"

/*
ImGuiWindowFlags_None                   = 0,
ImGuiWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
ImGuiWindowFlags_NoResize               = 1 << 1,   // Disable user resizing with the lower-right grip
ImGuiWindowFlags_NoMove                 = 1 << 2,   // Disable user moving the window
ImGuiWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
ImGuiWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window to its content every frame
ImGuiWindowFlags_NoBackground           = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save settings in .ini file
ImGuiWindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mouse, hovering test with pass through.
ImGuiWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
ImGuiWindowFlags_NoNavInputs            = 1 << 18,  // No gamepad/keyboard navigation within the window
ImGuiWindowFlags_NoNavFocus             = 1 << 19,  // No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
ImGuiWindowFlags_UnsavedDocument        = 1 << 20,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
ImGuiWindowFlags_NoDocking              = 1 << 21,  // Disable docking of this window

ImGuiWindowFlags_NoNav                  = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,
ImGuiWindowFlags_NoDecoration           = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse,
ImGuiWindowFlags_NoInputs               = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

// [Internal]
ImGuiWindowFlags_NavFlattened           = 1 << 23,  // [BETA] On child window: allow gamepad/keyboard navigation to cross over parent border to this child or between sibling child windows.
ImGuiWindowFlags_ChildWindow            = 1 << 24,  // Don't use! For internal use by BeginChild()
ImGuiWindowFlags_Tooltip                = 1 << 25,  // Don't use! For internal use by BeginTooltip()
ImGuiWindowFlags_Popup                  = 1 << 26,  // Don't use! For internal use by BeginPopup()
ImGuiWindowFlags_Modal                  = 1 << 27,  // Don't use! For internal use by BeginPopupModal()
ImGuiWindowFlags_ChildMenu              = 1 << 28,  // Don't use! For internal use by BeginMenu()
ImGuiWindowFlags_DockNodeHost           = 1 << 29,
*/

CEditorWindow::CEditorWindow()	:
	m_WindowFlag(ImGuiWindowFlags_NoCollapse),
	m_Open(true)
{
}

CEditorWindow::~CEditorWindow()
{
	size_t	Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecWidget[i]);
	}
}

void CEditorWindow::DeleteAllWidget()
{
	size_t	Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecWidget[i]);
	}

	m_vecWidget.clear();
}

void CEditorWindow::ClearWidget()
{
	m_vecWidget.clear();
}

void CEditorWindow::Open()
{
	m_Open = true;
}

void CEditorWindow::Close()
{
	m_Open = false;
}

CEditorWidget* CEditorWindow::FindWidget(const std::string& Name)
{
	size_t	Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecWidget[i]->GetName() == Name)
			return m_vecWidget[i];
	}

	return nullptr;
}

bool CEditorWindow::Init()
{
	return true;
}

void CEditorWindow::Update(float DeltaTime)
{
	if (!m_Open)
		return;

	if (!ImGui::Begin(m_Name.c_str(), &m_Open, m_WindowFlag))
		m_Open = false;

	CEditorGUIManager::GetInst()->EnableFont();

	size_t	Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecWidget[i]->Render();
	}

	CEditorGUIManager::GetInst()->DisableFont();

	ImGui::End();
}
