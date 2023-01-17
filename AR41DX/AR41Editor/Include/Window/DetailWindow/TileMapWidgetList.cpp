#include "TileMapWidgetList.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorComboBox.h"
#include "../Animation2DWindow.h"
#include "Editor/EditorGUIManager.h"
#include "../DetailWindow.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Animation/Animation2D.h"
#include "Resource/ResourceManager.h"
#include "Resource/Animation/AnimationManager.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Component/TileMapComponent.h"
#include "Input.h"
#include "../../Editor.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

CTileMapWidgetList::CTileMapWidgetList()
{
}

CTileMapWidgetList::~CTileMapWidgetList()
{
}

void CTileMapWidgetList::SetTileMapComponent(CTileMapComponent* TileMap)
{
	m_TileMap = TileMap;
}

bool CTileMapWidgetList::Init()
{
	CEditorTree<void*>* Category = CreateWidget<CEditorTree<void*>>("TileMapComponent");

	Category->SetHideName("TileMapComponent");

	Category->SetSize(400.f, 300.f);

	Category->AddItem(nullptr, "TileMapComponent");
	Category->AddItem(nullptr, "TileMap", "TileMapComponent");

	CEditorGroup* CreateGroup = Category->CreateWidget<CEditorGroup>("TileMap",
		"CreateGroup");

	m_ShapeCombo = CreateGroup->CreateWidget<CEditorComboBox>("TileShape",
		120.f, 30.f);
	m_ShapeCombo->SetHideName("TileShape");
	m_ShapeCombo->AddItem("사각형");
	m_ShapeCombo->AddItem("마름모");
	m_ShapeCombo->Sort(false);
	m_ShapeCombo->SetSelectPrevViewName(true);
	m_ShapeCombo->SetSelect(1);
	m_ShapeCombo->SetPrevViewName("마름모");

	m_ShapeCombo->SetSelectCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::SetShapeCallback);

	m_CountX = CreateGroup->CreateWidget<CEditorInput>("CountX", 100.f, 30.f);
	m_CountX->SetInputType(EImGuiInputType::Int);

	m_CountX->SetInt(100);

	CreateGroup->CreateWidget<CEditorSameLine>("Line");

	m_CountY = CreateGroup->CreateWidget<CEditorInput>("CountY", 100.f, 30.f);
	m_CountY->SetInputType(EImGuiInputType::Int);

	m_CountY->SetInt(100);

	m_TileSizeX = CreateGroup->CreateWidget<CEditorInput>("SizeX", 100.f, 30.f);
	m_TileSizeX->SetInputType(EImGuiInputType::Float);

	m_TileSizeX->SetFloat(160.f);

	CreateGroup->CreateWidget<CEditorSameLine>("Line");

	m_TileSizeY = CreateGroup->CreateWidget<CEditorInput>("SizeY", 100.f, 30.f);
	m_TileSizeY->SetInputType(EImGuiInputType::Float);

	m_TileSizeY->SetFloat(80.f);

	CEditorButton* Button = CreateGroup->CreateWidget<CEditorButton>("CreateTileMap", 150.f, 40.f);

	Button->SetClickCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::CreateTileMapClick);

	Category->CreateWidget<CEditorSameLine>("TileMap", "Line");

	CEditorGroup* EditGroup = Category->CreateWidget<CEditorGroup>("TileMap",
		"EditGroup");

	m_TileEditCombo = EditGroup->CreateWidget<CEditorComboBox>("EditMode",
		120.f, 30.f);
	m_TileEditCombo->SetHideName("EditMode");
	m_TileEditCombo->AddItem("이미지");
	m_TileEditCombo->AddItem("옵션");
	m_TileEditCombo->Sort(false);
	m_TileEditCombo->SetSelectPrevViewName(true);
	m_TileEditCombo->SetSelect(1);
	m_TileEditCombo->SetPrevViewName("옵션");

	m_TileEditCombo->SetSelectCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::SetEditModeCallback);

	EditGroup->CreateWidget<CEditorSameLine>("Line");

	m_TileOptionCombo = EditGroup->CreateWidget<CEditorComboBox>("TileOption",
		120.f, 30.f);
	m_TileOptionCombo->SetHideName("TileOption");
	m_TileOptionCombo->AddItem("일반");
	m_TileOptionCombo->AddItem("이동불가");
	m_TileOptionCombo->Sort(false);
	m_TileOptionCombo->SetSelectPrevViewName(true);
	m_TileOptionCombo->SetSelect(1);
	m_TileOptionCombo->SetPrevViewName("이동불가");

	//m_TileEditCombo->SetSelectCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::SetEditModeCallback);

	m_TileFrameList = EditGroup->CreateWidget<CEditorListBox>("FrameList",
		150.f, 400.f);
	m_TileFrameList->SetPageItemCount(10);
	m_TileFrameList->SetHideName("FrameList");

	for (int i = 0; i <= 379; ++i)
	{
		char	Number[32] = {};

		_itoa_s(i, Number, 10);

		m_TileFrameList->AddItem(Number);
	}
	m_TileFrameList->Sort(false);

	m_TileFrameList->SetSelectCallback<CTileMapWidgetList>(this, &CTileMapWidgetList::SetFrameListCallback);

	m_TileFrameImage = EditGroup->CreateWidget<CEditorImage>("TileFrameImage");


	return true;
}

void CTileMapWidgetList::Render()
{
	CComponentWidgetList::Render();

	if (GetAsyncKeyState('W') & 0x8000)
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPositionY(200.f * g_DeltaTime);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPositionY(-200.f * g_DeltaTime);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPositionX(200.f * g_DeltaTime);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPositionX(-200.f * g_DeltaTime);
	}

	// 마우스의 월드위치를 얻어온다.
	Vector2 Pos = CInput::GetInst()->GetMouseWorldPos();

	int	Index = m_TileMap->GetTileIndex(Pos);

	m_TileMap->SetEditorMouseOnTile(Index);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		switch ((ETileEditMode)m_TileEditCombo->GetSelectIndex())
		{
		case ETileEditMode::Image:
			m_TileMap->ChangeTileFrame(Pos, m_TileFrameList->GetSelectIndex());
			break;
		case ETileEditMode::Option:
			m_TileMap->ChangeTileOption(Pos, (ETileOption)m_TileOptionCombo->GetSelectIndex());
			break;
		}
	}
}

void CTileMapWidgetList::CreateTileMapClick()
{
	m_TileMap->CreateTile((ETileShape)m_ShapeCombo->GetSelectIndex(),
		m_CountX->GetInt(), m_CountY->GetInt(),
		Vector2(m_TileSizeX->GetFloat(), m_TileSizeY->GetFloat()));

	CTexture* Texture = m_TileMap->GetTileMaterial()->GetTexture(0);

	m_TileFrameImage->SetTexture(Texture);

	m_TileFrameImage->SetImageStart(0.f, 0.f);
	m_TileFrameImage->SetImageEnd(160.f, 80.f);
}

void CTileMapWidgetList::SetShapeCallback(int Index, const std::string& Item)
{
}

void CTileMapWidgetList::SetEditModeCallback(int Index, const std::string& Item)
{
}

void CTileMapWidgetList::SetFrameListCallback(int Index, const std::string& Item)
{
	m_TileFrameImage->SetImageIndex(Index);
}
