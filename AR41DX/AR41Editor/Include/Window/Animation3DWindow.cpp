#include "Animation3DWindow.h"
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
#include "Component/AnimationMeshComponent.h"
#include "Editor/EditorGroup.h" //이게뭐냐

CAnimation3DWindow::CAnimation3DWindow()
    : m_BlendScale{}
    , m_BlendPos{}
    , m_BlendRot{}
    , m_AnimationName(nullptr)
    , m_GlobalTime(nullptr)
    , m_SequenceProgress(nullptr)
    , m_ChangeTimeAcc(nullptr)
    , m_ChangeTime(nullptr)
    , m_Play(nullptr)
    , m_AnimTree(nullptr)
    , m_AnimName(nullptr)
    , m_SequenceAnimName(nullptr)
    , m_AnimTime(nullptr)
    , m_AnimFrameTime(nullptr)
    , m_AnimPlayTime(nullptr)
    , m_AnimPlayScale(nullptr)
    , m_AnimLoop(nullptr)
    , m_AnimReverse(nullptr)
    , m_CurAnimName(nullptr)
    , m_CurSequenceAnimName(nullptr)
    , m_CurAnimTime(nullptr)
    , m_CurAnimFrameTime(nullptr)
    , m_CurAnimPlayTime(nullptr)
    , m_CurAnimPlayScale(nullptr)
    , m_CurAnimLoop(nullptr)
    , m_CurAnimReverse(nullptr)
    , m_ChangeAnimName(nullptr)
    , m_ChangeSequenceAnimName(nullptr)
    , m_ChangeAnimTime(nullptr)
    , m_ChangeAnimFrameTime(nullptr)
    , m_ChangeAnimPlayTime(nullptr)
    , m_ChangeAnimPlayScale(nullptr)
    , m_ChangeAnimLoop(nullptr)
    , m_ChangeAnimReverse(nullptr)
    , m_SkeletoneName(nullptr)
    , m_BoneCount(nullptr)
    , m_SelectAnimationData(nullptr)
{
    
}

CAnimation3DWindow::~CAnimation3DWindow()
{
}

void CAnimation3DWindow::SetSelectComponent(CAnimationMeshComponent* component)
{
    m_SelectAnimation =component->GetAnimation();
    m_SelectSkeleton =component->GetSkeleton();
    //정보 받아오기
    if (m_SelectAnimation)
    {
        UpdateInfo();
        Clear();
        int size = m_SelectAnimation->GetAnimationCount();
        std::vector<std::string>	vecName;
        m_SelectAnimation->GetAnimationNames(vecName);
        for (int i = 0; i < size; ++i)
        {
            AddItem(m_SelectAnimation->FindAnimation(vecName[i]), vecName[i]);
        }
    }
    if (m_SelectSkeleton)
    {

    }
}

bool CAnimation3DWindow::Init()
{
    CEditorTree<void*>* category = CreateWidget<CEditorTree<void*>>("Animation3D");
    category->SetHideName("Animation3D");
    category->SetSize(400.f, 300.f);
    category->AddItem(nullptr, "Animation3D");
    category->AddItem(nullptr, "CurAnim", "Animation3D");
    category->AddItem(nullptr, "ChangeAnim", "Animation3D");
    category->AddItem(nullptr, "Animation", "Animation3D");
    category->AddItem(nullptr, "BlendScale", "Animation");
    category->AddItem(nullptr, "BlendPos", "Animation");
    category->AddItem(nullptr, "BlendRot", "Animation");
    category->AddItem(nullptr, "Bone", "Animation3D");
    //현재 애니메이션 정보
    m_CurAnimLoop = category->CreateWidget<CEditorCheckBox>("CurAnim","ChangeAnimLoop");
    category->CreateWidget<CEditorSameLine>("CurAnim", "Line");
    m_CurAnimReverse = category->CreateWidget<CEditorCheckBox>("CurAnim","CurAnimReverse");
    m_CurAnimName = category->CreateWidget<CEditorInput>("CurAnim","현재 애니메이션", 100.f, 30.f);
    m_CurSequenceAnimName = category->CreateWidget<CEditorInput>("CurAnim", "현재 애니메이션 시퀸스", 120.f, 30.f);
    m_CurAnimTime = category->CreateWidget<CEditorInput>("CurAnim", "CurAnimTime", 80.f, 30.f);
    m_CurAnimTime->SetInputType(EImGuiInputType::Float);
    m_CurAnimFrameTime = category->CreateWidget<CEditorInput>("CurAnim", "CurAnimFrameTime", 80.f, 30.f);
    m_CurAnimFrameTime->SetInputType(EImGuiInputType::Float);
    m_CurAnimPlayTime = category->CreateWidget<CEditorInput>("CurAnim", "CurAnimPlayTime", 80.f, 30.f);
    m_CurAnimPlayTime->SetInputType(EImGuiInputType::Float);
    m_CurAnimPlayScale = category->CreateWidget<CEditorInput>("CurAnim", "CurAnimPlayScale", 80.f, 30.f);
    m_CurAnimPlayScale->SetInputType(EImGuiInputType::Float);
    //다음 애니메이션 정보
    m_ChangeAnimLoop = category->CreateWidget<CEditorCheckBox>("ChangeAnim","ChangeAnimLoop");
    category->CreateWidget<CEditorSameLine>("ChangeAnim", "Line");
    m_ChangeAnimReverse = category->CreateWidget<CEditorCheckBox>("ChangeAnim", "ChangeAnimReverse");
    m_ChangeAnimName = category->CreateWidget<CEditorInput>("ChangeAnim", "다음 애니메이션", 100.f, 30.f);
    m_ChangeSequenceAnimName = category->CreateWidget<CEditorInput>("ChangeAnim", "다음 애니메이션 시퀸스", 120.f, 30.f);
    m_ChangeAnimTime = category->CreateWidget<CEditorInput>("ChangeAnim", "ChangeAnimTime", 80.f, 30.f);
    m_ChangeAnimTime->SetInputType(EImGuiInputType::Float);
    m_ChangeAnimFrameTime = category->CreateWidget<CEditorInput>("ChangeAnim", "ChangeAnimFrameTime", 80.f, 30.f);
    m_ChangeAnimFrameTime->SetInputType(EImGuiInputType::Float);
    m_ChangeAnimPlayTime = category->CreateWidget<CEditorInput>("ChangeAnim", "ChangeAnimPlayTime", 80.f, 30.f);
    m_ChangeAnimPlayTime->SetInputType(EImGuiInputType::Float);
    m_ChangeAnimPlayScale = category->CreateWidget<CEditorInput>("ChangeAnim", "ChangeAnimPlayScale", 80.f, 30.f);
    m_ChangeAnimPlayScale->SetInputType(EImGuiInputType::Float);
    //애니메이션 
    m_AnimationName = category->CreateWidget<CEditorInput>("Animation","애니메이터 이름", 100.f, 30.f);
    category->CreateWidget<CEditorSameLine>("Animation", "Line");
    m_Play = category->CreateWidget<CEditorCheckBox>("Animation", "Play");
    m_GlobalTime = category->CreateWidget<CEditorInput>("Animation", "GlobalTime", 80.f, 30.f);
    m_GlobalTime->SetInputType(EImGuiInputType::Float);
    m_SequenceProgress = category->CreateWidget<CEditorInput>("Animation", "SequenceProgress", 80.f, 30.f);
    m_SequenceProgress->SetInputType(EImGuiInputType::Float);
    m_ChangeTimeAcc = category->CreateWidget<CEditorInput>("Animation", "ChangeTimeAcc", 80.f, 30.f);
    m_ChangeTimeAcc->SetInputType(EImGuiInputType::Float);
    m_ChangeTime = category->CreateWidget<CEditorInput>("Animation", "ChangeTime", 80.f, 30.f);
    m_ChangeTime->SetInputType(EImGuiInputType::Float);

    m_BlendScale[0] = category->CreateWidget<CEditorInput>("BlendScale","BlendScaleX", 80.f, 30.f);
    m_BlendScale[0]->SetHideName("BlendScaleX");
    m_BlendScale[0]->SetInputType(EImGuiInputType::Float);
    category->CreateWidget<CEditorSameLine>("BlendScale", "Line");
    m_BlendScale[1] = category->CreateWidget<CEditorInput>("BlendScale", "BlendScaleY", 80.f, 30.f);
    m_BlendScale[1]->SetHideName("BlendScaleY");
    m_BlendScale[1]->SetInputType(EImGuiInputType::Float);
    category->CreateWidget<CEditorSameLine>("BlendScale", "Line");
    m_BlendScale[2] = category->CreateWidget<CEditorInput>("BlendScale", "BlendScaleZ", 80.f, 30.f);
    m_BlendScale[2]->SetHideName("BlendScaleZ");
    m_BlendScale[2]->SetInputType(EImGuiInputType::Float);

    m_BlendPos[0] = category->CreateWidget<CEditorInput>("BlendPos", "BlendPosX", 80.f, 30.f);
    m_BlendPos[0]->SetHideName("BlendPosX");
    m_BlendPos[0]->SetInputType(EImGuiInputType::Float);
    category->CreateWidget<CEditorSameLine>("BlendPos", "Line");
    m_BlendPos[1] = category->CreateWidget<CEditorInput>("BlendPos", "BlendPosY", 80.f, 30.f);
    m_BlendPos[1]->SetHideName("BlendPosY");
    m_BlendPos[1]->SetInputType(EImGuiInputType::Float);
    category->CreateWidget<CEditorSameLine>("BlendPos", "Line");
    m_BlendScale[2] = category->CreateWidget<CEditorInput>("BlendPos", "BlendPosZ", 80.f, 30.f);
    m_BlendScale[2]->SetHideName("BlendPosZ");
    m_BlendScale[2]->SetInputType(EImGuiInputType::Float);

    m_BlendRot[0] = category->CreateWidget<CEditorInput>("BlendRot", "BlendRotX", 80.f, 30.f);
    m_BlendRot[0]->SetHideName("BlendRotX");
    m_BlendRot[0]->SetInputType(EImGuiInputType::Float);
    category->CreateWidget<CEditorSameLine>("BlendRot", "Line");
    m_BlendRot[1] = category->CreateWidget<CEditorInput>("BlendRot", "BlendRotY", 80.f, 30.f);
    m_BlendRot[1]->SetHideName("BlendRotY");
    m_BlendRot[1]->SetInputType(EImGuiInputType::Float);
    category->CreateWidget<CEditorSameLine>("BlendRot", "Line");
    m_BlendRot[2] = category->CreateWidget<CEditorInput>("BlendRot", "BlendRotZ", 80.f, 30.f);
    m_BlendRot[2]->SetHideName("BlendRotZ");
    m_BlendRot[2]->SetInputType(EImGuiInputType::Float);
    category->CreateWidget<CEditorSameLine>("BlendRot", "Line");
    m_BlendRot[3] = category->CreateWidget<CEditorInput>("BlendRot", "BlendRotW", 80.f, 30.f);
    m_BlendRot[3]->SetHideName("BlendRotW");
    m_BlendRot[3]->SetInputType(EImGuiInputType::Float); 
    //애니메이션의 세부 정보
    m_AnimTree = category->CreateWidget<CEditorTree<CAnimationData*>>("Animation", "Animation3DWindow");
    m_AnimTree->SetHideName("Animation3DWindow");
    m_AnimTree->UseDragDropSelf(true);
    m_AnimTree->SetSelectCallback<CAnimation3DWindow>(this, &CAnimation3DWindow::AnimTreeCallback);
    m_AnimTree->SetSize(400.f, 300.f);
    m_AnimTree->AddItem(nullptr, "AnimationData");
    m_AnimLoop = category->CreateWidget<CEditorCheckBox>("Animation", "AnimLoop");
    category->CreateWidget<CEditorSameLine>("Animation", "Line");
    m_AnimReverse = category->CreateWidget<CEditorCheckBox>("Animation", "AnimReverse");
    category->CreateWidget<CEditorSameLine>("Animation", "Line");
    CEditorButton* button = category->CreateWidget<CEditorButton>("Animation", "변경", 50.f, 30.f);
    button->SetClickCallback<CAnimation3DWindow>(this, &CAnimation3DWindow::ChangeInfo);
    m_AnimName = category->CreateWidget<CEditorInput>("Animation", "애니메이션", 100.f, 30.f);
    m_SequenceAnimName = category->CreateWidget<CEditorInput>("Animation", "애니메이션 시퀸스", 120.f, 30.f);
    m_AnimTime = category->CreateWidget<CEditorInput>("Animation", "AnimTime", 80.f, 30.f);
    m_AnimTime->SetInputType(EImGuiInputType::Float);
    m_AnimFrameTime = category->CreateWidget<CEditorInput>("Animation", "AnimFrameTime", 80.f, 30.f);
    m_AnimFrameTime->SetInputType(EImGuiInputType::Float);
    m_AnimPlayTime = category->CreateWidget<CEditorInput>("Animation", "AnimPlayTime", 80.f, 30.f);
    m_AnimPlayTime->SetInputType(EImGuiInputType::Float);
    m_AnimPlayScale = category->CreateWidget<CEditorInput>("Animation", "AnimPlayScale", 80.f, 30.f);
    m_AnimPlayScale->SetInputType(EImGuiInputType::Float);
    //스켈레톤
    m_SkeletoneName = category->CreateWidget<CEditorInput>("Bone", "스켈레톤 이름", 100.f, 30.f);
    category->CreateWidget<CEditorSameLine>("Bone", "Line");
    m_BoneCount = category->CreateWidget<CEditorInput>("Bone", "BoneCount", 80.f, 30.f);
    m_BoneCount->SetInputType(EImGuiInputType::Int);
    return true;
}

void CAnimation3DWindow::Update(float deltaTime)
{
    CEditorWindow::Update(deltaTime);
    if (m_SelectAnimation)
    {
        if (!m_SelectAnimation->GetActive())
        {
            m_SelectAnimation = nullptr;
        }
    }
    if (m_SelectSkeleton)
    {
        if (!m_SelectSkeleton->GetActive())
        {
            m_SelectSkeleton = nullptr;
        }
    }
}

bool CAnimation3DWindow::AddItem(CAnimationData* data, const std::string& name, const std::string& parentName)
{
    return m_AnimTree->AddItem(data, name, parentName);
}

void CAnimation3DWindow::Clear()
{
    m_AnimTree->Clear();
    m_AnimTree->AddItem(nullptr, "AnimationData");
}

void CAnimation3DWindow::ClearSelect()
{
    m_SelectAnimation = nullptr;
    m_SelectSkeleton = nullptr;
    m_SelectAnimationData = nullptr;
}

void CAnimation3DWindow::UpdateInfo()
{
    CAnimationData* curAnimData= m_SelectAnimation->GetCurrentAnimation();
    curAnimData->GetName();
    //curAnimData->Get();
}

void CAnimation3DWindow::ChangeInfo()
{
}

void CAnimation3DWindow::AnimTreeCallback(CEditorTreeItem<class CAnimationData*>* node, const std::string& item)
{
    m_SelectAnimationData = node->GetCustomData();
    if (!m_SelectAnimationData)
    {
        return;
    }
}

void CAnimation3DWindow::ChangeAnimation3D()
{
}