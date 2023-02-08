#include "CameraWindow.h"
#include "Device.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorComboBox.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"

CCameraWindow::CCameraWindow()
	: m_CameraCBIndex(0)
	, m_CameraCB(nullptr)
	, m_CameraViewDistance(nullptr)
{
}

CCameraWindow::~CCameraWindow()
{
}

void CCameraWindow::SetSelectComponent(CSceneComponent* component)
{
	m_SelectCamera = (CCameraComponent*)component;
	int index = (int)m_SelectCamera->GetCameraType();
	std::string type= m_CameraCB->GetItem(index);
	m_CameraCB->SetPrevViewName(type);
	m_CameraCB->SetSelectIndex(index);
	m_CameraViewDistance->SetFloat(m_SelectCamera->GetCameraViewDistance());
}

bool CCameraWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("카메라");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CCameraWindow>(this, &CCameraWindow::CameraChangeCallback);
	m_CameraCB = CreateWidget<CEditorComboBox>("CameraType",110.f);
	m_CameraCB->SetPrevViewName("Camera2D");
	m_CameraCB->AddItem("Camera2D");
	m_CameraCB->AddItem("Camera3D");
	m_CameraCB->AddItem("CameraUI");
	m_CameraCB->SetSelectPrevViewName(true);
	m_CameraCB->SetSelectCallback<CCameraWindow>(this, &CCameraWindow::CameraCBClickCallback);
	m_CameraViewDistance = CreateWidget<CEditorInput>("CameraViewDistance", 90.f, 30.f);
	m_CameraViewDistance->SetInputType(EImGuiInputType::Float);
	return true;
}

void CCameraWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectCamera)
	{
		if (!m_SelectCamera->GetActive())
		{
			m_SelectCamera = nullptr;
		}
	}
}

void CCameraWindow::CameraChangeCallback()
{
	if (!m_SelectCamera)
	{
		return;
	}
	float size = m_CameraViewDistance->GetFloat();
	m_SelectCamera->SetCameraViewDistance(size);
	m_SelectCamera->SetCameraType((ECameraType)m_CameraCBIndex);
}

void CCameraWindow::CameraCBClickCallback(int index, const std::string& item)
{
	m_CameraCBIndex = index;
}
