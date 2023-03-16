#include "Npc.h"

#include "Scene/Scene.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderOBB3D.h"
#include "../../UI/InteractUI.h"
#include "../../UI/DialogUI.h"

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
    m_Collider->SetCollisionCallback<CNpc>(ECollision_Result::Collision, this, &CNpc::Collision_Player);
    m_Collider->SetCollisionCallback<CNpc>(ECollision_Result::Release, this, &CNpc::Release_Player);
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

void CNpc::Collision_Player(const CollisionResult& result)
{
    const std::string& DestName = result.Dest->GetCollisionProfile()->Name;

    if (strcmp("Player", DestName.c_str()) == 0) {
        CInteractUI* InteractUI = m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");

        if (!InteractUI)
            return;

        InteractUI->SetTarget(EInteractTarget::Npc);
        InteractUI->ActiveInteractUI();
        m_EnableDialog = true;
    }

}

void CNpc::Release_Player(const CollisionResult& result)
{
    const std::string& DestName = result.Dest->GetCollisionProfile()->Name;

    if (strcmp("Player", DestName.c_str()) == 0) {
        CInteractUI* InteractUI = m_Scene->GetViewport()->FindUIWindow<CInteractUI>("InteractUI");

        if (!InteractUI)
            return;

        InteractUI->InActiveInteractUI();
        m_EnableDialog = false;
    }
}
