#include "ParticleWindow.h"
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
#include "Component/ParticleComponent.h"
#include "Resource/ResourceManager.h"
#include "Resource/Shader/ParticleConstantBuffer.h"

CParticleWindow::CParticleWindow()
    : m_ParticleStartMin{}
    , m_ParticleStartMax{}
    , m_ParticleScaleMin{}
    , m_ParticleScaleMax{}
    , m_ParticleMoveDir{}
    , m_ParticleMoveAngle{}
    , m_ParticleColorMin{}
    , m_ParticleColorMax{}
    , m_SpawnTime(nullptr)
    , m_SpawnCountMax(nullptr)
    , m_Name(nullptr)
    , m_ParticleSpawnEnable(nullptr)
    , m_ParticleMoveEnable(nullptr)
    , m_ParticleGravity(nullptr)
    , m_ParticleMoveDirEnable(nullptr)
    , m_ParticleLifeTimeMin(nullptr)
    , m_ParticleLifeTimeMax(nullptr)
    , m_ParticleList(nullptr)
{
}

CParticleWindow::~CParticleWindow()
{
}

void CParticleWindow::SetSelectComponent(CParticleComponent* component)
{
    m_SelectComponent = component;
    if (m_SelectComponent)
    {
        m_SelectParticle = m_SelectComponent->GetParticle();
        m_SpawnTime->SetFloat(m_SelectComponent->GetSpawnTimeMax());
        m_SpawnCountMax->SetInt(m_SelectComponent->GetSpawnCountMax());
        if(!m_SelectParticle)
        {
            return;
        }
        m_Name->SetText(m_SelectParticle->GetName().c_str());
        ParticleCBuffer	bufferData = m_SelectParticle->GetParticleCBuffer()->GetBufferData();
        m_ParticleSpawnEnable->SetCheck(bufferData.ParticleSpawnEnable);
        m_ParticleMoveEnable->SetCheck(bufferData.ParticleMoveEnable);
        m_ParticleGravity->SetCheck(bufferData.ParticleGravity);
        m_ParticleMoveDirEnable->SetCheck(bufferData.ParticleMoveDirEnable);
        m_ParticleLifeTimeMin->SetFloat(bufferData.ParticleLifeTimeMin);
        m_ParticleLifeTimeMax->SetFloat(bufferData.ParticleLifeTimeMax);
        m_ParticleMoveAngle[0]->SetFloat(bufferData.ParticleMoveAngle.x);
        m_ParticleMoveAngle[1]->SetFloat(bufferData.ParticleMoveAngle.y);
        m_ParticleMoveAngle[2]->SetFloat(bufferData.ParticleMoveAngle.z);
        m_ParticleColorMin[0]->SetFloat(bufferData.ParticleColorMin.x);
        m_ParticleColorMin[1]->SetFloat(bufferData.ParticleColorMin.y);
        m_ParticleColorMin[2]->SetFloat(bufferData.ParticleColorMin.z);
        m_ParticleColorMin[3]->SetFloat(bufferData.ParticleColorMin.w);
        m_ParticleColorMax[0]->SetFloat(bufferData.ParticleColorMax.x);
        m_ParticleColorMax[1]->SetFloat(bufferData.ParticleColorMax.y);
        m_ParticleColorMax[2]->SetFloat(bufferData.ParticleColorMax.z);
        m_ParticleColorMax[3]->SetFloat(bufferData.ParticleColorMax.w);
        m_ParticleStartMin[0]->SetFloat(bufferData.ParticleStartMin.x);
        m_ParticleStartMin[1]->SetFloat(bufferData.ParticleStartMin.y);
        m_ParticleStartMin[2]->SetFloat(bufferData.ParticleStartMin.z);
        m_ParticleStartMax[0]->SetFloat(bufferData.ParticleStartMax.x);
        m_ParticleStartMax[1]->SetFloat(bufferData.ParticleStartMax.y);
        m_ParticleStartMax[2]->SetFloat(bufferData.ParticleStartMax.z);
        m_ParticleScaleMin[0]->SetFloat(bufferData.ParticleScaleMin.x);
        m_ParticleScaleMin[1]->SetFloat(bufferData.ParticleScaleMin.y);
        m_ParticleScaleMin[2]->SetFloat(bufferData.ParticleScaleMin.z);
        m_ParticleScaleMax[0]->SetFloat(bufferData.ParticleScaleMax.x);
        m_ParticleScaleMax[1]->SetFloat(bufferData.ParticleScaleMax.y);
        m_ParticleScaleMax[2]->SetFloat(bufferData.ParticleScaleMax.z);
        m_ParticleMoveDir[0]->SetFloat(bufferData.ParticleMoveDir.x);
        m_ParticleMoveDir[1]->SetFloat(bufferData.ParticleMoveDir.y);
        m_ParticleMoveDir[2]->SetFloat(bufferData.ParticleMoveDir.z);
    }
}

bool CParticleWindow::Init()
{
    CEditorLabel* label = CreateWidget<CEditorLabel>("파티클");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(130.f, 30.f);
    m_Name = CreateWidget<CEditorInput>("이름", 120.f, 30.f);
    CEditorSameLine* line = CreateWidget<CEditorSameLine>("Line");
    CEditorButton* button = CreateWidget<CEditorButton>("변경", 50.f, 30.f);
    button->SetClickCallback<CParticleWindow>(this, &CParticleWindow::ParticleChangeCallback);

    m_ParticleSpawnEnable = CreateWidget<CEditorCheckBox>("파티클 생성");
    line = CreateWidget<CEditorSameLine>("Line");
    m_ParticleMoveEnable = CreateWidget<CEditorCheckBox>("이동");
    line = CreateWidget<CEditorSameLine>("Line");
    m_ParticleGravity = CreateWidget<CEditorCheckBox>("중력");
    line = CreateWidget<CEditorSameLine>("Line");
    m_ParticleMoveDirEnable = CreateWidget<CEditorCheckBox>("방향");

    m_SpawnTime = CreateWidget<CEditorInput>("스폰 시간", 80.f, 30.f);
    m_SpawnTime->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("Line");
    m_SpawnCountMax = CreateWidget<CEditorInput>("스폰 개수", 80.f, 30.f);
    m_SpawnCountMax->SetInputType(EImGuiInputType::Int);

    m_ParticleLifeTimeMin = CreateWidget<CEditorInput>("최소 생존시간", 80.f, 30.f);
    m_ParticleLifeTimeMin->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("Line");
    m_ParticleLifeTimeMax = CreateWidget<CEditorInput>("최대 생존시간", 80.f, 30.f);
    m_ParticleLifeTimeMax->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("MoveAngle");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleMoveAngle[0] = CreateWidget<CEditorInput>("ParticleMoveAngleX", 80.f, 30.f);
    m_ParticleMoveAngle[0]->SetHideName("ParticleMoveAngleX");
    m_ParticleMoveAngle[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleMoveAngle[1] = CreateWidget<CEditorInput>("ParticleMoveAngleY", 80.f, 30.f);
    m_ParticleMoveAngle[1]->SetHideName("ParticleMoveAngleY");
    m_ParticleMoveAngle[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleMoveAngle[2] = CreateWidget<CEditorInput>("ParticleMoveAngleZ", 80.f, 30.f);
    m_ParticleMoveAngle[2]->SetHideName("ParticleMoveAngleZ");
    m_ParticleMoveAngle[2]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("MoveDir");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleMoveDir[0] = CreateWidget<CEditorInput>("ParticleMoveDirX", 80.f, 30.f);
    m_ParticleMoveDir[0]->SetHideName("ParticleMoveDirX");
    m_ParticleMoveDir[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleMoveDir[1] = CreateWidget<CEditorInput>("ParticleMoveDirY", 80.f, 30.f);
    m_ParticleMoveDir[1]->SetHideName("ParticleMoveDirY");
    m_ParticleMoveDir[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleMoveDir[2] = CreateWidget<CEditorInput>("ParticleMoveDirZ", 80.f, 30.f);
    m_ParticleMoveDir[2]->SetHideName("ParticleMoveDirZ");
    m_ParticleMoveDir[2]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("StartMin");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleStartMin[0] = CreateWidget<CEditorInput>("ParticleStartMinX", 80.f, 30.f);
    m_ParticleStartMin[0]->SetHideName("ParticleStartMinX");
    m_ParticleStartMin[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleStartMin[1] = CreateWidget<CEditorInput>("ParticleStartMinY", 80.f, 30.f);
    m_ParticleStartMin[1]->SetHideName("ParticleStartMinY");
    m_ParticleStartMin[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleStartMin[2] = CreateWidget<CEditorInput>("ParticleStartMinZ", 80.f, 30.f);
    m_ParticleStartMin[2]->SetHideName("ParticleStartMinZ");
    m_ParticleStartMin[2]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("StartMax");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleStartMax[0] = CreateWidget<CEditorInput>("ParticleStartMaxX", 80.f, 30.f);
    m_ParticleStartMax[0]->SetHideName("ParticleStartMaxX");
    m_ParticleStartMax[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleStartMax[1] = CreateWidget<CEditorInput>("ParticleStartMaxY", 80.f, 30.f);
    m_ParticleStartMax[1]->SetHideName("ParticleStartMaxY");
    m_ParticleStartMax[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleStartMax[2] = CreateWidget<CEditorInput>("ParticleStartMaxZ", 80.f, 30.f);
    m_ParticleStartMax[2]->SetHideName("ParticleStartMaxZ");
    m_ParticleStartMax[2]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("ScaleMin");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleScaleMin[0] = CreateWidget<CEditorInput>("ParticleScaleMinX", 80.f, 30.f);
    m_ParticleScaleMin[0]->SetHideName("ParticleScaleMinX");
    m_ParticleScaleMin[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleScaleMin[1] = CreateWidget<CEditorInput>("ParticleScaleMinY", 80.f, 30.f);
    m_ParticleScaleMin[1]->SetHideName("ParticleScaleMinY");
    m_ParticleScaleMin[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleScaleMin[2] = CreateWidget<CEditorInput>("ParticleScaleMinZ", 80.f, 30.f);
    m_ParticleScaleMin[2]->SetHideName("ParticleScaleMinZ");
    m_ParticleScaleMin[2]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("ScaleMax");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleScaleMax[0] = CreateWidget<CEditorInput>("ParticleScaleMaxX", 80.f, 30.f);
    m_ParticleScaleMax[0]->SetHideName("ParticleScaleMaxX");
    m_ParticleScaleMax[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleScaleMax[1] = CreateWidget<CEditorInput>("ParticleScaleMaxY", 80.f, 30.f);
    m_ParticleScaleMax[1]->SetHideName("ParticleScaleMaxY");
    m_ParticleScaleMax[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleScaleMax[2] = CreateWidget<CEditorInput>("ParticleScaleMaxZ", 80.f, 30.f);
    m_ParticleScaleMax[2]->SetHideName("ParticleScaleMaxZ");
    m_ParticleScaleMax[2]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("ColorMin");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleColorMin[0] = CreateWidget<CEditorInput>("ParticleColorMinX", 80.f, 30.f);
    m_ParticleColorMin[0]->SetHideName("ParticleColorMinX");
    m_ParticleColorMin[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleColorMin[1] = CreateWidget<CEditorInput>("ParticleColorMinY", 80.f, 30.f);
    m_ParticleColorMin[1]->SetHideName("ParticleColorMinY");
    m_ParticleColorMin[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleColorMin[2] = CreateWidget<CEditorInput>("ParticleColorMinZ", 80.f, 30.f);
    m_ParticleColorMin[2]->SetHideName("ParticleColorMinZ");
    m_ParticleColorMin[2]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleColorMin[3] = CreateWidget<CEditorInput>("ParticleColorMinW", 80.f, 30.f);
    m_ParticleColorMin[3]->SetHideName("ParticleColorMinW");
    m_ParticleColorMin[3]->SetInputType(EImGuiInputType::Float);

    label = CreateWidget<CEditorLabel>("ColorMax");
    label->SetColor(255, 0, 0, 255);
    label->SetAlign(0.5f, 0.5f);
    label->SetSize(100.f, 30.f);
    m_ParticleColorMax[0] = CreateWidget<CEditorInput>("ParticleColorMaxX", 80.f, 30.f);
    m_ParticleColorMax[0]->SetHideName("ParticleColorMaxX");
    m_ParticleColorMax[0]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleColorMax[1] = CreateWidget<CEditorInput>("ParticleColorMaxY", 80.f, 30.f);
    m_ParticleColorMax[1]->SetHideName("ParticleColorMaxY");
    m_ParticleColorMax[1]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleColorMax[2] = CreateWidget<CEditorInput>("ParticleColorMaxZ", 80.f, 30.f);
    m_ParticleColorMax[2]->SetHideName("ParticleColorMaxZ");
    m_ParticleColorMax[2]->SetInputType(EImGuiInputType::Float);
    line = CreateWidget<CEditorSameLine>("line");
    m_ParticleColorMax[3] = CreateWidget<CEditorInput>("ParticleColorMaxW", 80.f, 30.f);
    m_ParticleColorMax[3]->SetHideName("ParticleColorMaxW");
    m_ParticleColorMax[3]->SetInputType(EImGuiInputType::Float);

    m_ParticleList = CreateWidget<CEditorListBox>("ParticleListBox");
    m_ParticleList->SetHideName("ParticleListBox");
    m_ParticleList->SetSize(200.f, 300.f);
    m_ParticleList->SetPageItemCount(6);
    m_ParticleList->SetSelectCallback<CParticleWindow>(this, &CParticleWindow::ListClickCallback);
    std::vector<std::string> vecName;
    CResourceManager::GetInst()->GetParticleNames(vecName);
    size_t size = vecName.size();
    for(size_t i=0; i<size;++i)
    {
        m_ParticleList->AddItem(vecName[i]);
    }
    line = CreateWidget<CEditorSameLine>("Line");
    button = CreateWidget<CEditorButton>("로드", 50.f, 30.f);
    button->SetClickCallback<CParticleWindow>(this, &CParticleWindow::ParticleLoadCallback);
    return true;
}

void CParticleWindow::Update(float deltaTime)
{
    CEditorWindow::Update(deltaTime);
    if (m_SelectComponent)
    {
        if (!m_SelectComponent->GetActive())
        {
            m_SelectComponent = nullptr;
        }
    }    
    if (m_SelectParticle)
    {
        if (!m_SelectParticle->GetActive())
        {
            m_SelectParticle = nullptr;
        }
    }
}

void CParticleWindow::ListClickCallback(int index, const std::string& item)
{
    m_SelectListItem = item;
}

void CParticleWindow::ParticleChangeCallback()
{
    if (!m_SelectComponent)
    {
        return;
    }
    m_SelectComponent->SetSpawnTime(m_SpawnTime->GetFloat());
    m_SelectComponent->SetSpawnCountMax(m_SpawnCountMax->GetInt());
    if (!m_SelectParticle)
    {
        return;
    }
    m_SelectParticle->SetName(m_Name->GetText());
    m_SelectParticle->SetParticleEnable(m_ParticleSpawnEnable->GetCheck());
    m_SelectParticle->SetParticleMoveEnable(m_ParticleMoveEnable->GetCheck());
    m_SelectParticle->SetParticleGravityEnable(m_ParticleGravity->GetCheck());
    m_SelectParticle->SetParticleMoveDirEnable(m_ParticleMoveDirEnable->GetCheck());
    m_SelectParticle->SetParticleLifeTimeMin(m_ParticleLifeTimeMin->GetFloat());
    m_SelectParticle->SetParticleLifeTimeMax(m_ParticleLifeTimeMax->GetFloat());

    Vector3 vec3;
    vec3.x = m_ParticleMoveAngle[0]->GetFloat();
    vec3.y = m_ParticleMoveAngle[1]->GetFloat();
    vec3.z = m_ParticleMoveAngle[2]->GetFloat();
    m_SelectParticle->SetParticleMoveAngle(vec3);
    vec3.x = m_ParticleMoveDir[0]->GetFloat();
    vec3.y = m_ParticleMoveDir[1]->GetFloat();
    vec3.z = m_ParticleMoveDir[2]->GetFloat();
    m_SelectParticle->SetParticleMoveDir(vec3);
    vec3.x = m_ParticleStartMin[0]->GetFloat();
    vec3.y = m_ParticleStartMin[1]->GetFloat();
    vec3.z = m_ParticleStartMin[2]->GetFloat();
    m_SelectParticle->SetParticleStartMin(vec3);
    vec3.x = m_ParticleStartMax[0]->GetFloat();
    vec3.y = m_ParticleStartMax[1]->GetFloat();
    vec3.z = m_ParticleStartMax[2]->GetFloat();
    m_SelectParticle->SetParticleStartMax(vec3);
    vec3.x = m_ParticleScaleMin[0]->GetFloat();
    vec3.y = m_ParticleScaleMin[1]->GetFloat();
    vec3.z = m_ParticleScaleMin[2]->GetFloat();
    m_SelectParticle->SetParticleScaleMin(vec3);
    vec3.x = m_ParticleScaleMax[0]->GetFloat();
    vec3.y = m_ParticleScaleMax[1]->GetFloat();
    vec3.z = m_ParticleScaleMax[2]->GetFloat();
    m_SelectParticle->SetParticleScaleMax(vec3);

    Vector4 vec4;
    vec4.x = m_ParticleColorMin[0]->GetFloat();
    vec4.y = m_ParticleColorMin[1]->GetFloat();
    vec4.z = m_ParticleColorMin[2]->GetFloat();
    vec4.w = m_ParticleColorMin[3]->GetFloat();
    m_SelectParticle->SetParticleColorMin(vec4);
    vec4.x = m_ParticleColorMax[0]->GetFloat();
    vec4.y = m_ParticleColorMax[1]->GetFloat();
    vec4.z = m_ParticleColorMax[2]->GetFloat();
    vec4.w = m_ParticleColorMax[3]->GetFloat();
    m_SelectParticle->SetParticleColorMax(vec4);
    m_SelectComponent->SetParticle(m_SelectParticle);
}

void CParticleWindow::ParticleLoadCallback()
{
    if (m_SelectListItem == "")
    {
        return;
    }
    m_SelectComponent->SetParticle(m_SelectListItem);
}