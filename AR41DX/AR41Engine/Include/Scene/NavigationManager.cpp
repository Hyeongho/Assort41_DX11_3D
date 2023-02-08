#include "NavigationManager.h"
#include "../Thread/NavigationThread.h"

CNavigationManager::CNavigationManager()
{
}

CNavigationManager::~CNavigationManager()
{
}

void CNavigationManager::Start()
{
}

bool CNavigationManager::Init()
{
    return false;
}

bool CNavigationManager::CreateNavigationMesh(CSceneComponent* Component)
{
    return false;
}

void CNavigationManager::Move(CNavigationAgent* Agent, const Vector2& End)
{
}

void CNavigationManager::Move(CNavigationAgent3D* Agent, const Vector3& End)
{
}

float CNavigationManager::GetHeight(const Vector3& Pos)
{
    return 0.0f;
}

bool CNavigationManager::GetPickingPos(Vector3& Result) const
{
    return false;
}
