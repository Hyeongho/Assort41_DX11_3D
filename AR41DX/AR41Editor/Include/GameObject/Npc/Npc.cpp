#include "Npc.h"

#include "Scene/Scene.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "../../UI/InteractUI.h"
#include "../../UI/DialogUI.h"

CNpc::CNpc()
{
}

CNpc::CNpc(const CNpc& Obj)
    : CGameObject(Obj)
{
}

CNpc::~CNpc()
{
}

void CNpc::Start()
{
	CGameObject::Start();
}

bool CNpc::Init()
{
	CGameObject::Init();

	return true;
}

void CNpc::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

    float DetectDistance = 500.f;

    Vector3 PlayerPos = m_Scene->GetPlayerObject()->GetWorldPos();

    float Dist = 0.f;

    if (m_NpcMeshType == MeshType::Animation)
        Dist = m_AnimMesh->GetWorldPos().Distance(PlayerPos);
    else if (m_NpcMeshType == MeshType::Static)
        Dist = m_StaticMesh->GetWorldPos().Distance(PlayerPos);
    else
        return;

    if (DetectDistance >= Dist) {
        CInteractUI* InteractUI = m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");

        if (!InteractUI)
            return;

        // �÷��̾ ������(�浹 Ȥ�� Ư�� ���� ���� ���� ��) F�� ������ ��ȭ�� �߻��ϰԲ�.
        if (DetectDistance / 2.f >= Dist) {
            InteractUI->SetTarget(EInteractTarget::Npc);
            InteractUI->ActiveInteractUI();
            m_EnableDialog = true;
        }
        else {
            InteractUI->InActiveInteractUI();
            m_EnableDialog = false;
        }
    }
}

void CNpc::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNpc* CNpc::Clone() const
{
	return new CNpc(*this);
}

void CNpc::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CNpc::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CNpc::ChangeAnimByName(const std::string& Name)
{
}

void CNpc::StartDialog()
{
}
