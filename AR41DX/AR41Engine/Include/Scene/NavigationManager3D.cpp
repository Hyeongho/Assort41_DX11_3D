#include "NavigationManager3D.h"
#include "../Thread/NavigationThread.h"
#include "../Thread/Navigation3DThread.h"
#include "../Thread/Navigation3D.h"
#include "../Thread/ThreadManager.h"
#include "NavigationMesh.h"
#include "Scene.h"
#include "CameraManager.h"
#include "../Component/CameraComponent.h"
#include "../Input.h"

CNavigationManager3D::CNavigationManager3D() : m_Terrain(nullptr), m_NavigationMesh(nullptr)
{
}

CNavigationManager3D::~CNavigationManager3D()
{
    SAFE_DELETE(m_NavigationMesh);
}

void CNavigationManager3D::AddNavigationThread(CNavigationThread* Thread)
{
    CNavigationManager::AddNavigationThread(Thread);

    /*if (!m_Terrain)
    {
        m_Terrain = ((CNavigation3DThread*)Thread)->GetTerrain();

        CNavigation3D* Nav = ((CNavigation3DThread*)Thread)->GetNavigation();

        if (Nav)
        {
            m_Navigation = Nav->Clone();
        }
    }*/
}

void CNavigationManager3D::Start()
{
}

bool CNavigationManager3D::Init()
{
    return true;
}

bool CNavigationManager3D::CreateNavigationMesh(CSceneComponent* Component)
{
    m_Terrain = (CTerrainComponent*)Component;

    m_NavigationMesh = new CNavigationMesh;

    m_NavigationMesh->m_Mgr = this;

    m_NavigationMesh->CreateNavigation(m_Terrain);

    CThreadManager::GetInst()->CreateNavigationThread(m_NavigationMesh);

    return true;
}

void CNavigationManager3D::Move(CNavigationAgent3D* Agent, const Vector3& End)
{
    if (m_vecNavThread.empty())
    {
        return;
    }

    // 가지고 있는 스레드에서 가장 일이 적게 들어있는 스레드를 판단한다.
    int SelectThread = 0;
    int WorkCount = m_vecNavThread[0]->GetWorkCount();

    if (WorkCount > 0)
    {
        size_t  Size = m_vecNavThread.size();

        for (size_t i = 1; i < Size; i++)
        {
            if (WorkCount > m_vecNavThread[i]->GetWorkCount())
            {
                WorkCount = m_vecNavThread[i]->GetWorkCount();
                SelectThread = (int)i;

                if (WorkCount == 0)
                {
                    break;
                }
            }
        }
    }

    m_vecNavThread[SelectThread]->AddInputData(Agent, End);
}

float CNavigationManager3D::GetHeight(const Vector3& Pos)
{
    return m_NavigationMesh->GetHeight(Pos);
}

bool CNavigationManager3D::GetPickingPos(Vector3& Result) const
{
    if (!m_NavigationMesh)
    {
        return false;
    }

    // 뷰행렬을 얻어온다.
    Matrix  matView = m_Owner->GetCameraManager()->GetCurrentCamera()->GetViewMatrix();

    matView.Inverse();

    Ray ray = CInput::GetInst()->GetRay();
    ray.Pos = ray.Pos.TransformCoord(matView);
    ray.Dir = ray.Dir.TransformNormal(matView);

    return m_NavigationMesh->GetPickingPos(Result, ray);
}
