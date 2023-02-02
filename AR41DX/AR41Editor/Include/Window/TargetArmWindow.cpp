#include "TargetArmWindow.h"
#include "Device.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorCheckBox.h"
#include "Component/TargetArm.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CTargetArmWindow::CTargetArmWindow()
	: m_TargetArmCBIndex(0)
	, m_TargetArmAxisCB(nullptr)
	, m_TargetDistance(nullptr)
	, m_WheelSensitivity(nullptr)
	, m_WheelTickMove(nullptr)
	, m_WheelZoomMin(nullptr)
	, m_WheelZoomMax(nullptr)
	, m_WheelZoomInOutEnable(nullptr)
	, m_TargetArmOffset{}
{
}

CTargetArmWindow::~CTargetArmWindow()
{
}

void CTargetArmWindow::SetSelectComponent(CTargetArm* component)
{
	m_SelectArm = (CTargetArm*)component;
	m_TargetArmOffset[0]->SetFloat(m_SelectArm->GetTargetOffset().x);
	m_TargetArmOffset[1]->SetFloat(m_SelectArm->GetTargetOffset().y);
	m_TargetArmOffset[2]->SetFloat(m_SelectArm->GetTargetOffset().z);
	int index = (int)m_SelectArm->GetTargetDistanceAxis();
	std::string type = m_TargetArmAxisCB->GetItem(index);
	m_TargetArmAxisCB->SetPrevViewName(type);
	m_TargetArmAxisCB->SetSelectIndex(index);
	m_TargetDistance->SetFloat(m_SelectArm->GetTargetDistance());
	m_WheelSensitivity->SetFloat(m_SelectArm->GetWheelSensitivity());
	m_WheelTickMove->SetFloat(m_SelectArm->GetWheelTickMove());
	m_WheelZoomMin->SetFloat(m_SelectArm->GetWheelZoomMin());
	m_WheelZoomMax->SetFloat(m_SelectArm->GetWheelZoomMax());
	m_WheelZoomInOutEnable->SetCheck(m_SelectArm->GetWheelZoomInOutEnable());
}

bool CTargetArmWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("≈∏∞Ÿæœ");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("≈∏∞Ÿæœ ∫Ø∞Ê", 80.f, 30.f);
	button->SetClickCallback<CTargetArmWindow>(this, &CTargetArmWindow::TargetArmChangeCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	m_WheelZoomInOutEnable = CreateWidget<CEditorCheckBox>("¡‹¿Œ »∞º∫»≠");
	label = CreateWidget<CEditorLabel>("TargetOffset");
	label->SetColor(0, 0, 255, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	m_TargetArmOffset[0] = CreateWidget<CEditorInput>("TargetOffsetX", 80.f, 30.f);
	m_TargetArmOffset[0]->SetInputType(EImGuiInputType::Float);
	m_TargetArmOffset[0]->SetHideName("TargetOffsetX");
	line = CreateWidget<CEditorSameLine>("Line");
	m_TargetArmOffset[1] = CreateWidget<CEditorInput>("TargetOffsetY", 80.f, 30.f);
	m_TargetArmOffset[1]->SetInputType(EImGuiInputType::Float);
	m_TargetArmOffset[1]->SetHideName("TargetOffsetY");
	line = CreateWidget<CEditorSameLine>("Line");
	m_TargetArmOffset[2] = CreateWidget<CEditorInput>("TargetOffsetZ", 80.f, 30.f);
	m_TargetArmOffset[2]->SetInputType(EImGuiInputType::Float);
	m_TargetArmOffset[2]->SetHideName("TargetOffsetZ");
	m_TargetArmAxisCB = CreateWidget<CEditorComboBox>("TargetDistanceAxis", 100.f);
	m_TargetArmAxisCB->SetPrevViewName("AXIS_X");
	m_TargetArmAxisCB->AddItem("AXIS_X");
	m_TargetArmAxisCB->AddItem("AXIS_Y");
	m_TargetArmAxisCB->AddItem("AXIS_Z");
	m_TargetArmAxisCB->AddItem("AXIS_MAX");
	m_TargetArmAxisCB->SetSelectPrevViewName(true);
	m_TargetArmAxisCB->SetSelectCallback<CTargetArmWindow>(this, &CTargetArmWindow::TargetArmCBClickCallback);
	m_TargetDistance = CreateWidget<CEditorInput>("TargetDistance", 80.f, 30.f);
	m_TargetDistance->SetInputType(EImGuiInputType::Float);
	m_WheelSensitivity = CreateWidget<CEditorInput>("WheelSensitivity", 80.f, 30.f);
	m_WheelSensitivity->SetInputType(EImGuiInputType::Float);
	m_WheelTickMove = CreateWidget<CEditorInput>("WheelTickMove", 80.f, 30.f);
	m_WheelTickMove->SetInputType(EImGuiInputType::Float);
	m_WheelZoomMin = CreateWidget<CEditorInput>("WheelZoomMin", 80.f, 30.f);
	m_WheelZoomMin->SetInputType(EImGuiInputType::Float);
	m_WheelZoomMax = CreateWidget<CEditorInput>("WheelZoomMax", 80.f, 30.f);
	m_WheelZoomMax->SetInputType(EImGuiInputType::Float);
	return true;
}

void CTargetArmWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectArm)
	{
		if (!m_SelectArm->GetActive())
		{
			m_SelectArm = nullptr;
		}
	}
}

void CTargetArmWindow::TargetArmChangeCallback()
{
	if (!m_SelectArm)
	{
		return;
	}
	m_SelectArm->SetTargetOffset(
		m_TargetArmOffset[0]->GetFloat(), m_TargetArmOffset[1]->GetFloat(), m_TargetArmOffset[2]->GetFloat());
	m_SelectArm->SetTargetDistanceAxis((AXIS)m_TargetArmCBIndex);
	float size = m_TargetDistance->GetFloat();
	m_SelectArm->SetTargetDistance(size);
	m_SelectArm->SetMouseWheelSensitivity(m_WheelSensitivity->GetFloat());
	m_SelectArm->SetWheelTickMove(m_WheelTickMove->GetFloat());
	m_SelectArm->SetZoomMin(m_WheelZoomMin->GetFloat());
	m_SelectArm->SetZoomMax(m_WheelZoomMax->GetFloat());
	m_SelectArm->SetWheelZoomInOutEnable(m_WheelZoomInOutEnable->GetCheck());
}

void CTargetArmWindow::TargetArmCBClickCallback(int index, const std::string& item)
{
	m_TargetArmCBIndex = index;
}