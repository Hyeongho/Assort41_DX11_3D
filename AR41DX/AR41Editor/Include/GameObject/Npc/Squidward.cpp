#include "Squidward.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"
#include "../Object/Common/Collectible/GoldenSpatula.h"

CSquidward::CSquidward()
{
	SetTypeID<CSquidward>();

	m_ObjectTypeName = "Squidward";

    m_DialogCount = 0;
    m_NpcType = ENpcList::Squidward;
    m_NpcMapPos = EMapList::Bikini_Bottom;
    m_EnableDialog = false;
    m_NpcMeshType = MeshType::Animation;
}

CSquidward::CSquidward(const CSquidward& Obj) 
    : CNpc(Obj)
{
    m_AnimMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("SquidwardAnimation");

    m_DialogCount = Obj.m_DialogCount;
    m_NpcType = Obj.m_NpcType;
    m_NpcMapPos = Obj.m_NpcMapPos;
    m_EnableDialog = Obj.m_EnableDialog;
    m_NpcMeshType = Obj.m_NpcMeshType;
}

CSquidward::~CSquidward()
{
}

void CSquidward::Start()
{
    CNpc::Start();

#ifdef _DEBUG
    CInput::GetInst()->AddBindFunction<CSquidward>("F1", Input_Type::Up, this, &CSquidward::SetBandageMode, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F2", Input_Type::Up, this, &CSquidward::ChangeAnim_Intro1, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F3", Input_Type::Up, this, &CSquidward::ChangeAnim_Intro2, m_Scene);
    CInput::GetInst()->AddBindFunction<CSquidward>("F4", Input_Type::Up, this, &CSquidward::ChangeAnim_Outro1, m_Scene);
#endif // DEBUG

    CInput::GetInst()->AddBindFunction<CSquidward>("F", Input_Type::Up, this, &CSquidward::StartDialog, m_Scene);

    CreateAnim();
}

bool CSquidward::Init()
{
    CNpc::Init();

    m_AnimMesh->SetMesh("Squidward");

    Vector3 ColSize = m_AnimMesh->GetMeshSize();

    if (m_AnimMesh->GetMeshSize().x >= m_AnimMesh->GetMeshSize().z)
        ColSize.z = m_AnimMesh->GetMeshSize().x;
    else
        ColSize.x = m_AnimMesh->GetMeshSize().z;

    m_Collider->SetBoxHalfSize(ColSize / 2.f);
    m_Collider->SetRelativePositionY(ColSize.y / 2.f);

    CreateAnim();

    return true;
}

void CSquidward::Update(float DeltaTime)
{
    CNpc::Update(DeltaTime);
}

void CSquidward::PostUpdate(float DeltaTime)
{
    CNpc::PostUpdate(DeltaTime);
}

CSquidward* CSquidward::Clone() const
{
    return new CSquidward(*this);
}

void CSquidward::Save(FILE* File)
{
    CNpc::Save(File);
}

void CSquidward::Load(FILE* File)
{
    CNpc::Load(File);
}

void CSquidward::SetBandageMode()
{
    m_AnimMesh->SetMesh("Squidward_Bandage");
    m_Animation->SetCurrentAnimation("Squidward_Hurt_Start");
}

void CSquidward::StartDialog()
{
    if (!m_EnableDialog)
        return;

    CDialogUI* DialogUI = m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI");

    if (!DialogUI)
        return;

    DialogUI->SetDialogInfo(m_NpcMapPos, m_NpcType);


    if (m_NpcMapPos == EMapList::Bikini_Bottom) {
        if (m_DialogCount == 0)
            DialogUI->SetCurDialog("First_Contact");
        else if (m_DialogCount == 5) {
            DialogUI->SetCurDialog("EasterEgg");

            CreateSpatula();
        }
        else
            DialogUI->SetCurDialog("Contact");

    }
    else if (m_NpcMapPos == EMapList::Jelly_Fish_Field) {
        if (m_DialogCount == 0)
            DialogUI->SetCurDialog("First_Contact");
        else if (m_DialogCount == 1) {
            DialogUI->SetCurDialog("Second_Contact");

            CreateSpatula();
        }
        else
            DialogUI->SetCurDialog("Contact");

    }

    DialogUI->OpenDialog();

    m_DialogCount++;
}

void CSquidward::CreateAnim()
{
    if (m_Animation)
        return;


    m_Animation = m_AnimMesh->SetAnimation<CAnimation>("SquidwardAnimation");

    m_Animation->AddAnimation("Squidward_Angry_Loop", "Squidward_Angry_Loop", 50.f / 60.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Angry_Start", "Squidward_Angry_Start", 16.f / 60.f, 1.f, false);
    m_Animation->AddAnimation("Squidward_Annoyed_Loop", "Squidward_Annoyed_Loop", 60.f / 60.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Annoyed_Start", "Squidward_Annoyed_Start", 10.f / 60.f, 1.f, false);
    m_Animation->AddAnimation("Squidward_Happy_Loop", "Squidward_Happy_Loop", 60.f / 60.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Happy_Start", "Squidward_Happy_Start", 20.f / 60.f, 1.f, false);
    m_Animation->AddAnimation("Squidward_Hurt_Loop", "Squidward_Hurt_Loop", 52.f / 60.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Hurt_Start", "Squidward_Hurt_Start", 18.f / 60.f, 1.f, false);
    m_Animation->AddAnimation("Squidward_Idle", "Squidward_Idle", 40.f / 60.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Sarcastic_Loop", "Squidward_Sarcastic_Loop", 55.f / 60.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Sarcastic_Start", "Squidward_Sarcastic_Start", 23.f / 60.f, 1.f, false);
    m_Animation->AddAnimation("Squidward_Talk", "Squidward_Talk", 100.f / 60.f, 1.f, true);
    m_Animation->AddAnimation("Squidward_Talk_Idle", "Squidward_Talk_Idle", 40.f / 60.f, 1.f, true);

    m_Animation->SetCurrentEndFunction("Squidward_Angry_Start", this, &CSquidward::ChangeAnim_Angry_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Annoyed_Start", this, &CSquidward::ChangeAnim_Annoyed_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Happy_Start", this, &CSquidward::ChangeAnim_Happy_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Hurt_Start", this, &CSquidward::ChangeAnim_Hurt_Loop);
    m_Animation->SetCurrentEndFunction("Squidward_Sarcastic_Start", this, &CSquidward::ChangeAnim_Sarcastic_Loop);

    // CutScene
    m_Animation->AddAnimation("Squidward_Intro1", "Squidward_Intro1", 76.f/ 60.f, 1.f, false);
    m_Animation->AddAnimation("Squidward_Intro2", "Squidward_Intro2", 120.f / 60.f, 1.f, false);
    m_Animation->AddAnimation("Squidward_Outro1", "Squidward_Outro1", 135.f / 60.f, 1.f, false);

    m_Animation->SetCurrentEndFunction("Squidward_Intro1", this, &CSquidward::ChangeAnim_Intro2);
    m_Animation->SetCurrentEndFunction("Squidward_Intro2", this, &CSquidward::ChangeAnim_Intro2);
    m_Animation->SetCurrentEndFunction("Squidward_Outro1", this, &CSquidward::ChangeAnim_Intro2);


    m_Animation->SetCurrentAnimation("Squidward_Idle");
}

void CSquidward::ChangeAnim_Angry_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Angry_Loop");
}

void CSquidward::ChangeAnim_Angry_Start()
{
    m_Animation->ChangeAnimation("Squidward_Angry_Start");
}

void CSquidward::ChangeAnim_Annoyed_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Annoyed_Loop");
}

void CSquidward::ChangeAnim_Annoyed_Start()
{
    m_Animation->ChangeAnimation("Squidward_Annoyed_Start");
}

void CSquidward::ChangeAnim_Happy_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Happy_Loop");
}

void CSquidward::ChangeAnim_Happy_Start()
{
    m_Animation->ChangeAnimation("Squidward_Happy_Start");
}

void CSquidward::ChangeAnim_Hurt_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Hurt_Loop");
}

void CSquidward::ChangeAnim_Hurt_Start()
{
    m_Animation->ChangeAnimation("Squidward_Hurt_Start");
}

void CSquidward::ChangeAnim_Sarcastic_Loop()
{
    m_Animation->ChangeAnimation("Squidward_Sarcastic_Loop");
}

void CSquidward::ChangeAnim_Sarcastic_Start()
{
    m_Animation->ChangeAnimation("Squidward_Sarcastic_Start");
}

void CSquidward::ChangeAnim_Talk()
{
    m_Animation->ChangeAnimation("Squidward_Talk");
}

void CSquidward::ChangeAnim_Talk_Idle()
{
    m_Animation->ChangeAnimation("Squidward_Talk_Idle");
}

void CSquidward::ChangeAnim_Idle()
{
    m_Animation->ChangeAnimation("Squidward_Idle");
}

void CSquidward::ChangeAnim_Intro1()
{
    m_Animation->ChangeAnimation("Squidward_Intro1");
}

void CSquidward::ChangeAnim_Intro2()
{
    m_Animation->ChangeAnimation("Squidward_Intro2");
}

void CSquidward::ChangeAnim_Outro1()
{
    m_Animation->ChangeAnimation("Squidward_Outro1");
}
