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
    return true;
}

void CNavigationManager::Move(CNavigationAgent* Agent, const Vector2& End)
{
    if (m_vecNavThread.empty())
        return;

    // 가지고 있는 스레드에서 가장 일이 적게 들어있는 스레드를 판단한다.
    int SelectThread = 0;
    int WorkCount = m_vecNavThread[0]->GetWorkCount();

    if (WorkCount > 0)
    {
        size_t  Size = m_vecNavThread.size();

        for (size_t i = 1; i < Size; ++i)
        {
            if (WorkCount > m_vecNavThread[i]->GetWorkCount())
            {
                WorkCount = m_vecNavThread[i]->GetWorkCount();
                SelectThread = (int)i;

                if (WorkCount == 0)
                    break;
            }
        }
    }

    m_vecNavThread[SelectThread]->AddInputData(Agent, End);
}
