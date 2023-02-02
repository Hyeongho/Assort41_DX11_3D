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
    m_SelectMesh = component->GetMesh();
    if (!m_SelectMesh)
    {
        return;
    }
    m_Name->SetText(m_SelectMesh->GetName().c_str());
    m_Min[0]->SetFloat(m_SelectMesh->GetMin().x);
    m_Min[1]->SetFloat(m_SelectMesh->GetMin().y);
    m_Min[2]->SetFloat(m_SelectMesh->GetMin().z);
    m_Max[0]->SetFloat(m_SelectMesh->GetMax().x);
    m_Max[1]->SetFloat(m_SelectMesh->GetMax().y);
    m_Max[2]->SetFloat(m_SelectMesh->GetMax().z);
    m_RenderCount->SetInt(m_SelectMesh->GetRenderCount());
    int index = (int)m_SelectMesh->GetMeshType();
    std::string name = m_MeshType->GetItem(index);
    m_MeshType->SetPrevViewName(name);
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
	line = CreateWidget<CEditorSameLine>("Line");
	CEditorButton* button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
	button->SetClickCallback<CMeshWindow>(this, &CMeshWindow::MeshCreateCallback);
	line = CreateWidget<CEditorSameLine>("Line");
	button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
	button->SetClickCallback<CMeshWindow>(this, &CMeshWindow::MeshChangeCallback);

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
    if (m_SelectMesh)
    {
        if (!m_SelectMesh->GetActive())
        {
            m_SelectMesh = nullptr;
        }
    }
}

void CMeshWindow::MeshChangeCallback()
{
    if (!m_SelectMesh)
    {
        return;
    }
   //m_SelectMesh->SetMeshType((MeshType)m_MeshType->GetSelectIndex());
    m_SelectMesh->SetName(m_Name->GetText());
}

void CMeshWindow::MeshCreateCallback()
{
    CResourceManager* resourceManager = CResourceManager::GetInst();
    int meshType=m_MeshType->GetSelectIndex();
    if(MeshType::Animation == (MeshType)meshType)
    {
        resourceManager->LoadMesh(nullptr, MeshType::Animation, m_Name->GetText(), TEXT("Spongebob_mesh.msh"), MESH_PATH);
        resourceManager->LoadSkeleton(nullptr, m_Name->GetText(), TEXT("Spongebob_mesh.bne"), MESH_PATH);
        resourceManager->SetMeshSkeleton(m_Name->GetText(), m_Name->GetText());
    }
    else
    {
        resourceManager->LoadMesh(nullptr, (MeshType)meshType, m_Name->GetText(), TEXT("Spongebob_mesh.msh"));
    }
}