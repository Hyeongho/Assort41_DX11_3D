#include "StatusWindow.h"
#include "Engine.h"
#include "Input.h"
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
//#include "Editor/EditorGizmo.h"
#include "Component/SceneComponent.h"
#include "UI/UIWidget.h"
#include "GameObject/GameObject.h"

CStatusWindow::CStatusWindow()
	: m_Check(nullptr)
	, m_Frame(nullptr)
	, m_PosX(nullptr)
	, m_PosY(nullptr)
	, m_PosZ(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
	, m_SizeZ(nullptr)
	, m_RayPos{}
	, m_RayDir{}
	, m_MousePos{}
	, m_MouseWorldPos{}
	, m_MouseUIPos{}
	, m_MouseMove{}
	, m_MouseMove2D{}
{
}

CStatusWindow::~CStatusWindow()
{
}

void CStatusWindow::SetSelectComponent(CSceneComponent* component)
{
	m_SelectComponent = component;
}

void CStatusWindow::SetSelectWidget(CUIWidget* widget)
{
	m_SelectWidget = widget;
}

bool CStatusWindow::Init()
{
	//프레임 반토막 나서 폐기
	//CEditorGizmo* gizmo = CreateWidget<CEditorGizmo>("기즈모");

	m_Check = CreateWidget<CEditorCheckBox>("활성화 버튼");
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("line");
	m_Frame = CreateWidget<CEditorInput>("프레임", 80.f, 30.f);
	m_Frame->SetInputType(EImGuiInputType::Float);
	m_Frame->ReadOnly(true);

	m_PosX = CreateWidget<CEditorInput>("X 위치", 80.f, 30.f);
	m_PosX->SetInputType(EImGuiInputType::Float);
	m_PosX->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_SizeX = CreateWidget<CEditorInput>("X 크기", 80.f, 30.f);
	m_SizeX->SetInputType(EImGuiInputType::Float);
	m_SizeX->ReadOnly(true);

	m_PosY = CreateWidget<CEditorInput>("Y 위치", 80.f, 30.f);
	m_PosY->SetInputType(EImGuiInputType::Float);
	m_PosY->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_SizeY = CreateWidget<CEditorInput>("Y 크기", 80.f, 30.f);
	m_SizeY->SetInputType(EImGuiInputType::Float);
	m_SizeY->ReadOnly(true);

	m_PosZ = CreateWidget<CEditorInput>("Z 위치", 80.f, 30.f);
	m_PosZ->SetInputType(EImGuiInputType::Float);
	m_PosZ->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_SizeZ = CreateWidget<CEditorInput>("Z 크기", 80.f, 30.f);
	m_SizeZ->SetInputType(EImGuiInputType::Float);
	m_SizeZ->ReadOnly(true);

	CEditorLabel* label = CreateWidget<CEditorLabel>("레이 위치");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_RayPos[0] = CreateWidget<CEditorInput>("RayPosX", 80.f, 30.f);
	m_RayPos[0]->SetHideName("RayPosX");
	m_RayPos[0]->SetInputType(EImGuiInputType::Float);
	m_RayPos[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_RayPos[1] = CreateWidget<CEditorInput>("RayPosY", 80.f, 30.f);
	m_RayPos[1]->SetHideName("RayPosY");
	m_RayPos[1]->SetInputType(EImGuiInputType::Float);
	m_RayPos[1]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_RayPos[2] = CreateWidget<CEditorInput>("RayPosZ", 80.f, 30.f);
	m_RayPos[2]->SetHideName("RayPosZ");
	m_RayPos[2]->SetInputType(EImGuiInputType::Float);
	m_RayPos[2]->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("레이 방향");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	m_RayDir[0] = CreateWidget<CEditorInput>("RayDirX", 80.f, 30.f);
	m_RayDir[0]->SetHideName("RayDirX");
	m_RayDir[0]->SetInputType(EImGuiInputType::Float);
	m_RayDir[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_RayDir[1] = CreateWidget<CEditorInput>("RayDirY", 80.f, 30.f);
	m_RayDir[1]->SetHideName("RayDirY");
	m_RayDir[1]->SetInputType(EImGuiInputType::Float);
	m_RayDir[1]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_RayDir[2] = CreateWidget<CEditorInput>("RayDirZ", 80.f, 30.f);
	m_RayDir[2]->SetHideName("RayDirZ");
	m_RayDir[2]->SetInputType(EImGuiInputType::Float);
	m_RayDir[2]->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("마우스 위치");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	m_MousePos[0] = CreateWidget<CEditorInput>("MousePosX", 80.f, 30.f);
	m_MousePos[0]->SetHideName("MousePosX");
	m_MousePos[0]->SetInputType(EImGuiInputType::Float);
	m_MousePos[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_MousePos[1] = CreateWidget<CEditorInput>("MousePosY", 80.f, 30.f);
	m_MousePos[1]->SetHideName("MousePosY");
	m_MousePos[1]->SetInputType(EImGuiInputType::Float);
	m_MousePos[1]->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("마우스 월드 위치");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	m_MouseWorldPos[0] = CreateWidget<CEditorInput>("MouseWorldPosX", 80.f, 30.f);
	m_MouseWorldPos[0]->SetHideName("MouseWorldPosX");
	m_MouseWorldPos[0]->SetInputType(EImGuiInputType::Float);
	m_MouseWorldPos[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_MouseWorldPos[1] = CreateWidget<CEditorInput>("MouseWorldPosY", 80.f, 30.f);
	m_MouseWorldPos[1]->SetHideName("MouseWorldPosY");
	m_MouseWorldPos[1]->SetInputType(EImGuiInputType::Float);
	m_MouseWorldPos[1]->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("마우스 UI 위치");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	m_MouseUIPos[0] = CreateWidget<CEditorInput>("MouseUIPosX", 80.f, 30.f);
	m_MouseUIPos[0]->SetHideName("MouseUIPosX");
	m_MouseUIPos[0]->SetInputType(EImGuiInputType::Float);
	m_MouseUIPos[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_MouseUIPos[1] = CreateWidget<CEditorInput>("MouseUIPosY", 80.f, 30.f);
	m_MouseUIPos[1]->SetHideName("MouseUIPosY");
	m_MouseUIPos[1]->SetInputType(EImGuiInputType::Float);
	m_MouseUIPos[1]->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("마우스 이동량");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	m_MouseMove[0] = CreateWidget<CEditorInput>("MouseMoveX", 80.f, 30.f);
	m_MouseMove[0]->SetHideName("MouseMoveX");
	m_MouseMove[0]->SetInputType(EImGuiInputType::Float);
	m_MouseMove[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_MouseMove[1] = CreateWidget<CEditorInput>("MouseMoveY", 80.f, 30.f);
	m_MouseMove[1]->SetHideName("MouseMoveY");
	m_MouseMove[1]->SetInputType(EImGuiInputType::Float);
	m_MouseMove[1]->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("마우스 이동량 2D");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(120.f, 30.f);
	m_MouseMove2D[0] = CreateWidget<CEditorInput>("MouseMove2DX", 80.f, 30.f);
	m_MouseMove2D[0]->SetHideName("MouseMove2DX");
	m_MouseMove2D[0]->SetInputType(EImGuiInputType::Float);
	m_MouseMove2D[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_MouseMove2D[1] = CreateWidget<CEditorInput>("MouseMove2DY", 80.f, 30.f);
	m_MouseMove2D[1]->SetHideName("MouseMove2DY");
	m_MouseMove2D[1]->SetInputType(EImGuiInputType::Float);
	m_MouseMove2D[1]->ReadOnly(true);
	return true;
}

void CStatusWindow::Update(float deltaTime)
{
	CEditorWindow::Update(deltaTime);
	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
		}
	}
	if (m_SelectWidget)
	{
		if (!m_SelectWidget->GetActive())
		{
			m_SelectWidget = nullptr;
		}
	}
	if(!m_Check->GetCheck())
	{
		return;
	}
	float FPS = CEngine::GetInst()->GetFPS();
	m_Frame->SetFloat(FPS);
	if (m_SelectComponent)
	{
		Vector3 pos=m_SelectComponent->GetWorldPos();
		m_PosX->SetFloat(pos.x);
		m_PosY->SetFloat(pos.y);
		m_PosZ->SetFloat(pos.z);
		Vector3 size = m_SelectComponent->GetWorldScale();
		m_SizeX->SetFloat(size.x);
		m_SizeY->SetFloat(size.y);
		m_SizeZ->SetFloat(size.z);
	}
	else if (m_SelectWidget)
	{
		Vector2 pos = m_SelectWidget->GetPos();
		m_PosX->SetFloat(pos.x);
		m_PosY->SetFloat(pos.y);
		m_PosZ->SetFloat(0.f);
		Vector2 size = m_SelectWidget->GetSize();
		m_SizeX->SetFloat(size.x);
		m_SizeY->SetFloat(size.y);
		m_SizeZ->SetFloat(0.f);
	}
	CInput* input = CInput::GetInst();
	Ray ray = input->GetRay();
	m_RayPos[0]->SetFloat(ray.Pos.x);
	m_RayPos[1]->SetFloat(ray.Pos.y);
	m_RayPos[2]->SetFloat(ray.Pos.z);
	m_RayDir[0]->SetFloat(ray.Dir.x);
	m_RayDir[1]->SetFloat(ray.Dir.y);
	m_RayDir[2]->SetFloat(ray.Dir.z);
	Vector2 vec2 = input->GetMousePos();
	m_MousePos[0]->SetFloat(vec2.x);
	m_MousePos[1]->SetFloat(vec2.y);
	vec2 = input->GetMouseWorldPos();
	m_MouseWorldPos[0]->SetFloat(vec2.x);
	m_MouseWorldPos[1]->SetFloat(vec2.y);
	vec2 = input->GetMouseUIPos();
	m_MouseUIPos[0]->SetFloat(vec2.x);
	m_MouseUIPos[1]->SetFloat(vec2.y);
	vec2 = input->GetMouseMove();
	m_MouseMove[0]->SetFloat(vec2.x);
	m_MouseMove[1]->SetFloat(vec2.y);
	vec2 = input->GetMouseMove2D();
	m_MouseMove2D[0]->SetFloat(vec2.x);
	m_MouseMove2D[1]->SetFloat(vec2.y);
}
