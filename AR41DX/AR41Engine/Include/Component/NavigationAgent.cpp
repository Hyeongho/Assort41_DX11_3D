#include "NavigationAgent.h"
#include "SceneComponent.h"
#include "../Thread/DataStream.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"

CNavigationAgent::CNavigationAgent()
{
	m_ComponentType = ComponentType::Object;

	SetTypeID<CNavigationAgent>();

	m_ComponentTypeName = "NavigationAgent";

	m_MoveSpeed = 400.f;
}

CNavigationAgent::CNavigationAgent(const CNavigationAgent& Obj) :
    CObjectComponent(Obj)
{
	m_MoveSpeed = Obj.m_MoveSpeed;
}

CNavigationAgent::~CNavigationAgent()
{
}

void CNavigationAgent::SetUpdateComponent(CSceneComponent* Component)
{
	m_UpdateComponent = Component;
}

void CNavigationAgent::SetMoveSpeed(float Speed)
{
	m_MoveSpeed = Speed;
}

void CNavigationAgent::AddPathList(const std::list<Vector2>& PathList)
{
	CDataStream		stream;
	unsigned char	Buffer[1024] = {};

	stream.SetBuffer(Buffer);

	int	Count = (int)PathList.size();

	stream.AddData<int>(&Count, sizeof(int));

	auto	iter = PathList.begin();
	auto	iterEnd = PathList.end();

	for (; iter != iterEnd; iter++)
	{
		stream.AddData<Vector2>(&(*iter), sizeof(Vector2));
	}

	m_FindQueue.push(0, stream.GetSize(), Buffer);
}

bool CNavigationAgent::Move(const Vector2& TargetPos)
{
	if (!m_UpdateComponent)
		return false;

	m_Scene->GetNavigationManager()->Move(this, TargetPos);

	return true;
}

void CNavigationAgent::Destroy()
{
	CObjectComponent::Start();
}

void CNavigationAgent::Start()
{
	CObjectComponent::Start();

	if (!m_UpdateComponent)
		m_UpdateComponent = m_Owner->GetRootComponent();
}

bool CNavigationAgent::Init()
{
	if (!CObjectComponent::Init())
		return false;

    return true;
}

void CNavigationAgent::Update(float DeltaTime)
{
	CObjectComponent::Update(DeltaTime);

	if (!m_FindQueue.empty())
	{
		m_PathList.clear();

		int	Header = 0, Size = 0;
		unsigned char	Buffer[1024] = {};

		m_FindQueue.pop(&Header, &Size, Buffer);

		CDataStream		stream;

		stream.SetBuffer(Buffer);

		int		Count = 0;

		stream.GetData<int>(&Count, sizeof(int));

		for (int i = 0; i < Count; ++i)
		{
			Vector2	Pos;
			stream.GetData<Vector2>(&Pos, sizeof(Vector2));

			m_PathList.push_back(Pos);
		}
	}

	if (!m_PathList.empty() && m_UpdateComponent)
	{
		Vector2	TargetPos = m_PathList.front();
		Vector2	SrcPos = m_UpdateComponent->GetWorldPos();

		float	Dist = SrcPos.Distance(TargetPos);

		Vector2	Dir = TargetPos - SrcPos;
		Dir.Normalize();

		float	Speed = m_MoveSpeed * DeltaTime;

		SrcPos += Dir * Speed;

		// 도착했을 경우
		if (Speed >= Dist)
		{
			SrcPos = TargetPos;

			m_PathList.pop_front();
		}

		m_UpdateComponent->SetWorldPosition(SrcPos);

	}
}

void CNavigationAgent::PostUpdate(float DeltaTime)
{
	CObjectComponent::PostUpdate(DeltaTime);
}

void CNavigationAgent::Render()
{
	CObjectComponent::Render();
}

CNavigationAgent* CNavigationAgent::Clone() const
{
    return new CNavigationAgent(*this);
}

void CNavigationAgent::Save(FILE* File)
{
	CObjectComponent::Save(File);
}

void CNavigationAgent::Load(FILE* File)
{
	CObjectComponent::Load(File);
}
