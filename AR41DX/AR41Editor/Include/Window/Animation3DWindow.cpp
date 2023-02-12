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
#include "Resource/ResourceManager.h"

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
    , m_ChangeAnimName(nullptr)
    , m_SkeletoneName(nullptr)
    , m_BoneCount(nullptr)
    , m_Loop(nullptr)
    , m_End(nullptr)
    , m_StartTime(nullptr)
    , m_EndTime(nullptr)
    , m_TimeLength(nullptr)
    , m_FrameTime(nullptr)
    , m_PlayTime(nullptr)
    , m_PlayScale(nullptr)
    , m_StartFrame(nullptr)
    , m_EndFrame(nullptr)
    , m_FrameLength(nullptr)
    , m_FrameMode(nullptr)
    , m_ChangeFrame(nullptr)
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
        m_AnimationName->SetText(m_SelectAnimation->GetAnimationClassName().c_str());
        m_BlendScale[0]->SetFloat(m_SelectAnimation->GetBlendScale().x);
        m_BlendScale[1]->SetFloat(m_SelectAnimation->GetBlendScale().y);
        m_BlendScale[2]->SetFloat(m_SelectAnimation->GetBlendScale().z);
        m_BlendPos[0]->SetFloat(m_SelectAnimation->GetBlendPos().x);
        m_BlendPos[1]->SetFloat(m_SelectAnimation->GetBlendPos().y);
        m_BlendPos[2]->SetFloat(m_SelectAnimation->GetBlendPos().z);
        m_BlendRot[0]->SetFloat(m_SelectAnimation->GetBlendRot().x);
        m_BlendRot[1]->SetFloat(m_SelectAnimation->GetBlendRot().y);
        m_BlendRot[2]->SetFloat(m_SelectAnimation->GetBlendRot().z);
        m_BlendRot[3]->SetFloat(m_SelectAnimation->GetBlendRot().z);
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
        m_SkeletoneName->SetText(m_SelectSkeleton->GetName().c_str());
        m_BoneCount->SetInt((int)m_SelectSkeleton->GetBoneCount());
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
    category->AddItem(nullptr, "Sequence", "Animation");
    category->AddItem(nullptr, "Bone", "Animation3D");
    //현재 애니메이션 정보
    m_CurAnimName = category->CreateWidget<CEditorInput>("CurAnim","현재 애니메이션", 100.f, 30.f);
    //다음 애니메이션 정보
    m_ChangeAnimName = category->CreateWidget<CEditorInput>("ChangeAnim", "다음 애니메이션", 100.f, 30.f);
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
    m_BlendPos[2] = category->CreateWidget<CEditorInput>("BlendPos", "BlendPosZ", 80.f, 30.f);
    m_BlendPos[2]->SetHideName("BlendPosZ");
    m_BlendPos[2]->SetInputType(EImGuiInputType::Float);

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
    m_AnimName = category->CreateWidget<CEditorInput>("Animation", "애니메이션", 100.f, 30.f);
    m_AnimLoop = category->CreateWidget<CEditorCheckBox>("Animation", "AnimLoop");
    category->CreateWidget<CEditorSameLine>("Animation", "Line");
    m_AnimReverse = category->CreateWidget<CEditorCheckBox>("Animation", "AnimReverse");
    m_AnimTime = category->CreateWidget<CEditorInput>("Animation", "AnimTime", 80.f, 30.f);
    m_AnimTime->SetInputType(EImGuiInputType::Float);
    m_AnimFrameTime = category->CreateWidget<CEditorInput>("Animation", "AnimFrameTime", 80.f, 30.f);
    m_AnimFrameTime->SetInputType(EImGuiInputType::Float);
    m_AnimPlayTime = category->CreateWidget<CEditorInput>("Animation", "AnimPlayTime", 80.f, 30.f);
    m_AnimPlayTime->SetInputType(EImGuiInputType::Float);
    m_AnimPlayScale = category->CreateWidget<CEditorInput>("Animation", "AnimPlayScale", 80.f, 30.f);
    m_AnimPlayScale->SetInputType(EImGuiInputType::Float);
    //시퀸스 정보 Sequence
    m_SequenceAnimName = category->CreateWidget<CEditorInput>("Sequence", "애니메이션 시퀸스", 120.f, 30.f);
    category->CreateWidget<CEditorSameLine>("Sequence", "Line");
    CEditorButton* button = category->CreateWidget<CEditorButton>("Sequence", "변경", 50.f, 30.f);
    button->SetClickCallback<CAnimation3DWindow>(this, &CAnimation3DWindow::SequenceChangeInfo);
    category->CreateWidget<CEditorSameLine>("Sequence", "Line");
    button = category->CreateWidget<CEditorButton>("Sequence", "저장", 50.f, 30.f);
    button->SetClickCallback<CAnimation3DWindow>(this, &CAnimation3DWindow::SequenceSaveInfo);
    m_Loop = category->CreateWidget<CEditorCheckBox>("Sequence", "Loop");
    category->CreateWidget<CEditorSameLine>("Sequence", "Line");
    m_End = category->CreateWidget<CEditorCheckBox>("Sequence", "End");
    m_StartTime = category->CreateWidget<CEditorInput>("Sequence", "StartTime", 80.f, 30.f);
    m_StartTime->SetInputType(EImGuiInputType::Float);
    m_EndTime = category->CreateWidget<CEditorInput>("Sequence", "EndTime", 80.f, 30.f);
    m_EndTime->SetInputType(EImGuiInputType::Float);
    m_TimeLength = category->CreateWidget<CEditorInput>("Sequence", "TimeLength", 80.f, 30.f);
    m_TimeLength->SetInputType(EImGuiInputType::Float);
    m_FrameTime = category->CreateWidget<CEditorInput>("Sequence", "FrameTime", 80.f, 30.f);
    m_FrameTime->SetInputType(EImGuiInputType::Float);
    m_PlayTime = category->CreateWidget<CEditorInput>("Sequence", "PlayTime", 80.f, 30.f);
    m_PlayTime->SetInputType(EImGuiInputType::Float);
    m_PlayScale = category->CreateWidget<CEditorInput>("Sequence", "PlayScale", 80.f, 30.f);
    m_PlayScale->SetInputType(EImGuiInputType::Float);
    m_StartFrame = category->CreateWidget<CEditorInput>("Sequence", "StartFrame", 100.f, 30.f);
    m_StartFrame->SetInputType(EImGuiInputType::Int);
    m_EndFrame = category->CreateWidget<CEditorInput>("Sequence", "EndFrame", 100.f, 30.f);
    m_EndFrame->SetInputType(EImGuiInputType::Int);
    m_FrameLength = category->CreateWidget<CEditorInput>("Sequence", "FrameLength", 100.f, 30.f);
    m_FrameLength->SetInputType(EImGuiInputType::Int);
    m_FrameMode = category->CreateWidget<CEditorInput>("Sequence", "FrameMode", 100.f, 30.f);
    m_FrameMode->SetInputType(EImGuiInputType::Int);
    m_ChangeFrame = category->CreateWidget<CEditorInput>("Sequence", "ChangeFrame", 100.f, 30.f);
    m_ChangeFrame->SetInputType(EImGuiInputType::Int);
    //스켈레톤
    m_SkeletoneName = category->CreateWidget<CEditorInput>("Bone", "스켈레톤 이름", 120.f, 30.f);
    category->CreateWidget<CEditorSameLine>("Bone", "Line");
    m_BoneCount = category->CreateWidget<CEditorInput>("Bone", "BoneCount", 100.f, 30.f);
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
        else
        {
            UpdateInfo();
        }
    }
    if (m_SelectSkeleton)
    {
        if (!m_SelectSkeleton->GetActive())
        {
            m_SelectSkeleton = nullptr;
        }
    }
    if (m_SelectAnimationSequence)
    {
        if (!m_SelectAnimationSequence->GetActive())
        {
            m_SelectAnimationSequence = nullptr;
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
    m_SelectAnimationSequence = nullptr;
}

void CAnimation3DWindow::UpdateInfo()
{
    CAnimationData* curAnimData= m_SelectAnimation->GetCurrentAnimation();
    if(curAnimData)
    {
        m_CurAnimName->SetText(curAnimData->GetName().c_str());
    }
    else
    {
        m_CurAnimName->SetText("");
    }
    CAnimationData* changeAnimData = m_SelectAnimation->GetChangeAnimation();
    if(changeAnimData)
    {
        m_ChangeAnimName->SetText(changeAnimData->GetName().c_str());
    }
    else
    {
        m_ChangeAnimName->SetText("");
    }
    m_GlobalTime->SetFloat(m_SelectAnimation->GetAnimationGlobalTime());
    m_SequenceProgress->SetFloat(m_SelectAnimation->GetAnimationSequenceProgress());
    m_ChangeTimeAcc->SetFloat(m_SelectAnimation->GetAnimationChangeTimeAcc());
    m_ChangeTime->SetFloat(m_SelectAnimation->GetAnimationChangeTime());
    m_Play->SetCheck(m_SelectAnimation->GetAnimationPlay());
}

void CAnimation3DWindow::SequenceChangeInfo()
{
    if (!m_SelectAnimationSequence)
    {
        return;
    }
    m_SelectAnimationSequence->SetName(m_SequenceAnimName->GetText());
    m_SelectAnimationSequence->SetPlayTime(m_PlayTime->GetFloat());
    m_SelectAnimationSequence->SetPlayScale(m_PlayScale->GetFloat());
}

void CAnimation3DWindow::SequenceSaveInfo()
{
    if (!m_SelectAnimationSequence)
    {
        return;
    }
    char	animFullPath[MAX_PATH] = {};
    strcpy_s(animFullPath, m_SequenceAnimName->GetText());
    strcat_s(animFullPath,".sqc");
    m_SelectAnimationSequence->SaveMultibyte(animFullPath,ANIMATION_PATH);
}

void CAnimation3DWindow::AnimTreeCallback(CEditorTreeItem<class CAnimationData*>* node, const std::string& item)
{
    CAnimationData* animData = node->GetCustomData();
    if (!animData)
    {
        return;
    }
    m_AnimName->SetText(animData->GetName().c_str());
    m_SequenceAnimName->SetText(animData->GetSequenceName().c_str());
    m_AnimTime->SetFloat(animData->GetAnimationTime());
    m_AnimFrameTime->SetFloat(animData->GetAnimationFrameTime());
    m_AnimPlayTime->SetFloat(animData->GetAnimationPlayTime());
    m_AnimPlayScale->SetFloat(animData->GetAnimationPlayScale());
    m_AnimLoop->SetCheck(animData->GetAnimationLoop());
    m_AnimReverse->SetCheck(animData->GetAnimationReverse());
    //시퀸스
    m_SelectAnimationSequence = animData->GetAnimationSequence();
    if(!m_SelectAnimationSequence)
    {
        return;
    }
    m_Loop->SetCheck(m_SelectAnimationSequence->GetSequenceLoop());
    m_End->SetCheck(m_SelectAnimationSequence->GetSequenceEnd());
    m_StartTime->SetFloat(m_SelectAnimationSequence->GetSequenceStartTime());
    m_EndTime->SetFloat(m_SelectAnimationSequence->GetSequenceEndTime());
    m_TimeLength->SetFloat(m_SelectAnimationSequence->GetSequenceTimeLength());
    m_FrameTime->SetFloat(m_SelectAnimationSequence->GetSequenceFrameTime());
    m_PlayTime->SetFloat(m_SelectAnimationSequence->GetSequencePlayTime());
    m_PlayScale->SetFloat(m_SelectAnimationSequence->GetSequencePlayScale());
    m_StartFrame->SetInt(m_SelectAnimationSequence->GetSequenceStartFrame());
    m_EndFrame->SetInt(m_SelectAnimationSequence->GetSequenceEndFrame());
    m_FrameLength->SetInt(m_SelectAnimationSequence->GetKeyFrameCount());
    m_FrameMode->SetInt(m_SelectAnimationSequence->GetSequenceFrameMode());
    m_ChangeFrame->SetInt(m_SelectAnimationSequence->GetSequenceChangeFrame());
}

void CAnimation3DWindow::ChangeAnimation3D(const TCHAR* path)
{
    if(!m_SelectAnimation)
    {
        return;
    }
    char name[256];
    char ext[256];
    WideCharToMultiByte(CP_ACP, 0, path, _MAX_EXT, name, _MAX_EXT, NULL, NULL);
    _splitpath_s(name, nullptr, 0, nullptr, 0, name, _MAX_EXT, ext, 256);
    CResourceManager::GetInst()->LoadAnimationSequenceFullPath(name, path);
    m_SelectAnimation->AddAnimation(name, name, 1.f, 1.f, true);
    m_SelectAnimation->SetCurrentAnimation(name);

}