#include "DetailWindow.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorTree.h"
#include "Editor/EditorComboBox.h"
#include "Component/SpriteComponent.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/TileMapComponent.h"
#include "Engine.h"
#include "PathManager.h"
#include "DetailWindow/CameraWidgetList.h"
#include "DetailWindow/TargetArmWidgetList.h"
#include "DetailWindow/SpriteComponentWidgetList.h"
#include "DetailWindow/SceneComponentWidgetList.h"
#include "DetailWindow/PrimitiveWidgetList.h"
#include "DetailWindow/TileMapWidgetList.h"
#include "Animation2DWindow.h"
#include "Editor/EditorGUIManager.h"

CDetailWindow::CDetailWindow()
{
}

CDetailWindow::~CDetailWindow()
{
	ClearWidget();
	
	size_t	Size = m_vecComponentWidgetList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		AddWidget(m_vecComponentWidgetList[i]);
	}
}

CComponentWidgetList* CDetailWindow::GetComponentWidgetList(
	const std::string& Name)
{
	size_t	Size = m_vecComponentWidgetList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecComponentWidgetList[i]->GetName() == Name)
			return m_vecComponentWidgetList[i];
	}

	return nullptr;
}

void CDetailWindow::SetSelectComponent(CSceneComponent* Component)
{
	if (m_SelectComponent && Component)
	{
		if (m_SelectComponent != Component)
		{
			ClearWidget();

			// 타입에 맞는 GUI 추가
			ChangeWidget(Component);
		}
	}

	m_SelectComponent = Component;
}

bool CDetailWindow::Init()
{
	m_vecComponentWidgetList.resize((size_t)ESceneComponentType::Max);

	for (int i = 0; i < (int)ESceneComponentType::Max; ++i)
	{
		CreateEditorWidgetList((ESceneComponentType)i);
	}

	// 위젯 한번 지워주기
	ClearWidget();

	return true;
}

void CDetailWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);

	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
		{
			m_SelectComponent = nullptr;
			ClearWidget();
		}
	}
}

void CDetailWindow::ChangeWidget(CSceneComponent* Component)
{
	if (Component->GetComponentTypeName() == "SceneComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::Scene]);
	}

	else if (Component->GetComponentTypeName() == "SpriteComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::Sprite]);

		CSpriteComponentWidgetList* SpriteWidget = (CSpriteComponentWidgetList*)m_vecComponentWidgetList[(int)ESceneComponentType::Sprite];

		CAnimation2DWindow* Anim2DWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CAnimation2DWindow>("Animation2DWindow");

		SpriteWidget->SetSelectAnimationSequence2DName(Anim2DWindow->GetAnimation2DSequenceSelectName());
		SpriteWidget->SetSpriteContent((CSpriteComponent*)Component);
	}

	else if (Component->GetComponentTypeName() == "CameraComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::Camera]);
	}

	else if (Component->GetComponentTypeName() == "TargetArmComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::TargetArm]);
	}

	else if (Component->GetComponentTypeName() == "TileMapComponent")
	{
		AddWidget(m_vecComponentWidgetList[(int)ESceneComponentType::TileMap]);

		CTileMapWidgetList* TileMapWidget = (CTileMapWidgetList*)m_vecComponentWidgetList[(int)ESceneComponentType::TileMap];

		TileMapWidget->SetTileMapComponent((CTileMapComponent*)Component);
	}
}

void CDetailWindow::LoadButtonClick()
{
	//OPENFILENAME	ofn = {};

	//TCHAR	FullPath[MAX_PATH] = {};

	//TCHAR	Filter[] = TEXT("모든 파일\0*.*\0PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp");

	//ofn.lStructSize = sizeof(OPENFILENAME);
	//ofn.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	//ofn.lpstrFilter = Filter;
	//ofn.lpstrFile = FullPath;
	//ofn.nMaxFile = MAX_PATH;
	//ofn.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	//if (GetOpenFileName(&ofn) != 0)
	//{
	//	CEditorTree<void*>* Category = (CEditorTree<void*>*)m_vecSpriteComponent[0];

	//	CEditorImage* ImageWidget = Category->FindWidget<CEditorImage>("SpriteImage");

	//	if (ImageWidget)
	//	{
	//		TCHAR	wTexName[256] = {};

	//		_wsplitpath_s(FullPath, 0, 0, 0, 0, wTexName, 256, 0, 0);

	//		char	TexName[256] = {};

	//		int Length = (int)WideCharToMultiByte(CP_ACP, 0, wTexName, -1, nullptr, 0, nullptr, nullptr);

	//		WideCharToMultiByte(CP_ACP, 0, wTexName, -1, TexName, Length, nullptr, nullptr);

	//		ImageWidget->SetTextureFullPath(TexName, FullPath);

	//		// 선택한 SpriteComponent의 Texture를 교체한다.
	//		if (m_SelectComponent)
	//		{
	//			((CSpriteComponent*)m_SelectComponent.Get())->SetTextureFullPath(TexName, FullPath);

	//			// Animation을 제거한다.
	//			((CSpriteComponent*)m_SelectComponent.Get())->ClearAnimation();
	//		}
	//	}
	//}
}

void CDetailWindow::CreateEditorWidgetList(ESceneComponentType Type)
{
	CComponentWidgetList* WidgetList = nullptr;

	switch (Type)
	{
	case ESceneComponentType::Scene:
		WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Primitive:
		WidgetList = CreateWidgetEmpty<CPrimitiveWidgetList>("PrimitiveComponent");
		break;
	case ESceneComponentType::Sprite:
		WidgetList = CreateWidgetEmpty<CSpriteComponentWidgetList>("SpriteComponent");
		break;
	case ESceneComponentType::Camera:
		WidgetList = CreateWidgetEmpty<CCameraWidgetList>("CameraComponent");
		break;
	case ESceneComponentType::TargetArm:
		WidgetList = CreateWidgetEmpty<CTargetArmWidgetList>("TargetArmComponent");
		break;
	case ESceneComponentType::Collider:
		//WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Collider2D:
		//WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Box2D:
		//WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::OBB2D:
		//WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Sphere2D:
		//WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Pixel:
		//WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::Collider3D:
		//WidgetList = CreateWidgetEmpty<CSceneComponentWidgetList>("SceneComponent");
		break;
	case ESceneComponentType::TileMap:
		WidgetList = CreateWidgetEmpty<CTileMapWidgetList>("TileMapComponent");
		break;
	}

	if (!WidgetList)
		return;

	WidgetList->m_DetailWindow = this;

	m_vecComponentWidgetList[(int)Type] = WidgetList;
}
