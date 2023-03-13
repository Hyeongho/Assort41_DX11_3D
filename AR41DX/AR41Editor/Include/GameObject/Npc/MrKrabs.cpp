#include "MrKrabs.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"

CMrKrabs::CMrKrabs() 
{
    SetTypeID<CMrKrabs>();

    m_ObjectTypeName = "MrKrabs";

    m_DialogCount = 0;
    m_NpcType = ENpcList::MrKrabs;
    m_NpcMapPos = EMapList::Bikini_Bottom;
    m_EnableDialog = false;
    m_NpcMeshType = MeshType::Animation;
}

CMrKrabs::CMrKrabs(const CMrKrabs& Obj) : CNpc(Obj)
{
    m_AnimMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("MrKrabsAnimation");

    m_DialogCount = Obj.m_DialogCount;
    m_NpcType = Obj.m_NpcType;
    m_NpcMapPos = Obj.m_NpcMapPos;
    m_EnableDialog = Obj.m_EnableDialog;
    m_NpcMeshType = Obj.m_NpcMeshType;
}

CMrKrabs::~CMrKrabs()
{

}

void CMrKrabs::Start()
{
    CNpc::Start();

//#ifdef _DEBUG
//    CInput::GetInst()->AddBindFunction<CMrKrabs>("F1", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Angry_Start, m_Scene);
//    CInput::GetInst()->AddBindFunction<CMrKrabs>("F2", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Deceptive_Start, m_Scene);
//    CInput::GetInst()->AddBindFunction<CMrKrabs>("F3", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Greedy_Start, m_Scene);
//    CInput::GetInst()->AddBindFunction<CMrKrabs>("F4", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Laughing, m_Scene);
//    CInput::GetInst()->AddBindFunction<CMrKrabs>("F5", Input_Type::Up, this, &CMrKrabs::ChangeAnim_Idle, m_Scene);
//#endif // DEBUG

    CInput::GetInst()->AddBindFunction<CMrKrabs>("F", Input_Type::Up, this, &CMrKrabs::StartDialog, m_Scene);
}

bool CMrKrabs::Init()
{
    CNpc::Init();

    m_AnimMesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_AnimMesh);

    m_AnimMesh->SetMesh("MrKrabs");

    m_Animation = m_AnimMesh->SetAnimation<CAnimation>("MrKrabsAnimation");

    m_Animation->AddAnimation("MrKrabs_Angry_Loop", "MrKrabs_Angry_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Angry_Start", "MrKrabs_Angry_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Deceptive_Loop", "MrKrabs_Deceptive_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Deceptive_Start", "MrKrabs_Deceptive_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Greedy_Loop", "MrKrabs_Greedy_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Greedy_Start", "MrKrabs_Greedy_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Laughing", "MrKrabs_Laughing", 1.f, 1.f, true);
    m_Animation->AddAnimation("MrKrabs_Idle", "MrKrabs_Idle", 1.f, 1.f, true);

    m_Animation->SetCurrentEndFunction("MrKrabs_Angry_Start", this, &CMrKrabs::ChangeAnim_Angry_Loop);
    m_Animation->SetCurrentEndFunction("MrKrabs_Deceptive_Start", this, &CMrKrabs::ChangeAnim_Deceptive_Loop);
    m_Animation->SetCurrentEndFunction("MrKrabs_Greedy_Start", this, &CMrKrabs::ChangeAnim_Greedy_Loop);

    m_Animation->SetCurrentAnimation("MrKrabs_Idle");

    return true;
}

void CMrKrabs::Update(float DeltaTime)
{
    CNpc::Update(DeltaTime);
}

void CMrKrabs::PostUpdate(float DeltaTime)
{
    CNpc::PostUpdate(DeltaTime);
}

CMrKrabs* CMrKrabs::Clone() const
{
    return new CMrKrabs(*this);
}

void CMrKrabs::Save(FILE* File)
{
    CNpc::Save(File);
}

void CMrKrabs::Load(FILE* File)
{
    CNpc::Load(File);
}

void CMrKrabs::ChangeAnimByName(const std::string& Name)
{
    if (!m_Animation->FindAnimation(Name))
        return;

    m_Animation->ChangeAnimation(Name);
}

void CMrKrabs::ChangeAnim_Angry_Loop()
{
    m_Animation->ChangeAnimation("MrKrabs_Angry_Loop");
}

void CMrKrabs::ChangeAnim_Angry_Start()
{
    m_Animation->ChangeAnimation("MrKrabs_Angry_Start");
}

void CMrKrabs::ChangeAnim_Deceptive_Loop()
{
    m_Animation->ChangeAnimation("MrKrabs_Deceptive_Loop");
}

void CMrKrabs::ChangeAnim_Deceptive_Start()
{
    m_Animation->ChangeAnimation("MrKrabs_Deceptive_Start");
}

void CMrKrabs::ChangeAnim_Greedy_Loop()
{
    m_Animation->ChangeAnimation("MrKrabs_Greedy_Loop");
}

void CMrKrabs::ChangeAnim_Greedy_Start()
{
    m_Animation->ChangeAnimation("MrKrabs_Greedy_Start");
}

void CMrKrabs::ChangeAnim_Laughing()
{
    m_Animation->ChangeAnimation("MrKrabs_Laughing");
}

void CMrKrabs::ChangeAnim_Idle()
{
    m_Animation->ChangeAnimation("MrKrabs_Idle");
}

void CMrKrabs::StartDialog()
{
    if (!m_EnableDialog)
        return;

    CDialogUI* DialogUI = m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI");

    if (!DialogUI)
        return;

    DialogUI->SetDialogInfo(m_NpcMapPos, m_NpcType);

    if(m_DialogCount == 0)
        DialogUI->SetCurDialog("First_Contact");
    else {
        // 추후 플레이어로부터 플라워 카운트를 받아 체크. 
        int FlowerCount = 500;
        // CPlayer* Player = m_Scene->FindObject<CPlayer>("Player");
        // int FlowerCount = Player->GetFlowerCount();

        if (FlowerCount >= 500)
            DialogUI->SetCurDialog("Enough_Flower");
        else
            DialogUI->SetCurDialog("Not_Enough_Flower");
    }

    DialogUI->OpenDialog();

    m_DialogCount++;
}
