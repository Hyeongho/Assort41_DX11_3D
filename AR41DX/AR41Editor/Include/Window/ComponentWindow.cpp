
#include "ComponentWindow.h"
#include "TransformWindow.h"
#include "DetailWindow.h"
#include "UIButtonWindow.h"
#include "UIImageWindow.h"
#include "UINumberWindow.h"
#include "UIProgressBarWindow.h"
#include "UITextWindow.h"
#include "RigidBodyWindow.h"
#include "LightWindow.h"
#include "MaterialWindow.h"
#include "CameraWindow.h"
#include "TargetArmWindow.h"
#include "MeshWindow.h"
#include "Input.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorText.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorGUIManager.h"
#include "Component/Component.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2d.h"
#include "Component/ColliderSphere2d.h"
#include "Component/ColliderObb2d.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/TargetArm.h"
#include "Component/RigidBody.h"
#include "Component/LightComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"

#include "UI/UIWidget.h"
#include "UI/UIButton.h"
#include "UI/UIImage.h"
#include "UI/UINumber.h"
#include "UI/UIProgressBar.h"
#include "UI/UIText.h"

CComponentWindow::CComponentWindow()
	: m_Tree(nullptr)
	, m_WidgetTree(nullptr)
{
}

CComponentWindow::~CComponentWindow()
{
}

bool CComponentWindow::AddItem(CComponent* Component, const std::string& Name, const std::string& ParentName)
{
	return m_Tree->AddItem(Component, Name, ParentName);
}

bool CComponentWindow::AddWidget(CUIWidget* widget, const std::string& name, const std::string& parentName)
{
	return m_WidgetTree->AddItem(widget, name, parentName);
}

void CComponentWindow::AddInput(CScene* scene)
{
	CInput::GetInst()->AddBindFunction<CComponentWindow>
		("ChangePos", Input_Type::Down, this, &CComponentWindow::ChangePos, scene);
}

void CComponentWindow::Clear()
{
	m_Tree->Clear();
	m_WidgetTree->Clear();
}

void CComponentWindow::ClearSelect()
{
	m_SelectComponent = nullptr;
	m_SelectWidget = nullptr;
}

void CComponentWindow::ChangePos()
{
	if (!m_SelectComponent && !m_SelectWidget)
	{
		return;
	}
	Vector2 mouseWorldPos = CInput::GetInst()->GetMouseWorldPos();
	if (m_SelectComponent)
	{
		CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
		if (!CEngine::GetInst()->GetRender2D())
		{
			return;
			//Vector2 mousePos = CInput::GetInst()->GetMousePos();
			//mouseWorldPos -= mousePos;
		}
		component->SetWorldPosition(mouseWorldPos);
	}
	else if (m_SelectWidget)
	{
		m_SelectWidget->SetPos(mouseWorldPos);
	}
}

bool CComponentWindow::Init()
{
	m_Tree = CreateWidget<CEditorTree<CComponent*>>("ComponentTree");

	m_Tree->SetHideName("ComponentTree");

	m_Tree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::TreeCallback);

	m_Tree->SetDoubleClickCallback<CComponentWindow>(this, &CComponentWindow::TreeDCCallback);

	m_Tree->SetSize(400.f, 300.f);

	m_WidgetTree = CreateWidget<CEditorTree<CUIWidget*>>("WidgetTree");
	m_WidgetTree->SetHideName("WidgetTree");
	m_WidgetTree->SetSelectCallback<CComponentWindow>(this, &CComponentWindow::WidgetCallback);
	m_WidgetTree->SetDoubleClickCallback<CComponentWindow>(this, &CComponentWindow::WidgetDCCallback);
	m_WidgetTree->SetSize(400.f, 300.f);

	AddInput(CSceneManager::GetInst()->GetScene());
	return true;
}

void CComponentWindow::Update(float DeltaTime)
{
	CEditorWindow::Update(DeltaTime);

	if (m_SelectComponent)
	{
		if (!m_SelectComponent->GetActive())
			m_SelectComponent = nullptr;
	}
	if (m_SelectWidget)
	{
		if (!m_SelectWidget->GetActive())
		{
			m_SelectWidget = nullptr;
		}
	}
}

void CComponentWindow::TreeCallback(CEditorTreeItem<class CComponent*>* Node, const std::string& Item)
{
	m_SelectComponent = Node->GetCustomData();
	if (!m_SelectComponent)
	{
		return;
	}
	if (m_SelectComponent->GetComponentTypeName() == "CameraComponent")
	{
		CCameraWindow* cameraWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CCameraWindow>("CameraWindow");
		if (cameraWindow)
		{
			cameraWindow->SetSelectComponent((CCameraComponent*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "TargetArm")
	{
		CTargetArmWindow* targetArmWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CTargetArmWindow>("TargetArmWindow");
		if (targetArmWindow)
		{
			targetArmWindow->SetSelectComponent((CTargetArm*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "RigidBody")
	{
		CRigidBodyWindow* rigidBody = CEditorGUIManager::GetInst()->FindEditorWindow<CRigidBodyWindow>("RigidBodyWindow");
		if (rigidBody)
		{
			rigidBody->SetSelectComponent((CRigidBody*)m_SelectComponent.Get());
		}
	}
	else if (m_SelectComponent->GetComponentTypeName() == "LightComponent")
	{
		CLightWindow* light = CEditorGUIManager::GetInst()->FindEditorWindow<CLightWindow>("LightWindow");
		if (light)
		{
			light->SetSelectComponent((CLightComponent*)m_SelectComponent.Get());
		}
	}
	CMaterialWindow* materialWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMaterialWindow>("MaterialWindow");
	if (materialWindow)
	{
		materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
	}
	CMeshWindow* meshWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMeshWindow>("MeshWindow");
	if (meshWindow)
	{
		meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
	}
	CTransformWindow* transformWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CTransformWindow>("TransformWindow");
	if (!transformWindow)
	{
		return;
	}
	CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
	transformWindow->SetSelectComponent(component);
	if (component->GetParent())
	{
		transformWindow->SetPos(component->GetRelativePos());
		transformWindow->SetScale(component->GetRelativeScale());
		transformWindow->SetRotation(component->GetRelativeRot());
	}
	else
	{
		transformWindow->SetPos(component->GetWorldPos());
		transformWindow->SetScale(component->GetWorldScale());
		transformWindow->SetRotation(component->GetWorldRot());
	}
}

void CComponentWindow::TreeDCCallback(CEditorTreeItem<class CComponent*>* node, const std::string& item)
{
	m_SelectComponent = node->GetCustomData();
	if (!m_SelectComponent)
	{
		return;
	}
	CDetailWindow* detailWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CDetailWindow>("DetailWindow");
	if (detailWindow)
	{
		detailWindow->SetSelectComponent((CSceneComponent*)m_SelectComponent.Get());
	}
	if (m_SelectComponent->GetComponentTypeName() == "CameraComponent")
	{
		CCameraWindow* cameraWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CCameraWindow>("CameraWindow");
		if (!cameraWindow)
		{
			cameraWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CCameraWindow>("CameraWindow");
		}
		cameraWindow->SetSelectComponent((CCameraComponent*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "TargetArm")
	{
		CTargetArmWindow* targetArmWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CTargetArmWindow>("TargetArmWindow");
		if (!targetArmWindow)
		{
			targetArmWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CTargetArmWindow>("TargetArmWindow");
		}
		targetArmWindow->SetSelectComponent((CTargetArm*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "RigidBody")
	{
		CRigidBodyWindow* rigidBody = CEditorGUIManager::GetInst()->FindEditorWindow<CRigidBodyWindow>("RigidBodyWindow");
		if (!rigidBody)
		{
			rigidBody = CEditorGUIManager::GetInst()->CreateEditorWindow<CRigidBodyWindow>("RigidBodyWindow");
		}
		rigidBody->SetSelectComponent((CRigidBody*)m_SelectComponent.Get());
	}
	else if (m_SelectComponent->GetComponentTypeName() == "LightComponent")
	{
		CLightWindow* light = CEditorGUIManager::GetInst()->FindEditorWindow<CLightWindow>("LightWindow");
		if (!light)
		{
			light = CEditorGUIManager::GetInst()->CreateEditorWindow<CLightWindow>("LightWindow");
		}
		light->SetSelectComponent((CLightComponent*)m_SelectComponent.Get());
	}
	CMaterialWindow* materialWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMaterialWindow>("MaterialWindow");
	if (materialWindow)
	{
		materialWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
	}
	CMeshWindow* meshWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CMeshWindow>("MeshWindow");
	if (meshWindow)
	{
		meshWindow->SetSelectComponent((CPrimitiveComponent*)m_SelectComponent.Get());
	}
	CTransformWindow* transformWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CTransformWindow>("TransformWindow");
	if (!transformWindow)
	{
		return;
	}
	CSceneComponent* component = (CSceneComponent*)m_SelectComponent.Get();
	transformWindow->SetSelectComponent(component);
	if (component->GetParent())
	{
		transformWindow->SetPos(component->GetRelativePos());
		transformWindow->SetScale(component->GetRelativeScale());
		transformWindow->SetRotation(component->GetRelativeRot());
	}
	else
	{
		transformWindow->SetPos(component->GetWorldPos());
		transformWindow->SetScale(component->GetWorldScale());
		transformWindow->SetRotation(component->GetWorldRot());
	}
}

void CComponentWindow::WidgetCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item)
{
	m_SelectWidget = node->GetCustomData();
	if (!m_SelectWidget)
	{
		return;
	}
	if (m_SelectWidget->GetWidgetTypeName() == "UIButton")
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if (buttonWindow)
		{
			buttonWindow->SetSelectWidget((CUIButton*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIImage")
	{
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (imgWindow)
		{
			imgWindow->SetSelectWidget((CUIImage*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UINumber")
	{
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (numberWindow)
		{
			numberWindow->SetSelectWidget((CUINumber*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIProgressBar")
	{
		CUIProgressBarWindow* barWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (barWindow)
		{
			barWindow->SetSelectWidget((CUIProgressBar*)m_SelectWidget.Get());
		}
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIText")
	{
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (textWindow)
		{
			textWindow->SetSelectWidget((CUIText*)m_SelectWidget.Get());
		}
	}
}

void CComponentWindow::WidgetDCCallback(CEditorTreeItem<class CUIWidget*>* node, const std::string& item)
{
	m_SelectWidget = node->GetCustomData();
	if (!m_SelectWidget)
	{
		return;
	}
	if (m_SelectWidget->GetWidgetTypeName() == "UIButton")
	{
		CUIButtonWindow* buttonWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIButtonWindow>("UIButtonWindow");
		if(!buttonWindow)
		{
			buttonWindow=CEditorGUIManager::GetInst()->CreateEditorWindow<CUIButtonWindow>("UIButtonWindow");
		}
		buttonWindow->SetSelectWidget((CUIButton*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIImage")
	{
		CUIImageWindow* imgWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUIImageWindow>("UIImageWindow");
		if (!imgWindow)
		{
			imgWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUIImageWindow>("UIImageWindow");
		}
		imgWindow->SetSelectWidget((CUIImage*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UINumber")
	{
		CUINumberWindow* numberWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUINumberWindow>("UINumberWindow");
		if (!numberWindow)
		{
			numberWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUINumberWindow>("UINumberWindow");
		}
		numberWindow->SetSelectWidget((CUINumber*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIProgressBar")
	{
		CUIProgressBarWindow* barWindow =
			CEditorGUIManager::GetInst()->FindEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		if (!barWindow)
		{
			barWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUIProgressBarWindow>("UIProgressBarWindow");
		}
		barWindow->SetSelectWidget((CUIProgressBar*)m_SelectWidget.Get());
	}
	else if (m_SelectWidget->GetWidgetTypeName() == "UIText")
	{
		CUITextWindow* textWindow = CEditorGUIManager::GetInst()->FindEditorWindow<CUITextWindow>("UITextWindow");
		if (!textWindow)
		{
			textWindow = CEditorGUIManager::GetInst()->CreateEditorWindow<CUITextWindow>("UITextWindow");
		}
		textWindow->SetSelectWidget((CUIText*)m_SelectWidget.Get());
	}
}
