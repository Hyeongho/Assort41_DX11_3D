#include "InteractButton.h"

#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"

CInteractButton::CInteractButton()
{
    SetTypeID<CInteractButton>();

    m_ObjectTypeName = "InteractButton";

    m_ButtonDir = ButtonDir::Virtical;
    m_IsPushed = false;
}

CInteractButton::CInteractButton(const CInteractButton& Obj) :
    CGameObject(Obj)
{
    m_MeshButton = (CStaticMeshComponent*)FindComponent("Mesh");
    m_MeshButtonPlate = (CStaticMeshComponent*)FindComponent("Mesh");
    m_Collider = (CColliderOBB3D*)FindComponent("OBB3D");

    m_ButtonDir = Obj.m_ButtonDir;
    m_IsPushed = Obj.m_IsPushed;
}

CInteractButton::~CInteractButton()
{
}

void CInteractButton::Start()
{
    CGameObject::Start();
}

bool CInteractButton::Init()
{
    CGameObject::Init();

    m_MeshButton = CreateComponent<CStaticMeshComponent>("Mesh");
    m_MeshButtonPlate = CreateComponent<CStaticMeshComponent>("Mesh");
    m_Collider = CreateComponent<CColliderOBB3D>("OBB3D");

    SetRootComponent(m_MeshButtonPlate);
    m_MeshButtonPlate->AddChild(m_Collider);
    m_MeshButtonPlate->AddChild(m_MeshButton);

    m_MeshButton->SetMesh("Button");
    m_MeshButtonPlate->SetMesh("ButtonPlate");

    m_ButtonDir = ButtonDir::Horizon;

    if (m_ButtonDir == ButtonDir::Virtical) {
        m_MeshButton->SetRelativePositionY(m_MeshButtonPlate->GetMeshSize().y);

        float ColSizeX = m_MeshButtonPlate->GetMeshSize().x / 2.f;
        float ColSizeY = m_MeshButtonPlate->GetMeshSize().y / 2.f + m_MeshButton->GetMeshSize().y / 2.f;
        float ColSizeZ = m_MeshButtonPlate->GetMeshSize().z / 2.f;

        m_Collider->SetBoxHalfSize(ColSizeX, ColSizeY, ColSizeZ);
        m_Collider->SetRelativePositionY(ColSizeY);
    }
    else if (m_ButtonDir == ButtonDir::Horizon) {
        m_MeshButton->SetRelativeRotationX(-90.f);
        m_MeshButton->SetRelativePositionZ(-m_MeshButtonPlate->GetMeshSize().z / 2.f);
        m_MeshButton->SetRelativePositionY(m_MeshButtonPlate->GetMeshSize().y / 2.f);

        float ColSizeX = m_MeshButtonPlate->GetMeshSize().x / 2.f;
        float ColSizeY = m_MeshButtonPlate->GetMeshSize().y / 2.f;
        float ColSizeZ = m_MeshButtonPlate->GetMeshSize().z / 2.f + m_MeshButton->GetMeshSize().y / 2.f;

        m_Collider->SetBoxHalfSize(ColSizeX, ColSizeY, ColSizeZ);
        m_Collider->SetRelativePositionY(ColSizeY);
        m_Collider->SetRelativePositionZ(-m_MeshButton->GetMeshSize().y / 2.f);
    }

    m_Collider->SetCollisionProfile("Button");
    m_Collider->SetCollisionCallback<CInteractButton>(ECollision_Result::Collision, this, &CInteractButton::Collision_Attacked);

    m_Collider->SetInheritRotX(true);
    m_Collider->SetInheritRotY(true);
    m_Collider->SetInheritRotZ(true);

    return true;
}

void CInteractButton::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);

    if (m_IsPushed) {
        float Move = g_DeltaTime * 30.f;

        if (m_ButtonDir == ButtonDir::Virtical) {
            float ButtonPoint = m_MeshButton->GetWorldPos().y + m_MeshButton->GetMeshSize().y;
            float VerticPoint = m_MeshButtonPlate->GetWorldPos().y + m_MeshButtonPlate->GetMeshSize().y + m_MeshButton->GetMeshSize().y / 2.f;

            if (ButtonPoint < VerticPoint) {
                m_IsPushed = false;
                return;
            }

            m_MeshButton->AddRelativePositionY(-Move);
        }
        else if (m_ButtonDir == ButtonDir::Horizon) {
            float ButtonPoint = m_MeshButton->GetWorldPos().z - m_MeshButton->GetMeshSize().y;
            float HorizPoint = m_MeshButtonPlate->GetWorldPos().z - m_MeshButton->GetMeshSize().y - m_MeshButton->GetMeshSize().y;

            if (ButtonPoint > HorizPoint) {
                m_IsPushed = false;
                return;
            }

            m_MeshButton->AddRelativePositionZ(Move);
        }
    }

}

void CInteractButton::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

CInteractButton* CInteractButton::Clone() const
{
    return new CInteractButton(*this);
}

void CInteractButton::Save(FILE* File)
{
    CGameObject::Save(File);
}

void CInteractButton::Load(FILE* File)
{
    CGameObject::Load(File);
}

void CInteractButton::Collision_Attacked(const CollisionResult& result)
{
    m_IsPushed = true;

    if(m_Func != nullptr)
        m_Func();
}

