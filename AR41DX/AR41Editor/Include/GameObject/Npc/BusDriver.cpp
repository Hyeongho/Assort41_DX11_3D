#include "BusDriver.h"

#include "Input.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"

CBusDriver::CBusDriver()
{
    SetTypeID<CBusDriver>();

    m_ObjectTypeName = "BusDriver";

    m_DialogCount = 0;
    m_NpcType = ENpcList::BusDriver;
    m_NpcMapPos = EMapList::Bikini_Bottom;
    m_EnableDialog = false;
    m_NpcMeshType = MeshType::Animation;
}

CBusDriver::CBusDriver(const CBusDriver& Obj)
    : CNpc(Obj)
{
    m_AnimMesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("BusDriverAnimation");

    m_DialogCount = Obj.m_DialogCount;
    m_NpcType = Obj.m_NpcType;
    m_NpcMapPos = Obj.m_NpcMapPos;
    m_EnableDialog = Obj.m_EnableDialog;
    m_NpcMeshType = Obj.m_NpcMeshType;
}

CBusDriver::~CBusDriver()
{
}

void CBusDriver::Start()
{
    CNpc::Start();

#ifdef _DEBUG
    CInput::GetInst()->AddBindFunction<CBusDriver>("F1", Input_Type::Up, this, &CBusDriver::DebugKeyF1, m_Scene);
    CInput::GetInst()->AddBindFunction<CBusDriver>("F2", Input_Type::Up, this, &CBusDriver::DebugKeyF2, m_Scene);
#endif // DEBUG

    CInput::GetInst()->AddBindFunction<CBusDriver>("F", Input_Type::Up, this, &CBusDriver::StartDialog, m_Scene);
}

bool CBusDriver::Init()
{
    CNpc::Init();

    m_AnimMesh->SetMesh("Bus_Driver");

    Vector3 colSize(1.f, 1.f, 1.f);

    if (m_AnimMesh->GetMeshSize().x >= m_AnimMesh->GetMeshSize().z)
        colSize *= m_AnimMesh->GetMeshSize().x;
    else
        colSize *= m_AnimMesh->GetMeshSize().z;

    m_Collider->SetBoxHalfSize(colSize / 2.f);
    m_Collider->SetRelativePositionY(colSize.y / 2.f);

    m_Animation = m_AnimMesh->SetAnimation<CAnimation>("BusDriverAnimation");

    m_Animation->AddAnimation("Bus_Driver_Drive", "Bus_Driver_Drive", 1.f, 1.f, true);
    m_Animation->AddAnimation("Bus_Driver_Stop", "Bus_Driver_Stop", 1.f, 1.f, false);

    m_Animation->SetCurrentAnimation("Bus_Driver_Stop");


    return true;
}

void CBusDriver::Update(float DeltaTime)
{
    CNpc::Update(DeltaTime);
}

void CBusDriver::PostUpdate(float DeltaTime)
{
    CNpc::PostUpdate(DeltaTime);
}

CBusDriver* CBusDriver::Clone() const
{
    return new CBusDriver(*this);
}

void CBusDriver::Save(FILE* File)
{
    CNpc::Save(File);
}

void CBusDriver::Load(FILE* File)
{
    CNpc::Load(File);
}

void CBusDriver::StartDialog()
{
    if (!m_EnableDialog)
        return;

    CDialogUI* DialogUI = m_Scene->GetViewport()->FindUIWindow<CDialogUI>("DialogUI");

    if (!DialogUI)
        return;

    DialogUI->SetDialogInfo(m_NpcMapPos, m_NpcType);
    DialogUI->SetCurDialog("First_Contact");

    DialogUI->OpenDialog();

    m_DialogCount++;
}

void CBusDriver::MoveToBusStop()
{
}

void CBusDriver::ChangeAnim_Stop()
{
    m_Animation->ChangeAnimation("Bus_Driver_Stop");
}

void CBusDriver::ChangeAnim_Drive()
{
    m_Animation->ChangeAnimation("Bus_Driver_Drive");
}

void CBusDriver::DebugKeyF1()
{
    ChangeAnim_Stop();
}

void CBusDriver::DebugKeyF2()
{
    ChangeAnim_Drive();
}
