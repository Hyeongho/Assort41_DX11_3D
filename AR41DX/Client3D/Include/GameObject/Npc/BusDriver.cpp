#include "BusDriver.h"

#include "Component/AnimationMeshComponent.h"
#include "Scene/Scene.h"
#include "../../UI/DialogUI.h"

CBusDriver::CBusDriver()
{
    SetTypeID<CBusDriver>();

    m_ObjectTypeName = "BusDriver";

    m_DialogCount = 0;
    //m_NpcType = ENpcList::BusDriver;
    m_NpcMapPos = EMapList::Bikini_Bottom;
    m_EnableDialog = false;
}

CBusDriver::CBusDriver(const CBusDriver& Obj)
	: CNpc(Obj)
{
    m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
    m_Animation = (CAnimation*)FindComponent("BusDriverAnimation");

    m_DialogCount = Obj.m_DialogCount;
    m_NpcType = Obj.m_NpcType;
    m_NpcMapPos = Obj.m_NpcMapPos;
    m_EnableDialog = Obj.m_EnableDialog;
}

CBusDriver::~CBusDriver()
{
}

void CBusDriver::Start()
{
	CNpc::Start();
}

bool CBusDriver::Init()
{
	CNpc::Init();

    m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Bus_Driver");

    m_Animation = m_Mesh->SetAnimation<CAnimation>("BusDriverAnimation");

    m_Animation->AddAnimation("Bus_Driver_Drive", "Bus_Driver_Drive", 1.f, 1.f, true);
    m_Animation->AddAnimation("Bus_Driver_Stop", "Bus_Driver_Stop", 1.f, 1.f, true);

    m_Animation->SetCurrentAnimation("Bus_Driver_Drive");

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

void CBusDriver::ChangeAnim_Stop()
{
	m_Animation->ChangeAnimation("Bus_Driver_Stop");
}

void CBusDriver::ChangeAnim_Drive()
{
	m_Animation->ChangeAnimation("Bus_Driver_Drive");
}
