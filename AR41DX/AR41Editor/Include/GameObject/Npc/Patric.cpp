#include "Patric.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"
#include "../Object/Common/Collectible/Sock.h"

CPatric::CPatric()
{
	SetTypeID<CPatric>();

	m_ObjectTypeName = "Patric";

    m_DialogCount = 0;
    m_NpcType = ENpcList::Patric;
    m_NpcMapPos = EMapList::Bikini_Bottom;
    m_EnableDialog = false;
    m_NpcMeshType = MeshType::Animation;
}

CPatric::CPatric(const CPatric& Obj)
	: CNpc(Obj)
{
    m_AnimMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("PatricNpcAnimation");

    m_DialogCount = Obj.m_DialogCount;
    m_NpcType = Obj.m_NpcType;
    m_NpcMapPos = Obj.m_NpcMapPos;
    m_EnableDialog = Obj.m_EnableDialog;
    m_NpcMeshType = Obj.m_NpcMeshType;
}

CPatric::~CPatric()
{
}

void CPatric::Start()
{
    CNpc::Start();

//#ifdef _DEBUG
//    CInput::GetInst()->AddBindFunction<CPatric>("F1", Input_Type::Up, this, &CPatric::ChangeAnim_Confused, m_Scene);
//    CInput::GetInst()->AddBindFunction<CPatric>("F2", Input_Type::Up, this, &CPatric::ChangeAnim_Default, m_Scene);
//    CInput::GetInst()->AddBindFunction<CPatric>("F3", Input_Type::Up, this, &CPatric::ChangeAnim_Excited, m_Scene);
//    CInput::GetInst()->AddBindFunction<CPatric>("F4", Input_Type::Up, this, &CPatric::ChangeAnim_Scowl_Start, m_Scene);
//    CInput::GetInst()->AddBindFunction<CPatric>("F5", Input_Type::Up, this, &CPatric::ChangeAnim_Talk, m_Scene);
//    CInput::GetInst()->AddBindFunction<CPatric>("F6", Input_Type::Up, this, &CPatric::ChangeAnim_Thinking_Start, m_Scene);
//    CInput::GetInst()->AddBindFunction<CPatric>("F7", Input_Type::Up, this, &CPatric::CreateSock, m_Scene);
//#endif // DEBUG

    CInput::GetInst()->AddBindFunction<CPatric>("F", Input_Type::Up, this, &CPatric::StartDialog, m_Scene);
}

bool CPatric::Init()
{
    CNpc::Init();

    m_AnimMesh->SetMesh("Patric_Npc");

    Vector3 ColSize = m_AnimMesh->GetMeshSize();

    if (m_AnimMesh->GetMeshSize().x >= m_AnimMesh->GetMeshSize().z)
        ColSize.z = m_AnimMesh->GetMeshSize().x;
    else
        ColSize.x = m_AnimMesh->GetMeshSize().z;

    m_Collider->SetBoxHalfSize(ColSize / 2.f);
    m_Collider->SetRelativePositionY(ColSize.y / 2.f);

    m_Animation = m_AnimMesh->SetAnimation<CAnimation>("PatricNpcAnimation");

    m_Animation->AddAnimation("Patric_Npc_Confused", "Patric_Npc_Confused", 1.f, 1.f, false);
    m_Animation->AddAnimation("Patric_Npc_Default", "Patric_Npc_Default", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Excited", "Patric_Npc_Excited", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Scowl_Start", "Patric_Npc_Scowl_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Scowl_Loop", "Patric_Npc_Scowl_Loop", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Talk", "Patric_Npc_Talk", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Thinking_Start", "Patric_Npc_Thinking_Start", 1.f, 1.f, true);
    m_Animation->AddAnimation("Patric_Npc_Thinking_Loop", "Patric_Npc_Thinking_Loop", 1.f, 1.f, true);

    m_Animation->SetCurrentEndFunction("Patric_Npc_Scowl_Start", this, &CPatric::ChangeAnim_Scowl_Loop);
    m_Animation->SetCurrentEndFunction("Patric_Npc_Thinking_Start", this, &CPatric::ChangeAnim_Thinking_Loop);

    m_Animation->SetCurrentAnimation("Patric_Npc_Default");

    return true;
}

void CPatric::Update(float DeltaTime)
{
    CNpc::Update(DeltaTime);
}

void CPatric::PostUpdate(float DeltaTime)
{
    CNpc::PostUpdate(DeltaTime);
}

CPatric* CPatric::Clone() const
{
    return new CPatric(*this);
}

void CPatric::Save(FILE* File)
{
    CNpc::Save(File);
}

void CPatric::Load(FILE* File)
{
    CNpc::Load(File);
}

void CPatric::ChangeAnimByName(const std::string& Name)
{
    if (!m_Animation->FindAnimation(Name))
        return;

    m_Animation->ChangeAnimation(Name);
}

void CPatric::StartDialog()
{
    if (!m_EnableDialog)
        return;

    CDialogUI* DialogUI = m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI");

    if (!DialogUI)
        return;

    DialogUI->SetDialogInfo(m_NpcMapPos, m_NpcType);

    if (m_DialogCount == 0)
        DialogUI->SetCurDialog("First_Contact");
    else {
        // 추후 플레이어로부터 플라워 카운트를 받아 체크. 
        int FlowerCount = 500;
        // CPlayer* Player = m_Scene->FindObject<CPlayer>("Player");
        // int FlowerCount = Player->GetFlowerCount();

        if (FlowerCount >= 500)
            DialogUI->SetCurDialog("Enough_Socks");
        else
            DialogUI->SetCurDialog("Not_Enough_Socks");
    }

    DialogUI->OpenDialog();

    m_DialogCount++;
}

void CPatric::ChangeAnim_Confused()
{
    m_Animation->ChangeAnimation("Patric_Npc_Confused");
}

void CPatric::ChangeAnim_Default()
{
    m_Animation->ChangeAnimation("Patric_Npc_Default");
}

void CPatric::ChangeAnim_Excited()
{
    m_Animation->ChangeAnimation("Patric_Npc_Excited");
}

void CPatric::ChangeAnim_Scowl_Start()
{
    m_Animation->ChangeAnimation("Patric_Npc_Scowl_Start");
}

void CPatric::ChangeAnim_Scowl_Loop()
{
    m_Animation->ChangeAnimation("Patric_Npc_Scowl_Loop");
}

void CPatric::ChangeAnim_Talk()
{
    m_Animation->ChangeAnimation("Patric_Npc_Talk");
}

void CPatric::ChangeAnim_Thinking_Start()
{
    m_Animation->ChangeAnimation("Patric_Npc_Thinking_Start");
}

void CPatric::ChangeAnim_Thinking_Loop()
{
    m_Animation->ChangeAnimation("Patric_Npc_Thinking_Loop");
}

void CPatric::CreateSock()
{
    CSock* Sock = m_Scene->CreateObject<CSock>("Sock_Patrick");

    Sock->SetWorldPosition(GetWorldPos());
    Sock->SetWorldPositionZ(GetWorldPos().z - m_AnimMesh->GetMeshSize().z);
}
