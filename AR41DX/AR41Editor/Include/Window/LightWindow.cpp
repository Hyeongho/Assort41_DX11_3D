#include "LightWindow.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorListBox.h"
#include "Component/LightComponent.h"

CLightWindow::CLightWindow()
    : m_LightType(nullptr)
    , m_Color{}
    , m_Pos{}
    , m_Dir{}
    , m_Distance(nullptr)
    , m_AngleIn(nullptr)
    , m_AngleOut(nullptr)
    , m_Att1(nullptr)
    , m_Att2(nullptr)
    , m_Att3(nullptr)
    , m_Intensity(nullptr)
{
}

CLightWindow::~CLightWindow()
{
}

void CLightWindow::SetSelectComponent(CLightComponent* component)
{
    m_SelectComponent = component;
	int index = (int)m_SelectComponent->GetLightType();
	std::string name = m_LightType->GetItem(index);
	m_LightType->SetPrevViewName(name);
	m_Color[0]->SetFloat(m_SelectComponent->GetLightInfo().Color.x);
	m_Color[1]->SetFloat(m_SelectComponent->GetLightInfo().Color.y);
	m_Color[2]->SetFloat(m_SelectComponent->GetLightInfo().Color.z);
	m_Color[3]->SetFloat(m_SelectComponent->GetLightInfo().Color.w);
	m_Pos[0]->SetFloat(m_SelectComponent->GetLightInfo().Pos.x);
	m_Pos[1]->SetFloat(m_SelectComponent->GetLightInfo().Pos.y);
	m_Pos[2]->SetFloat(m_SelectComponent->GetLightInfo().Pos.z);
	m_Dir[0]->SetFloat(m_SelectComponent->GetLightInfo().Dir.x);
	m_Dir[1]->SetFloat(m_SelectComponent->GetLightInfo().Dir.y);
	m_Dir[2]->SetFloat(m_SelectComponent->GetLightInfo().Dir.z);
	m_Distance->SetFloat(m_SelectComponent->GetLightInfo().Distance);
	m_AngleIn->SetFloat(m_SelectComponent->GetLightInfo().AngleIn);
	m_AngleOut->SetFloat(m_SelectComponent->GetLightInfo().AngleOut);
	m_Att1->SetFloat(m_SelectComponent->GetLightInfo().Att1);
	m_Att2->SetFloat(m_SelectComponent->GetLightInfo().Att2);
	m_Att3->SetFloat(m_SelectComponent->GetLightInfo().Att3);
	m_Intensity->SetFloat(m_SelectComponent->GetLightInfo().Intensity);
}

bool CLightWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("라이트");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_LightType = CreateWidget<CEditorComboBox>("LgihtComboBox");
	m_LightType->SetHideName("LgihtComboBox");
	m_LightType->SetPrevViewName("Direction");
	m_LightType->AddItem("Direction");
	m_LightType->AddItem("Point");
	m_LightType->AddItem("Spot");
	m_LightType->SetSelectPrevViewName(true);
	//m_LightType->SetSelectCallback<CLightWindow>(this, &CLightWindow::ColiderClickCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CLightWindow>(this, &CLightWindow::LightChangeCallback);

	label = CreateWidget<CEditorLabel>("색상");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_Color[0] = CreateWidget<CEditorInput>("ColorX", 80.f, 30.f);
	m_Color[0]->SetHideName("ColorX");
	m_Color[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Color[1] = CreateWidget<CEditorInput>("ColorY", 80.f, 30.f);
	m_Color[1]->SetHideName("ColorY");
	m_Color[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Color[2] = CreateWidget<CEditorInput>("ColorZ", 80.f, 30.f);
	m_Color[2]->SetHideName("ColorZ");
	m_Color[2]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Color[3] = CreateWidget<CEditorInput>("ColorW", 80.f, 30.f);
	m_Color[3]->SetHideName("ColorW");
	m_Color[3]->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("위치");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_Pos[0] = CreateWidget<CEditorInput>("PosX", 80.f, 30.f);
	m_Pos[0]->SetHideName("PosX");
	m_Pos[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Pos[1] = CreateWidget<CEditorInput>("PosY", 80.f, 30.f);
	m_Pos[1]->SetHideName("PosY");
	m_Pos[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Pos[2] = CreateWidget<CEditorInput>("PosZ", 80.f, 30.f);
	m_Pos[2]->SetHideName("PosZ");
	m_Pos[2]->SetInputType(EImGuiInputType::Float);

	label = CreateWidget<CEditorLabel>("방향");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_Dir[0] = CreateWidget<CEditorInput>("DirX", 80.f, 30.f);
	m_Dir[0]->SetHideName("DirX");
	m_Dir[0]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Dir[1] = CreateWidget<CEditorInput>("DirY", 80.f, 30.f);
	m_Dir[1]->SetHideName("DirY");
	m_Dir[1]->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("line");
	m_Dir[2] = CreateWidget<CEditorInput>("DirZ", 80.f, 30.f);
	m_Dir[2]->SetHideName("DirZ");
	m_Dir[2]->SetInputType(EImGuiInputType::Float);

	m_Distance = CreateWidget<CEditorInput>("거리", 80.f, 30.f);
	m_Distance->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_AngleIn = CreateWidget<CEditorInput>("내각", 80.f, 30.f);
	m_AngleIn->SetInputType(EImGuiInputType::Float);
	m_AngleOut = CreateWidget<CEditorInput>("외각", 80.f, 30.f);
	m_AngleOut->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Intensity = CreateWidget<CEditorInput>("밝기", 80.f, 30.f);
	m_Intensity->SetInputType(EImGuiInputType::Float);
	m_Att1 = CreateWidget<CEditorInput>("속성1", 80.f, 30.f);
	m_Att1->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	m_Att2 = CreateWidget<CEditorInput>("속성2", 80.f, 30.f);
	m_Att2->SetInputType(EImGuiInputType::Float);
	m_Att3 = CreateWidget<CEditorInput>("속성3", 80.f, 30.f);
	m_Att3->SetInputType(EImGuiInputType::Float);
    return true;
}

void CLightWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
		}
	}
}

void CLightWindow::LightChangeCallback()
{
	if (!m_SelectComponent)
	{
		return;
	}
	m_SelectComponent->SetLightType((ELightType)m_LightType->GetSelectIndex());
	Vector4 color;
	color.x = m_Color[0]->GetFloat();
	color.y = m_Color[1]->GetFloat();
	color.z = m_Color[2]->GetFloat();
	color.w = m_Color[3]->GetFloat();
	m_SelectComponent->SetLightColor(color);
	Vector3 pos;
	pos.x = m_Pos[0]->GetFloat();
	pos.y = m_Pos[1]->GetFloat();
	pos.z = m_Pos[2]->GetFloat();
	m_SelectComponent->SetLightPos(pos);
	pos.x = m_Dir[0]->GetFloat();
	pos.y = m_Dir[1]->GetFloat();
	pos.z = m_Dir[2]->GetFloat();
	m_SelectComponent->SetLightDir(pos);
	m_SelectComponent->SetLightDistance(m_Distance->GetFloat());
	m_SelectComponent->SetLightAngleIn(m_AngleIn->GetFloat());
	m_SelectComponent->SetLightAngleOut(m_AngleOut->GetFloat());
	m_SelectComponent->SetLightAtt1(m_Att1->GetFloat());
	m_SelectComponent->SetLightAtt2(m_Att2->GetFloat());
	m_SelectComponent->SetLightAtt3(m_Att3->GetFloat());
	m_SelectComponent->SetLightIntensity(m_Intensity->GetFloat());
}
