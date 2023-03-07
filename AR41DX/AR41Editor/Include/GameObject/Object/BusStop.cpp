#include "BusStop.h"

#include "Scene/Scene.h"
#include "Component/StaticMeshComponent.h"
#include "../../UI/InteractUI.h"
#include "../../UI/DialogUI.h"

CBusStop::CBusStop() :
    m_BusExist(false)
{
}

CBusStop::CBusStop(const CBusStop& Obj) :
    CGameObject(Obj)
{
    m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");

    m_BusExist = Obj.m_BusExist;
}

CBusStop::~CBusStop()
{
}

void CBusStop::Start()
{
    CGameObject::Start();
}

bool CBusStop::Init()
{
    CGameObject::Init();

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

    SetRootComponent(m_Mesh);

    m_Mesh->SetMesh("Taxi_Stop");

    return true;
}

void CBusStop::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    //float DetectDistance = 500.f;

    //Vector3 PlayerPos = m_Scene->FindObject("Player")->GetWorldPos();

    //float Dist = m_Mesh->GetWorldPos().Distance(PlayerPos);

    //if (DetectDistance >= Dist) {
    //    CInteractUI* InteractUI = m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");

    //    if (!InteractUI)
    //        return;

    //    // �÷��̾ ������(�浹 Ȥ�� Ư�� ���� ���� ���� ��) F�� ������ ��ȭ�� �߻��ϰԲ�.
    //    if (DetectDistance / 2.f >= Dist) {
    //        InteractUI->SetTarget(EInteractTarget::Bus_Stop);
    //        InteractUI->ActiveInteractUI();
    //    }
    //    else {
    //        InteractUI->InActiveInteractUI();
    //    }
    //}
}

void CBusStop::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CBusStop* CBusStop::Clone() const
{
    return new CBusStop(*this);
}

void CBusStop::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CBusStop::Load(FILE* File)
{
    CGameObject::Load(File);
}
