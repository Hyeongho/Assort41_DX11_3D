#include "Navigation3DThread.h"
#include "DataStream.h"
#include "../Component/NavigationAgent.h"
#include "../Component/SceneComponent.h"
#include "Navigation3D.h"

CNavigation3DThread::CNavigation3DThread() : m_Terrain(nullptr), m_Navigation(nullptr)
{
}

CNavigation3DThread::~CNavigation3DThread()
{
	SAFE_DELETE(m_Navigation);
}

void CNavigation3DThread::SetNavigationComponent(CSceneComponent* NavComponent)
{
	m_Terrain = (CTerrainComponent*)NavComponent;

	SAFE_DELETE(m_Navigation);

	m_Navigation = new CNavigation3D;

	m_Navigation->CreateNavigation((CTerrainComponent*)m_Terrain);
}

void CNavigation3DThread::Run()
{
	if (!m_InputQueue.empty())
	{
		int	Header = 0, Size = 0;
		unsigned char	Buffer[1024] = {};

		m_InputQueue.pop(&Header, &Size, Buffer);

		CDataStream	stream;
		stream.SetBuffer(Buffer);

		unsigned __int64	Address = 0;

		stream.GetData<unsigned __int64>(&Address, 8);

		CNavigationAgent* Agent = (CNavigationAgent*)Address;

		if (!Agent)
		{
			if (m_InputQueue.empty())
			{
				Suspend();
			}

			return;
		}

		Vector3	Start, End;

		stream.GetData<Vector3>(&Start, 12);
		stream.GetData<Vector3>(&End, 12);


		// 길을 찾아준다.
		//std::list<Vector3>	PathList;
		//m_Navigation->FindPath(Start, End, PathList);


		// 길찾기가 끝나면 결과를 반환해준다.
		//Agent->AddPathList(PathList);


		//if (m_InputQueue.empty() && m_Loop)
		//	Suspend();
	}
}
