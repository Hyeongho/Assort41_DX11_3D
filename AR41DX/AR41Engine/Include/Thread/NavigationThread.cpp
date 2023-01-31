#include "NavigationThread.h"
#include "DataStream.h"
#include "../Component/NavigationAgent.h"
#include "../Component/SceneComponent.h"
#include "Navigation.h"

CNavigationThread::CNavigationThread()
{
}

CNavigationThread::~CNavigationThread()
{
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
	{
		ReStart();
	}
}

void CNavigationThread::AddInputData(CNavigationAgent* Agent, const Vector3& End)
{
	unsigned char	Buffer[1024] = {};

	CDataStream	stream;
	stream.SetBuffer(Buffer);

	unsigned __int64	Address = (unsigned __int64)Agent;

	stream.AddData<unsigned __int64>(&Address, 8);

	Vector3	OwnerPos = Agent->GetUpdateComponent()->GetWorldPos();

	stream.AddData<Vector3>(&OwnerPos, 12);
	stream.AddData<Vector3>(&End, 12);

	m_InputQueue.push(0, stream.GetSize(), Buffer);

	if (IsSuspend())
	{
		ReStart();
	}
}
