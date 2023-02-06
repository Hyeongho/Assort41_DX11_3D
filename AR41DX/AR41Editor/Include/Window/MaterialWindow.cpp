#include "MaterialWindow.h"
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
#include "Resource/Material/Material.h"
#include "Resource/Shader/Shader.h"
#include "Resource/ResourceManager.h"

CMaterialWindow::CMaterialWindow()
    : m_RenderState{}
    , m_BaseColor{}
    , m_AmbientColor{}
    , m_SpecularColor{}
    , m_EmissiveColor{}
    , m_EnableBump(nullptr)
    , m_EnableSpecular(nullptr)
    , m_EnableEmissive(nullptr)
    , m_Animation3D(nullptr)
    , m_Shader(nullptr)
    , m_Tree(nullptr)
    , m_TextureTree(nullptr)
    , m_Opacity(nullptr)
    , m_Name(nullptr)
    , m_Image(nullptr)
{
}

CMaterialWindow::~CMaterialWindow()
{
}

void CMaterialWindow::SetSelectComponent(CPrimitiveComponent* component)
{
    m_SelectComponent = component;
    Clear();
    int size=m_SelectComponent->GetSlotCount();
    for(int i=0;i<size;++i)
    {
        AddItem(m_SelectComponent->GetMaterial(i), m_SelectComponent->GetMaterial(i)->GetName());
    }
}

bool CMaterialWindow::Init()
{
    m_Tree = CreateWidget<CEditorTree<CMaterial*>>("MaterialWindow");
    m_Tree->SetHideName("MaterialWindow");
    m_Tree->SetSelectCallback<CMaterialWindow>(this, &CMaterialWindow::TreeCallback);
    m_Tree->SetSize(400.f, 300.f);
    m_Tree->AddItem(nullptr, "Material");

    m_Name = CreateWidget<CEditorInput>("매터리얼 이름", 100.f, 30.f);
    CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
    CEditorButton* button = CreateWidget<CEditorButton>("생성", 50.f, 30.f);
    button->SetClickCallback<CMaterialWindow>(this, &CMaterialWindow::MaterialCreateCallback);
    line = CreateWidget<CEditorSameLine>("Line");
    button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
    button->SetClickCallback<CMaterialWindow>(this, &CMaterialWindow::MaterialChangeCallback);

    m_Shader = CreateWidget<CEditorInput>("쉐이더", 120.f, 30.f);
    line = CreateWidget<CEditorSameLine>("Line");
    m_Opacity = CreateWidget<CEditorInput>("투명도", 80.f, 30.f);
    m_Opacity->SetInputType(EImGuiInputType::Float);
	
    m_EnableBump = CreateWidget<CEditorCheckBox>("범프");
    line = CreateWidget<CEditorSameLine>("Line");
    m_EnableSpecular = CreateWidget<CEditorCheckBox>("반사광");
    line = CreateWidget<CEditorSameLine>("Line");
    m_EnableEmissive = CreateWidget<CEditorCheckBox>("발광");
    line = CreateWidget<CEditorSameLine>("Line");
    m_Animation3D = CreateWidget<CEditorCheckBox>("3D애니메이션");

    CEditorLabel* label = CreateWidget<CEditorLabel>("렌더 스테이트");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_RenderState[0] = CreateWidget<CEditorInput>("RenderState0", 80.f, 30.f);
    m_RenderState[0]->SetHideName("RenderState0");
    line = CreateWidget<CEditorSameLine>("line");
    m_RenderState[1] = CreateWidget<CEditorInput>("RenderState1", 80.f, 30.f);
    m_RenderState[1]->SetHideName("RenderState1");
    line = CreateWidget<CEditorSameLine>("line");
    m_RenderState[2] = CreateWidget<CEditorInput>("RenderState2", 80.f, 30.f);
    m_RenderState[2]->SetHideName("RenderState2");

    label = CreateWidget<CEditorLabel>("기본 색상");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_BaseColor[0] = CreateWidget<CEditorInput>("BaseColorX", 80.f, 30.f);
    m_BaseColor[0]->SetHideName("BaseColorX");
    m_BaseColor[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_BaseColor[1] = CreateWidget<CEditorInput>("BaseColorY", 80.f, 30.f);
    m_BaseColor[1]->SetHideName("BaseColorY");
    m_BaseColor[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_BaseColor[2] = CreateWidget<CEditorInput>("BaseColorZ", 80.f, 30.f);
    m_BaseColor[2]->SetHideName("BaseColorZ");
    m_BaseColor[2]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_BaseColor[3] = CreateWidget<CEditorInput>("BaseColorW", 80.f, 30.f);
    m_BaseColor[3]->SetHideName("BaseColorW");
    m_BaseColor[3]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("환경광");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_AmbientColor[0] = CreateWidget<CEditorInput>("AmbientColorX", 80.f, 30.f);
    m_AmbientColor[0]->SetHideName("AmbientColorX");
    m_AmbientColor[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_AmbientColor[1] = CreateWidget<CEditorInput>("AmbientColorY", 80.f, 30.f);
    m_AmbientColor[1]->SetHideName("AmbientColorY");
    m_AmbientColor[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_AmbientColor[2] = CreateWidget<CEditorInput>("AmbientColorZ", 80.f, 30.f);
    m_AmbientColor[2]->SetHideName("AmbientColorZ");
    m_AmbientColor[2]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_AmbientColor[3] = CreateWidget<CEditorInput>("AmbientColorW", 80.f, 30.f);
    m_AmbientColor[3]->SetHideName("AmbientColorW");
    m_AmbientColor[3]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("반사광");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_SpecularColor[0] = CreateWidget<CEditorInput>("SpecularColorX", 80.f, 30.f);
    m_SpecularColor[0]->SetHideName("SpecularColorX");
    m_SpecularColor[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_SpecularColor[1] = CreateWidget<CEditorInput>("SpecularColorY", 80.f, 30.f);
    m_SpecularColor[1]->SetHideName("SpecularColorY");
    m_SpecularColor[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_SpecularColor[2] = CreateWidget<CEditorInput>("SpecularColorZ", 80.f, 30.f);
    m_SpecularColor[2]->SetHideName("SpecularColorZ");
    m_SpecularColor[2]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_SpecularColor[3] = CreateWidget<CEditorInput>("SpecularColorW", 80.f, 30.f);
    m_SpecularColor[3]->SetHideName("SpecularColorW");
    m_SpecularColor[3]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("발광");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_EmissiveColor[0] = CreateWidget<CEditorInput>("EmissiveColorX", 80.f, 30.f);
    m_EmissiveColor[0]->SetHideName("EmissiveColorX");
    m_EmissiveColor[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_EmissiveColor[1] = CreateWidget<CEditorInput>("EmissiveColorY", 80.f, 30.f);
    m_EmissiveColor[1]->SetHideName("EmissiveColorY");
    m_EmissiveColor[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_EmissiveColor[2] = CreateWidget<CEditorInput>("EmissiveColorZ", 80.f, 30.f);
    m_EmissiveColor[2]->SetHideName("EmissiveColorZ");
    m_EmissiveColor[2]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_EmissiveColor[3] = CreateWidget<CEditorInput>("EmissiveColorW", 80.f, 30.f);
    m_EmissiveColor[3]->SetHideName("EmissiveColorW");
    m_EmissiveColor[3]->SetInputType(EImGuiInputType::Float);

   m_TextureTree = CreateWidget<CEditorTree<CTexture*>>("PathWindow");
   m_TextureTree->SetHideName("PathWindow");
   m_TextureTree->UseDragDropSelf(true);
   m_TextureTree->SetSelectCallback<CMaterialWindow>(this, &CMaterialWindow::TextureTreeCallback);
   m_TextureTree->SetSize(400.f, 300.f);
   m_TextureTree->AddItem(nullptr, "Texture");

   m_Image= CreateWidget<CEditorImage>("Image");
   m_Image->SetSize(150.f, 150.f);
   m_Image->SetHideName("Texture");
   //m_Image->SetIsFixed(true);
    return true;
}

void CMaterialWindow::Update(float deltaTime)
{
    CEditorWindow::Update(deltaTime);
    if (m_SelectComponent)
    {
        if (!m_SelectComponent->GetActive())
        {
            m_SelectComponent = nullptr;
        }
    }
    if (m_SelectMaterial)
    {
        if (!m_SelectMaterial->GetActive())
        {
            m_SelectMaterial = nullptr;
        }
    }
}

bool CMaterialWindow::AddItem(CMaterial* material, const std::string& name, const std::string& parentName)
{
    return m_Tree->AddItem(material, name, parentName);
}

bool CMaterialWindow::AddItem(CTexture* texture, const std::string& name, const std::string& parentName)
{
    return m_TextureTree->AddItem(texture, name, parentName);
}

void CMaterialWindow::Clear()
{
    m_Tree->Clear();
    m_Tree->AddItem(nullptr, "Material");
}

void CMaterialWindow::ClearTexture()
{
    m_TextureTree->Clear();
    m_TextureTree->AddItem(nullptr, "Texture");
}

void CMaterialWindow::ClearSelect()
{
    m_SelectComponent = nullptr;
    m_SelectMaterial = nullptr;
}

void CMaterialWindow::MaterialChangeCallback()
{
    if (!m_SelectMaterial)
    {
        return;
    }
    m_SelectMaterial->SetName(m_Name->GetText());
    size_t size = std::size(m_RenderState);
    for (size_t i = 0; i < size; ++i)
    {
        m_SelectMaterial->SetRenderState(m_RenderState[i]->GetText());
    }
    if (m_EnableBump->GetCheck())
    {
        m_SelectMaterial->EnableBump();
    }
    else
    {
        m_SelectMaterial->UnEnableBump();
    }
    if (m_EnableSpecular->GetCheck())
    {
        m_SelectMaterial->EnableSpecular();
    }
    else
    {
        m_SelectMaterial->UnEnableSpecular();
    }
    if (m_EnableEmissive->GetCheck())
    {
        m_SelectMaterial->EnableSpecular();
    }
    else
    {
        m_SelectMaterial->UnEnableSpecular();
    }
    if (m_Animation3D->GetCheck())
    {
        m_SelectMaterial->EnableAnimation3D();
    }
    else
    {
        m_SelectMaterial->UnEnableAnimation3D();
    }
    Vector4 color;
    color.x = m_BaseColor[0]->GetFloat();
    color.y = m_BaseColor[1]->GetFloat();
    color.z = m_BaseColor[2]->GetFloat();
    color.w = m_BaseColor[3]->GetFloat();
    m_SelectMaterial->SetBaseColor(color);
    color.x = m_AmbientColor[0]->GetFloat();
    color.y = m_AmbientColor[1]->GetFloat();
    color.z = m_AmbientColor[2]->GetFloat();
    color.w = m_AmbientColor[3]->GetFloat();
    m_SelectMaterial->SetAmbientColor(color);
    color.x = m_SpecularColor[0]->GetFloat();
    color.y = m_SpecularColor[1]->GetFloat();
    color.z = m_SpecularColor[2]->GetFloat();
    color.w = m_SpecularColor[3]->GetFloat();
    m_SelectMaterial->SetSpecularColor(color);
    color.x = m_EmissiveColor[0]->GetFloat();
    color.y = m_EmissiveColor[1]->GetFloat();
    color.z = m_EmissiveColor[2]->GetFloat();
    color.w = m_EmissiveColor[3]->GetFloat();
    m_SelectMaterial->SetEmissiveColor(color);
    m_SelectMaterial->SetOpacity(m_Opacity->GetFloat());
    m_SelectMaterial->SetShader(m_Shader->GetText());
}

void CMaterialWindow::MaterialCreateCallback()
{
    if(m_Name->GetText()=="")
    {
        return;
    }
    CMaterial* material = CResourceManager::GetInst()->CreateMaterial<CMaterial>(m_Name->GetText());
    size_t size = std::size(m_RenderState);
    for (size_t i = 0; i < size; ++i)
    {
        material->SetRenderState(m_RenderState[i]->GetText());
    }
    if (m_EnableBump->GetCheck())
    {
        material->EnableBump();
    }
    else
    {
        material->UnEnableBump();
    }
    if (m_EnableSpecular->GetCheck())
    {
        material->EnableSpecular();
    }
    else
    {
        material->UnEnableSpecular();
    }
    if (m_EnableEmissive->GetCheck())
    {
        material->EnableSpecular();
    }
    else
    {
        material->UnEnableSpecular();
    }
    if (m_Animation3D->GetCheck())
    {
        material->EnableAnimation3D();
    }
    else
    {
        material->UnEnableAnimation3D();
    }
    Vector4 color;
    color.x = m_BaseColor[0]->GetFloat();
    color.y = m_BaseColor[1]->GetFloat();
    color.z = m_BaseColor[2]->GetFloat();
    color.w = m_BaseColor[3]->GetFloat();
    material->SetBaseColor(color);
    color.x = m_AmbientColor[0]->GetFloat();
    color.y = m_AmbientColor[1]->GetFloat();
    color.z = m_AmbientColor[2]->GetFloat();
    color.w = m_AmbientColor[3]->GetFloat();
    material->SetAmbientColor(color);
    color.x = m_SpecularColor[0]->GetFloat();
    color.y = m_SpecularColor[1]->GetFloat();
    color.z = m_SpecularColor[2]->GetFloat();
    color.w = m_SpecularColor[3]->GetFloat();
    material->SetSpecularColor(color);
    color.x = m_EmissiveColor[0]->GetFloat();
    color.y = m_EmissiveColor[1]->GetFloat();
    color.z = m_EmissiveColor[2]->GetFloat();
    color.w = m_EmissiveColor[3]->GetFloat();
    material->SetEmissiveColor(color);
    material->SetOpacity(m_Opacity->GetFloat());
    material->SetShader(m_Shader->GetText());
    //	material->AddTexture(0, (int)EShaderBufferType::Pixel, "DefaultTexture", TEXT("neutral.png"));
    /*
    	std::vector<const TCHAR*>	vecFileNames;
	for (int i = 0; i <= 379; ++i)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(fileName, TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor_%d.png"), i);
		vecFileNames.push_back(fileName);
	}
    */
    //material->AddTextureArray(10, (int)EShaderBufferType::Pixel, "DefaultTileIsometric", vecFileNames);
}

void CMaterialWindow::ImgChangeCallback(const std::string& name, const TCHAR* path)
{
    if(!m_SelectMaterial)
    {
        return;
    }
    std::string itemName=m_TextureTree->GetHoverItem()->GetItemName();
    int index = m_TextureTree->GetHoverItem()->FindIndex(itemName);
    if(index<0)
    {
        return;
    }
    m_SelectMaterial->SetTexture(index,0, (int)EShaderBufferType::Pixel, name, path);
    m_Image->SetTexture(m_SelectMaterial->GetTexture(index));
}

void CMaterialWindow::TreeCallback(CEditorTreeItem<class CMaterial*>* node, const std::string& item)
{
    m_SelectMaterial = node->GetCustomData();
    if (!m_SelectMaterial)
    {
        return;
    }
    std::string name = item;
    m_Name->SetText(name.c_str());
    int size = (int)std::size(m_RenderState);
    for (int i = 0; i < size;++i)
    {
       if(!m_SelectMaterial->GetRenderState(i))
       {
           continue;
       }
       m_RenderState[i]->SetText(m_SelectMaterial->GetRenderState(i)->GetName().c_str());
    }
    m_BaseColor[0]->SetFloat(m_SelectMaterial->GetBaseColor().x);
    m_BaseColor[1]->SetFloat(m_SelectMaterial->GetBaseColor().y);
    m_BaseColor[2]->SetFloat(m_SelectMaterial->GetBaseColor().z);
    m_BaseColor[3]->SetFloat(m_SelectMaterial->GetBaseColor().w);
    m_AmbientColor[0]->SetFloat(m_SelectMaterial->GetAmbientColor().x);
    m_AmbientColor[1]->SetFloat(m_SelectMaterial->GetAmbientColor().y);
    m_AmbientColor[2]->SetFloat(m_SelectMaterial->GetAmbientColor().z);
    m_AmbientColor[3]->SetFloat(m_SelectMaterial->GetAmbientColor().w);
    m_SpecularColor[0]->SetFloat(m_SelectMaterial->GetSpecularColor().x);
    m_SpecularColor[1]->SetFloat(m_SelectMaterial->GetSpecularColor().y);
    m_SpecularColor[2]->SetFloat(m_SelectMaterial->GetSpecularColor().z);
    m_SpecularColor[3]->SetFloat(m_SelectMaterial->GetSpecularColor().w);
    m_EmissiveColor[0]->SetFloat(m_SelectMaterial->GetEmissiveColor().x);
    m_EmissiveColor[1]->SetFloat(m_SelectMaterial->GetEmissiveColor().y);
    m_EmissiveColor[2]->SetFloat(m_SelectMaterial->GetEmissiveColor().z);
    m_EmissiveColor[3]->SetFloat(m_SelectMaterial->GetEmissiveColor().w);
    m_EnableBump->SetCheck(m_SelectMaterial->GetIsBump());
    m_EnableSpecular->SetCheck(m_SelectMaterial->GetIsSpecular());
    m_EnableEmissive->SetCheck(m_SelectMaterial->GetIsEmissive());
    m_Animation3D->SetCheck(m_SelectMaterial->GetIsAnimation3D());
    m_Opacity->SetFloat(m_SelectMaterial->GetOpacity());
    m_Shader->SetText(m_SelectMaterial->GetShader()->GetName().c_str());
    ClearTexture();
    size = m_SelectMaterial->GetTextureCount();
    for (int i = 0; i < size; ++i)
    {
        AddItem(m_SelectMaterial->GetTexture(i), m_SelectMaterial->GetTexture(i)->GetName());
    }
}

void CMaterialWindow::TextureTreeCallback(CEditorTreeItem<class CTexture*>* node, const std::string& item)
{
    m_Image->SetTexture(node->GetCustomData());
}
