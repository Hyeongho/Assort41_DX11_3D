#include "Patric.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"
#include "../Player.h"

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

    CreateAnim();
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

    CreateAnim();

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

void CPatric::StartDialog()
{
    if (!m_EnableDialog)
        return;

    CDialogUI* DialogUI = m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI");

    if (!DialogUI)
        return;

    DialogUI->SetDialogInfo(m_NpcMapPos, m_NpcType);

    if (m_DialogCount == 0) {
        DialogUI->SetCurDialog("First_Contact");

        CreateSock();
    }
    else {
        // 플레이어의 양말 개수를 체크해 기준 이하면 부족하다는 회화.
        // 충분하면 양말을 받고, 뒤집개를 준다.
        int SockStand = 3; // 임의로 3개를 기준으로 한다.

        CPlayer* Player = (CPlayer*)m_Scene->GetPlayerObject();

        if (Player) {
            if (Player->SubSock(SockStand)) {
                DialogUI->SetCurDialog("Enough_Socks");

                CreateSpatula();
            }
            else {
                DialogUI->SetCurDialog("Not_Enough_Socks");
            }
        }
    }

    DialogUI->OpenDialog();

    m_DialogCount++;
}

void CPatric::CreateAnim()
{
    if (m_Animation)
        return;


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