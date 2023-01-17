
#include "TestWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"

CTestWindow::CTestWindow()	:
	m_AddText{},
	m_AddIndex(0),
	m_AddCount(0),
	m_AddTime(0.f)
{
}

CTestWindow::~CTestWindow()
{
}

bool CTestWindow::Init()
{
	CEditorButton* Btn = CreateWidget<CEditorButton>("버튼");

	Btn->SetClickCallback<CTestWindow>(this, &CTestWindow::TestButtonCallback);

	CEditorSameLine* SameLine = CreateWidget<CEditorSameLine>("Line");

	//SameLine->SetOffsetX(100.f);
	SameLine->SetSpacing(50.f);

	Btn = CreateWidget<CEditorButton>("TestButton1");

	Btn->SetClickCallback<CTestWindow>(this, &CTestWindow::TestButtonCallback);

	CEditorLabel* Label = CreateWidget<CEditorLabel>("흑마법사");

	Label->SetColor(50, 50, 50, 255);
	Label->SetAlign(0.5f, 0.5f);

	SameLine = CreateWidget<CEditorSameLine>("Line");

	CEditorImage* Image = CreateWidget<CEditorImage>("Image");

	Image->SetTexture("BlackMagicion", TEXT("teemo.png"));

	m_Text = CreateWidget<CEditorText>("Text");

	m_Text->SetText("작심하루");
	m_Text->SetColor(255, 0, 0, 255);

	strcpy_s(m_AddText, " Failed");

	m_AddCount = (int)strlen(m_AddText);

	m_Input = CreateWidget<CEditorInput>("Input");

	m_Input->SetHideName("Input");
	//m_Input->SetMultiLine(true);
	//m_Input->ReadOnly(true);
	//m_Input->PasswordMode(true);
	//m_Input->SetInputType(EImGuiInputType::Int);
	m_Input->SetInputType(EImGuiInputType::Float);

	m_Input->SetInputCallback<CTestWindow>(this, &CTestWindow::InputCallback);

	m_List = CreateWidget<CEditorListBox>("ListBox");

	m_List->SetHideName("ListBox");

	m_List->AddItem("List1");
	m_List->AddItem("List2");
	m_List->AddItem("List3");
	m_List->AddItem("List4");


	m_List->SetSelectCallback<CTestWindow>(this, &CTestWindow::ListCallback);

	m_Combo = CreateWidget<CEditorComboBox>("ComboBox");

	m_Combo->SetSelectPrevViewName(true);

	m_Combo->SetHideName("ComboBox");
	m_Combo->SetPrevViewName("흑마법사");

	m_Combo->AddItem("Combo1");
	m_Combo->AddItem("Combo2");
	m_Combo->AddItem("Combo3");
	m_Combo->AddItem("Combo4");


	m_Combo->SetSelectCallback<CTestWindow>(this, &CTestWindow::ListCallback);

	m_Tree = CreateWidget<CEditorTree<int>>("Tree");

	m_Tree->SetHideName("Tree");

	m_Tree->AddItem(0, "Root");
	m_Tree->AddItem(0, "진짜 최상위 부모");
	m_Tree->AddItem(0, "자식1", "진짜 최상위 부모");
	m_Tree->AddItem(0, "자식2", "Root");
	m_Tree->AddItem(0, "자식3", "자식2");


	m_Tree->SetSelectCallback<CTestWindow>(this, &CTestWindow::TreeCallback);

	return true;
}

void CTestWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);

	if (m_AddIndex < m_AddCount)
	{
		m_AddTime += DeltaTime;

		if (m_AddTime >= 1.f)
		{
			m_AddTime -= 1.f;

			char	Text[2] = {};
			Text[0] = m_AddText[m_AddIndex];

			m_Text->AddText(Text);

			++m_AddIndex;
		}
	}
}

void CTestWindow::TestButtonCallback()
{
	MessageBox(0, TEXT("Button"), TEXT("Button"), MB_OK);
}

void CTestWindow::InputCallback()
{
	char	Text[256] = {};

	sprintf_s(Text, "%.5f\n", m_Input->GetFloat());

	OutputDebugStringA(Text);
}

void CTestWindow::ListCallback(int SelectIndex, const std::string& Item)
{
	char	Text[256] = {};

	sprintf_s(Text, "%d : %s\n", SelectIndex, Item.c_str());

	OutputDebugStringA(Text);
}

void CTestWindow::TreeCallback(CEditorTreeItem<int>* Node, const std::string& Item)
{
	char	Text[256] = {};

	sprintf_s(Text, "%s\n", Item.c_str());

	OutputDebugStringA(Text);
}
