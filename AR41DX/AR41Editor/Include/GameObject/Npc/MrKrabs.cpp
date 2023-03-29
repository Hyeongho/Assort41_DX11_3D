#include "MrKrabs.h"

#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "../../UI/DialogUI.h"
#include "../Object/Common/Collectible/GoldenSpatula.h"
#include "../Player.h"

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
//    CInput::GetInst()->AddBindFunction<CMrKrabs>("F6", Input_Type::Up, this, &CMrKrabs::CreateSpatula, m_Scene);
//#endif // DEBUG

    CInput::GetInst()->AddBindFunction<CMrKrabs>("F", Input_Type::Up, this, &CMrKrabs::StartDialog, m_Scene);

    CreateAnim();
}

bool CMrKrabs::Init()
{
    CNpc::Init();

    m_AnimMesh->SetMesh("MrKrabs");

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

void CMrKrabs::CreateAnim()
{
    if (m_Animation)
        return;

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
        // 플레이어의 반짝이 개수를 체크해 기준 이하면 부족하다는 회화.
        // 충분하면 반짝이를 받고, 뒤집개를 준다.
        int FlowerStand = 500; // 임의로 500개를 교환 기준으로 삼는다.

        CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

        if (Player) {
            if (Player->SubShinyFlower(FlowerStand)) {
                DialogUI->SetCurDialog("Enough_Flower");

                CreateSpatula();
            }
            else {
                DialogUI->SetCurDialog("Not_Enough_Flower");
            }
        }

    }

    DialogUI->OpenDialog();

    m_DialogCount++;
}
