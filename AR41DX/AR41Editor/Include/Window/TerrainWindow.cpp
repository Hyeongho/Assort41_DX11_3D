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
	CEditorButton* button = CreateWidget<CEditorButton>("터레인 생성", 100.f, 30.f);
	button->SetClickCallback<CTerrainWindow>(this, &CTerrainWindow::CreateTerrainClick);

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

	m_TerrainEditCombo = CreateWidget<CEditorComboBox>("EditMode", 120.f, 30.f);
	m_TerrainEditCombo->SetHideName("EditMode");
	m_TerrainEditCombo->AddItem("이미지");
	m_TerrainEditCombo->AddItem("옵션");
	m_TerrainEditCombo->Sort(false);
	m_TerrainEditCombo->SetSelectPrevViewName(true);
	m_TerrainEditCombo->SetSelectIndex(1);
	m_TerrainEditCombo->SetPrevViewName("옵션");
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
	if (GetAsyncKeyState('W') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(200.f * g_DeltaTime);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionY(-200.f * g_DeltaTime);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(200.f * g_DeltaTime);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
		camera->AddWorldPositionX(-200.f * g_DeltaTime);
	}
}

void CTerrainWindow::CreateTerrainClick()
{
    /*
    	m_TileMap->CreateTile((ETileShape)m_ShapeCombo->GetSelectIndex(), m_CountX->GetInt(), m_CountY->GetInt()
		,Vector2(m_TileSizeX->GetFloat(), m_TileSizeY->GetFloat()));
	CTexture* texture = m_TileMap->GetTileMaterial()->GetTexture(0);
	m_TileFrameImage->SetTexture(texture);
	m_TileFrameImage->SetImageStart(0.f, 0.f);
	m_TileFrameImage->SetImageEnd(160.f, 80.f);
    */
}

void CTerrainWindow::SetFrameListCallback(int index, const std::string& item)
{
    //	m_TileFrameImage->SetImageIndex(index);
}