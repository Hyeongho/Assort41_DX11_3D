#include "MeshWindow.h"
#include "Editor/EditorLabel.h"
#include "Editor/EditorSameLine.h"
#include "Editor/EditorInput.h"
#include "Editor/EditorImage.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorGUIManager.h"
#include "Editor/EditorCursorPos.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorSlider.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorListBox.h"
#include "Component/PrimitiveComponent.h"
#include "Resource/ResourceManager.h"
#include "Resource/Mesh/Mesh.h"

CMeshWindow::CMeshWindow()
    : m_Min{}
    , m_Max{}
    , m_Name(nullptr)
    , m_MeshType(nullptr)
    , m_RenderCount(nullptr)
{
}

CMeshWindow::~CMeshWindow()
{
}

void CMeshWindow::SetSelectComponent(CPrimitiveComponent* component)
{
	m_SelectComponent = component;
    if (!m_SelectComponent)
    {
        return;
    }
    m_Name->SetText(m_SelectComponent->GetMesh()->GetName().c_str());
    m_Min[0]->SetFloat(m_SelectComponent->GetMesh()->GetMin().x);
    m_Min[1]->SetFloat(m_SelectComponent->GetMesh()->GetMin().y);
    m_Min[2]->SetFloat(m_SelectComponent->GetMesh()->GetMin().z);
    m_Max[0]->SetFloat(m_SelectComponent->GetMesh()->GetMax().x);
    m_Max[1]->SetFloat(m_SelectComponent->GetMesh()->GetMax().y);
    m_Max[2]->SetFloat(m_SelectComponent->GetMesh()->GetMax().z);
    m_RenderCount->SetInt(m_SelectComponent->GetMesh()->GetRenderCount());
    int index = (int)m_SelectComponent->GetMesh()->GetMeshType();
    m_MeshType->SetSelectIndex(index);
}

bool CMeshWindow::Init()
{
	CEditorLabel* label = CreateWidget<CEditorLabel>("매쉬");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(130.f, 30.f);
	CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
	CEditorCursorPos* pos = CreateWidget<CEditorCursorPos>("Pos");
	pos->SetPosY(2.5f);
	m_MeshType = CreateWidget<CEditorComboBox>("MeshComboBox");
	m_MeshType->SetHideName("MeshComboBox");
	m_MeshType->SetPrevViewName("Sprite");
	m_MeshType->AddItem("Sprite");
	m_MeshType->AddItem("UI");
	m_MeshType->AddItem("Static");
	m_MeshType->AddItem("Animation");
	m_MeshType->SetSelectPrevViewName(true);
	m_MeshType->SetSelectIndex(0);

	m_Name = CreateWidget<CEditorInput>("매쉬 이름", 80.f, 30.f);
	line = CreateWidget<CEditorSameLine>("Line");
	m_RenderCount = CreateWidget<CEditorInput>("렌더 카운트", 80.f, 30.f);
	m_RenderCount->SetInputType(EImGuiInputType::Int);
	m_RenderCount->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("Min");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_Min[0] = CreateWidget<CEditorInput>("MinX", 80.f, 30.f);
	m_Min[0]->SetHideName("MinX");
	m_Min[0]->SetInputType(EImGuiInputType::Float);
	m_Min[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_Min[1] = CreateWidget<CEditorInput>("MinY", 80.f, 30.f);
	m_Min[1]->SetHideName("MinY");
	m_Min[1]->SetInputType(EImGuiInputType::Float);
	m_Min[1]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_Min[2] = CreateWidget<CEditorInput>("MinZ", 80.f, 30.f);
	m_Min[2]->SetHideName("MinZ");
	m_Min[2]->SetInputType(EImGuiInputType::Float);
	m_Min[2]->ReadOnly(true);

	label = CreateWidget<CEditorLabel>("Max");
	label->SetColor(255, 0, 0, 255);
	label->SetAlign(0.5f, 0.5f);
	label->SetSize(100.f, 30.f);
	m_Max[0] = CreateWidget<CEditorInput>("MaxX", 80.f, 30.f);
	m_Max[0]->SetHideName("MaxX");
	m_Max[0]->SetInputType(EImGuiInputType::Float);
	m_Max[0]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_Max[1] = CreateWidget<CEditorInput>("MaxY", 80.f, 30.f);
	m_Max[1]->SetHideName("MaxY");
	m_Max[1]->SetInputType(EImGuiInputType::Float);
	m_Max[1]->ReadOnly(true);
	line = CreateWidget<CEditorSameLine>("line");
	m_Max[2] = CreateWidget<CEditorInput>("MaxZ", 80.f, 30.f);
	m_Max[2]->SetHideName("MaxZ");
	m_Max[2]->SetInputType(EImGuiInputType::Float);
	m_Max[2]->ReadOnly(true);
	return true;
}

void CMeshWindow::Update(float deltaTime)
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

void CMeshWindow::MeshChangeCallback(const TCHAR* path)
{
	char fullPath[256];
	char name[256];
	char ext[256];
	WideCharToMultiByte(CP_ACP, 0, path, _MAX_EXT, fullPath, _MAX_EXT, NULL, NULL);
	_splitpath_s(fullPath, nullptr, 0, nullptr, 0, name, _MAX_EXT, ext, 256);
	CResourceManager* resourceManager = CResourceManager::GetInst();
	int meshType = m_MeshType->GetSelectIndex();
	if (MeshType::Animation == (MeshType)meshType)
	{
		resourceManager->LoadMeshFullPath(nullptr, MeshType::Animation, name, path);

		std::string bneName= fullPath;
		bneName = bneName.replace(bneName.size()-4, bneName.size(), ".bne");
		size_t szlen = bneName.size();
		TCHAR* bnePath = new TCHAR[szlen + 1];
		bnePath[szlen] = 0;
		std::copy(bneName.begin(), bneName.end(), bnePath);
		resourceManager->LoadSkeletonFullPath(nullptr, name, bnePath);
		resourceManager->SetMeshSkeleton(name, name);
		SAFE_DELETE_ARRAY(bnePath);
	}
	else
	{
		resourceManager->LoadMeshFullPath(nullptr, (MeshType)meshType, name, path);
	}
	if (m_SelectComponent)
	{
		m_SelectComponent->SetMesh(name);
	}
}

void CMeshWindow::SkeletonChangeCallback(const TCHAR* path)
{
	char name[256];
	char ext[256];
	WideCharToMultiByte(CP_ACP, 0, path, _MAX_EXT, name, _MAX_EXT, NULL, NULL);
	_splitpath_s(name, nullptr, 0, nullptr, 0, name, _MAX_EXT, ext, 256);
	CResourceManager* resourceManager = CResourceManager::GetInst();
	resourceManager->LoadSkeletonFullPath(nullptr, name, path);
	resourceManager->SetMeshSkeleton(name, name);
}
