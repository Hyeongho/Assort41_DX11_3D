#include "UIWidget.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"
#include "../Component/CameraComponent.h"
#include "../Scene/CameraManager.h"
#include "UIWindow.h"

std::unordered_map<std::string, CUIWidget*> CUIWidget::m_mapUIWidgetCDO;

CUIConstantBuffer* CUIWidget::m_CBuffer = nullptr;
CAnimation2DConstantBuffer* CUIWidget::m_AnimCBuffer = nullptr;

CUIWidget::CUIWidget()  :
    m_ZOrder(0),
    m_Angle(0.f),
    m_Opacity(1.f),
    m_Start(false),
    m_Tint(1.f, 1.f, 1.f, 1.f),
    m_MouseHovered(false)
{
    m_WidgetTypeName = "UIWidget";
}

CUIWidget::CUIWidget(const CUIWidget& Widget)   :
    CRef(Widget)
{
    m_ZOrder = Widget.m_ZOrder;
    m_Pos = Widget.m_Pos;
    m_RenderPos = Widget.m_RenderPos;
    m_Size = Widget.m_Size;
    m_Angle = Widget.m_Angle;
    m_Opacity = Widget.m_Opacity;
    m_Mesh = Widget.m_Mesh;
    m_Shader = Widget.m_Shader;
    m_Tint = Widget.m_Tint;
    m_MeshSize = Widget.m_MeshSize;
    m_MouseHovered = false;
}

CUIWidget::~CUIWidget()
{
}

void CUIWidget::SetShader(const std::string& Name)
{
    if (m_Scene)
    {
        m_Shader = m_Scene->GetResource()->FindShader(Name);
    }

    else
    {
        m_Shader = CResourceManager::GetInst()->FindShader(Name);
    }
}

void CUIWidget::SetMesh(const std::string& Name)
{
    if (m_Scene)
    {
        m_Mesh = m_Scene->GetResource()->FindMesh(Name);
    }

    else
    {
        m_Mesh = CResourceManager::GetInst()->FindMesh(Name);
    }

    Vector3 MeshSize = m_Mesh->GetMax() - m_Mesh->GetMin();

    m_MeshSize = Vector2(MeshSize.x, MeshSize.y);
}

void CUIWidget::Start()
{
}

bool CUIWidget::Init()
{
    if (m_Scene)
    {
        m_Mesh = m_Scene->GetResource()->FindMesh("LBUVRect");
        m_Shader = m_Scene->GetResource()->FindShader("UIShader");
    }

    else
    {
        m_Mesh = CResourceManager::GetInst()->FindMesh("LBUVRect");
        m_Shader = CResourceManager::GetInst()->FindShader("UIShader");
    }

    Vector3 MeshSize = m_Mesh->GetMax() - m_Mesh->GetMin();

    m_MeshSize = Vector2(MeshSize.x, MeshSize.y);

    return true;
}

void CUIWidget::Update(float DeltaTime)
{
}

void CUIWidget::PostUpdate(float DeltaTime)
{
    // 출력될 위치정보를 만들어준다. m_Pos는 위젯을 가지고 있는 윈도우로부터 상대적인 위치
    // 정보로 구성된다. 그러므로 최종 출력될 위치는 윈도우위치 + m_Pos 가 된다.
    m_RenderPos = m_Owner->GetPos() + m_Pos;
}

void CUIWidget::Render()
{
    // 상수버퍼를 채워준다.
    m_CBuffer->SetMeshSize(m_MeshSize);
    m_CBuffer->SetOpacity(m_Opacity);
    m_CBuffer->SetPivot(m_Pivot);
    m_CBuffer->SetTint(m_Tint);

    CCameraComponent* Camera = m_Scene->GetCameraManager()->GetUICamera();

    Matrix  matProj = Camera->GetProjMatrix();

    Matrix  matScale, matRot, matTranslate;

    matScale.Scaling(m_Size);
    matRot.RotationZ(m_Angle);
    matTranslate.Translation(m_RenderPos);

    Matrix  matWorld = matScale * matRot * matTranslate;

    Matrix  matWVP = matWorld * matProj;
    
    m_CBuffer->SetWVP(matWVP);

    m_CBuffer->UpdateBuffer();

    m_AnimCBuffer->UpdateBuffer();

    m_Shader->SetShader();

    m_Mesh->Render();
}

CUIWidget* CUIWidget::Clone()
{
    return nullptr;
}

void CUIWidget::Save(FILE* File)
{
    fwrite(&m_ZOrder, sizeof(int), 1, File);

    fwrite(&m_Pos, sizeof(Vector2), 1, File);
    fwrite(&m_RenderPos, sizeof(Vector2), 1, File);
    fwrite(&m_Size, sizeof(Vector2), 1, File);
    fwrite(&m_Pivot, sizeof(Vector2), 1, File);
    fwrite(&m_MeshSize, sizeof(Vector2), 1, File);

    fwrite(&m_Angle, sizeof(float), 1, File);
    fwrite(&m_Opacity, sizeof(float), 1, File);

    fwrite(&m_Tint, sizeof(Vector4), 1, File);

    int	Length = (int)m_Mesh->GetName().length();

    fwrite(&Length, 4, 1, File);
    fwrite(m_Mesh->GetName().c_str(), 1, Length, File);

    Length = (int)m_Shader->GetName().length();

    fwrite(&Length, 4, 1, File);
    fwrite(m_Shader->GetName().c_str(), 1, Length, File);
}

void CUIWidget::Load(FILE* File)
{
    fread(&m_ZOrder, sizeof(int), 1, File);

    fread(&m_Pos, sizeof(Vector2), 1, File);
    fread(&m_RenderPos, sizeof(Vector2), 1, File);
    fread(&m_Size, sizeof(Vector2), 1, File);
    fread(&m_Pivot, sizeof(Vector2), 1, File);
    fread(&m_MeshSize, sizeof(Vector2), 1, File);

    fread(&m_Angle, sizeof(float), 1, File);
    fread(&m_Opacity, sizeof(float), 1, File);

    fread(&m_Tint, sizeof(Vector4), 1, File);

    int	Length = 0;
    char    MeshName[256] = {};
    char    ShaderName[256] = {};

    fread(&Length, 4, 1, File);
    fread(MeshName, 1, Length, File);

    Length = 0;

    fread(&Length, 4, 1, File);
    fread(ShaderName, 1, Length, File);

    if (m_Scene)
    {
        m_Mesh = m_Scene->GetResource()->FindMesh(MeshName);
        m_Shader = m_Scene->GetResource()->FindShader(ShaderName);
    }

    else
    {
        m_Mesh = CResourceManager::GetInst()->FindMesh(MeshName);
        m_Shader = CResourceManager::GetInst()->FindShader(ShaderName);
    }
}

bool CUIWidget::CollisionMouse(const Vector2& MousePos)
{
    Vector2 Pos = m_Owner->GetPos() + m_Pos - m_Pivot * m_Size;

    if (Pos.x > MousePos.x)
        return false;

    else if (Pos.x + m_Size.x < MousePos.x)
        return false;

    else if (Pos.y > MousePos.y)
        return false;

    else if (Pos.y + m_Size.y < MousePos.y)
        return false;

    return true;
}
