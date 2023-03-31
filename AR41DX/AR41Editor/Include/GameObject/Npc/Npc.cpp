#include "Npc.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "../../UI/InteractUI.h"
#include "../../UI/DialogUI.h"
#include "../Object/Common/Collectible/Sock.h"
#include "../Object/Common/Collectible/GoldenSpatula.h"

CNpc::CNpc()
{
    SetTypeID<CNpc>();

    m_ObjectTypeName = "Npc";

    m_DialogCount = 0;
    m_NpcType = ENpcList::End;
    m_NpcMapPos = EMapList::End;
    m_EnableDialog = false;
    m_NpcMeshType = MeshType::Animation;
}

CNpc::CNpc(const CNpc& Obj)
    : CGameObject(Obj)
{
    m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");


    m_DialogCount = Obj.m_DialogCount;
    m_NpcType = Obj.m_NpcType;
    m_NpcMapPos = Obj.m_NpcMapPos;
    m_EnableDialog = Obj.m_EnableDialog;
    m_NpcMeshType = Obj.m_NpcMeshType;
}

CNpc::~CNpc()
{
}

void CNpc::Start()
{
	CGameObject::Start();

    m_Collider->SetCollisionCallback<CNpc>(ECollision_Result::Collision, this, &CNpc::Collision_Player);
    m_Collider->SetCollisionCallback<CNpc>(ECollision_Result::Release, this, &CNpc::Release_Player);
}

bool CNpc::Init()
{
	CGameObject::Init();

    m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

    if (m_NpcMeshType == MeshType::Animation) {
        m_AnimMesh = CreateComponent<CAnimationMeshComponent>("Mesh");

        SetRootComponent(m_AnimMesh);

        m_AnimMesh->AddChild(m_Collider);
    }
    else if (m_NpcMeshType == MeshType::Static) {
        m_StaticMesh = CreateComponent<CStaticMeshComponent>("Mesh");

        SetRootComponent(m_StaticMesh);

        m_StaticMesh->AddChild(m_Collider);
    }


    m_Collider->SetCollisionProfile("Npc");
    m_Collider->SetInheritRotX(true);
    m_Collider->SetInheritRotY(true);
    m_Collider->SetInheritRotZ(true);

	return true;
}

void CNpc::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
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

    fwrite(&m_NpcMapPos, sizeof(EMapList), 1, File);
    fwrite(&m_NpcType, sizeof(ENpcList), 1, File);
    fwrite(&m_NpcMeshType, sizeof(MeshType), 1, File);
}

void CNpc::Load(FILE* File)
{
	CGameObject::Load(File);

    fread(&m_NpcMapPos, sizeof(EMapList), 1, File);
    fread(&m_NpcType, sizeof(ENpcList), 1, File);
    fread(&m_NpcMeshType, sizeof(MeshType), 1, File);
}

void CNpc::ChangeAnimByName(const std::string& Name)
{
    if (!m_Animation || !m_Animation->FindAnimation(Name))
        return; 

    m_Animation->ChangeAnimation(Name);
}

void CNpc::StartDialog()
{
}

void CNpc::Collision_Player(const CollisionResult& result)
{
    const std::string& DestName = result.Dest->GetCollisionProfile()->Name;

    if (strcmp("Player", DestName.c_str()) == 0) {
        CInteractUI* InteractUI = m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");

        if (!InteractUI)
            return;

        InteractUI->SetTarget(EInteractTarget::Npc);

        if (m_NpcType == ENpcList::BusDriver) {
            InteractUI->SetTarget(EInteractTarget::Bus);
        }

        if (m_NpcType == ENpcList::TaxiStop) {
            InteractUI->SetTarget(EInteractTarget::Taxi);
        }

        if (m_NpcType == ENpcList::InfoSign && m_NpcMapPos == EMapList::Bikini_Bottom) {
            InteractUI->SetTarget(EInteractTarget::InfoSign);
        }

        if (m_NpcType == ENpcList::InfoSign && m_NpcMapPos == EMapList::Chum_Bucketlab) {
            InteractUI->SetTarget(EInteractTarget::InfoSign2);
        }
        

        InteractUI->ActiveInteractUI();
        m_EnableDialog = true;
    }

}

void CNpc::Release_Player(const CollisionResult& result)
{
    if (result.Src->GetName() == "InfoSignCollider")
        return;

    const std::string& DestName = result.Dest->GetCollisionProfile()->Name;

    if (strcmp("Player", DestName.c_str()) == 0) {
        //CInteractUI* InteractUI = m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");
        CInteractUI* InteractUI = CSceneManager::GetInst()->GetScene()->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");

        if (!InteractUI)
            return;

        InteractUI->InActiveInteractUI();
        m_EnableDialog = false;
    }
}

void CNpc::CreateSock()
{
    std::string ObjName = "Sock_" + m_ObjectTypeName;

    CSock* Sock = m_Scene->CreateObject<CSock>(ObjName);

    Sock->SetWorldPosition(GetWorldPos());
    Sock->SetWorldPositionZ(GetWorldPos().z - m_AnimMesh->GetMeshSize().z);
}

void CNpc::CreateSpatula()
{
    std::string ObjName = "GoldenSpatula_" + m_ObjectTypeName;

    CGoldenSpatula* GoldenSpatula = m_Scene->CreateObject<CGoldenSpatula>(ObjName);

    GoldenSpatula->SetWorldPosition(GetWorldPos());
    GoldenSpatula->SetWorldPositionZ(GetWorldPos().z - m_AnimMesh->GetMeshSize().z);
}
