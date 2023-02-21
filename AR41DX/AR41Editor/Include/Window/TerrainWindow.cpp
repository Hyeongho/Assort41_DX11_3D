#include "TerrainWindow.h"
#include "Input.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorGUIManager.h"
#include "Component/CameraComponent.h"
#include "Component/TerrainComponent.h"
#include "Scene/SceneManager.h"

CTerrainWindow::CTerrainWindow()
	: m_Grid(nullptr)
	, m_CountX(nullptr)
	, m_CountY(nullptr)
	, m_HeightMapX(nullptr)
	, m_HeightMapY(nullptr)
	, m_CellSizeX(nullptr)
	, m_CellSizeY(nullptr)
	, m_SizeX(nullptr)
	, m_SizeY(nullptr)
	, m_TerrainEditCombo(nullptr)
	, m_DetailLevel(nullptr)
	, m_SplatCount(nullptr)
	, m_MeshName(nullptr)
	, m_Image(nullptr)
{
}

CTerrainWindow::~CTerrainWindow()
{
}

void CTerrainWindow::SetSelectComponent(CTerrainComponent* component)
{
	m_SelectComponent = component;
	if (m_SelectComponent)
	{
		m_MeshName->SetText(m_SelectComponent->GetMesh()->GetName().c_str());
		m_Image->SetTexture(m_SelectComponent->GetMaterial(0)->GetTexture());
		m_Grid->SetCheck(m_SelectComponent->GetGrid());
		m_CountX->SetInt(m_SelectComponent->GetCountX());
		m_CountY->SetInt(m_SelectComponent->GetCountY());
		m_HeightMapX->SetInt(m_SelectComponent->GetHeightMapX());
		m_HeightMapY->SetInt(m_SelectComponent->GetHeightMapY());
		m_CellSizeX->SetFloat(m_SelectComponent->GetCellSize().x);
		m_CellSizeY->SetFloat(m_SelectComponent->GetCellSize().y);
		m_SizeX->SetFloat(m_SelectComponent->GetSize().x);
		m_SizeY->SetFloat(m_SelectComponent->GetSize().y);
		m_DetailLevel->SetFloat(m_SelectComponent->GetDetailLevel());
		m_SplatCount->SetInt(m_SelectComponent->GetSplatCount());
	}
}

bool CTerrainWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("터레인");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	m_Grid = CreateWidget<CEditorCheckBox>("격자");
	line = CreateWidget<CEditorSameLine>("Line");
	m_TerrainEditCombo = CreateWidget<CEditorComboBox>("EditMode", 120.f, 30.f);
	m_TerrainEditCombo->SetHideName("EditMode");
	m_TerrainEditCombo->AddItem("XY이동");
	m_TerrainEditCombo->AddItem("XZ이동");
	m_TerrainEditCombo->AddItem("YZ이동");
	m_TerrainEditCombo->AddItem("정지");
	m_TerrainEditCombo->Sort(false);
	m_TerrainEditCombo->SetSelectPrevViewName(true);
	m_TerrainEditCombo->SetSelectIndex(3);
	m_TerrainEditCombo->SetPrevViewName("정지");

	m_MeshName = CreateWidget<CEditorInput>("이름", 250.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CTerrainWindow>(this, &CTerrainWindow::ChangeTerrain);

	m_CountX = CreateWidget<CEditorInput>("CountX", 80.f, 30.f);
	m_CountX->SetInputType(EImGuiInputType::Int);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(45.f);
	m_CountY = CreateWidget<CEditorInput>("CountY", 80.f, 30.f);
	m_CountY->SetInputType(EImGuiInputType::Int);

	m_HeightMapX = CreateWidget<CEditorInput>("HeightMapX", 80.f, 30.f);
	m_HeightMapX->SetInputType(EImGuiInputType::Int);
	m_HeightMapX->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("Line");
	m_HeightMapY = CreateWidget<CEditorInput>("HeightMapY", 80.f, 30.f);
	m_HeightMapY->SetInputType(EImGuiInputType::Int);
	m_HeightMapY->ReadOnly(true);

	m_CellSizeX = CreateWidget<CEditorInput>("CellSizeX", 80.f, 30.f);
	m_CellSizeX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(28.f);
	m_CellSizeY = CreateWidget<CEditorInput>("CellSizeY", 80.f, 30.f);
	m_CellSizeY->SetInputType(EImGuiInputType::Float);

	m_SizeX = CreateWidget<CEditorInput>("SizeX", 80.f, 30.f);
	m_SizeX->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(57.f);
	m_SizeY = CreateWidget<CEditorInput>("SizeY", 80.f, 30.f);
	m_SizeY->SetInputType(EImGuiInputType::Float);

	m_DetailLevel = CreateWidget<CEditorInput>("DetailLevel", 80.f, 30.f);
	m_DetailLevel->SetInputType(EImGuiInputType::Float);
	line = CreateWidget<CEditorSameLine>("Line");
	line->SetSpacing(13.f);
	m_SplatCount = CreateWidget<CEditorInput>("SplatCount", 80.f, 30.f);
	m_SplatCount->SetInputType(EImGuiInputType::Int);

	m_Image = CreateWidget<CEditorImage>("Texture");
	m_Image->SetSize(150.f, 150.f);
	m_Image->SetHideName("Texture");
	//m_Image->SetIsFixed(true);

	AddInput(CSceneManager::GetInst()->GetScene());
	return true;
}

void CTerrainWindow::Update(float deltaTime)
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

void CTerrainWindow::ChangeTerrain()
{
	if (!m_SelectComponent)
	{
		return;
	}
	m_SelectComponent->SetDetailLevel(m_DetailLevel->GetFloat());
	m_SelectComponent->SetSplatCount(m_SplatCount->GetInt());
	//m_SelectComponent->CreateTerrain(m_CountX->GetInt(), m_CountY->GetInt(), m_CellSizeX->GetFloat(),
	//	m_CellSizeY->GetFloat(), m_Image->GetTexture()->GetFullPath(), ROOT_PATH);
}

void CTerrainWindow::UArrow()
{
	std::string itemName = m_TerrainEditCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(200.f * g_DeltaTime);
	}
	else if (itemName == "XZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionZ(200.f * g_DeltaTime);
	}
	else if (itemName == "YZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionZ(200.f * g_DeltaTime);
	}
}

void CTerrainWindow::DArrow()
{
	std::string itemName = m_TerrainEditCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(-200.f * g_DeltaTime);
	}
	else if (itemName == "XZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionZ(-200.f * g_DeltaTime);
	}
	else if (itemName == "YZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionZ(-200.f * g_DeltaTime);
	}
}

void CTerrainWindow::LArrow()
{
	std::string itemName = m_TerrainEditCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(-200.f * g_DeltaTime);
	}
	else if (itemName == "XZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(-200.f * g_DeltaTime);
	}
	else if (itemName == "YZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(-200.f * g_DeltaTime);
	}
}

void CTerrainWindow::RArrow()
{
	std::string itemName = m_TerrainEditCombo->GetSelectItem();
	if (itemName == "정지")
	{
		return;
	}
	else if (itemName == "XY이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(200.f * g_DeltaTime);
	}
	else if (itemName == "XZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(200.f * g_DeltaTime);
	}
	else if (itemName == "YZ이동")
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(200.f * g_DeltaTime);
	}
}

void CTerrainWindow::SetImage(const std::string& name, const TCHAR* path)
{
	m_Image->SetTextureFullPath(name, path);
}

void CTerrainWindow::AddInput(CScene* scene)
{
	CInput::GetInst()->AddBindFunction<CTerrainWindow>("UArrow", Input_Type::Push, this, &CTerrainWindow::UArrow, scene);
	CInput::GetInst()->AddBindFunction<CTerrainWindow>("DArrow", Input_Type::Push, this, &CTerrainWindow::DArrow, scene);
	CInput::GetInst()->AddBindFunction<CTerrainWindow>("LArrow", Input_Type::Push, this, &CTerrainWindow::LArrow, scene);
	CInput::GetInst()->AddBindFunction<CTerrainWindow>("RArrow", Input_Type::Push, this, &CTerrainWindow::RArrow, scene);
}
