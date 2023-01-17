#include "NavigationThread.h"
#include "DataStream.h"
#include "../Component/NavigationAgent.h"
#include "../Component/SceneComponent.h"
#include "Navigation.h"

CNavigationThread::CNavigationThread()	:
	m_TileMap(nullptr),
	m_Navigation(nullptr)
{
}

CNavigationThread::~CNavigationThread()
{
	SAFE_DELETE(m_Navigation);
}

void CNavigationThread::SetTileMapComponent(CTileMapComponent* TileMap)
{
	m_TileMap = TileMap;

	SAFE_DELETE(m_Navigation);

	m_Navigation = new CNavigation;

	m_Navigation->CreateNavigation(TileMap);
}

void CNavigationThread::AddInputData(CNavigationAgent* Agent, const Vector2& End)
{
	unsigned char	Buffer[1024] = {};

	CDataStream	stream;
	stream.SetBuffer(Buffer);

	unsigned __int64	Address = (unsigned __int64)Agent;

	stream.AddData<unsigned __int64>(&Address, 8);

	Vector3	OwnerPos = Agent->GetUpdateComponent()->GetWorldPos();

	Vector2	Start = Vector2(OwnerPos.x, OwnerPos.y);

	stream.AddData<Vector2>(&Start, 8);
	stream.AddData<Vector2>(&End, 8);

	m_InputQueue.push(0, stream.GetSize(), Buffer);

	if (IsSuspend())
		ReStart();
}

void CNavigationThread::Run()
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
				Suspend();

			return;
		}

		Vector2	Start, End;

		stream.GetData<Vector2>(&Start, 8);
		stream.GetData<Vector2>(&End, 8);


		// 길을 찾아준다.
		std::list<Vector2>	PathList;
		m_Navigation->FindPath(Start, End, PathList);


		// 길찾기가 끝나면 결과를 반환해준다.
		Agent->AddPathList(PathList);


		//if (m_InputQueue.empty() && m_Loop)
		//	Suspend();
	}
}
