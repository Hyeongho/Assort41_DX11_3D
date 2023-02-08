#include "NavigationAgent3D.h"
#include "SceneComponent.h"
#include "../Thread/DataStream.h"
#include "../GameObject/GameObject.h"
#include "../Scene/Scene.h"

CNavigationAgent3D::CNavigationAgent3D()
{
	m_ComponentType = ComponentType::Object;

	SetTypeID<CNavigationAgent3D>();

	m_ComponentTypeName = "NavigationAgent3D";

	m_MoveSpeed = 400.f;
}

CNavigationAgent3D::CNavigationAgent3D(const CNavigationAgent3D& Obj) : CObjectComponent(Obj)
{
	m_MoveSpeed = Obj.m_MoveSpeed;
}

CNavigationAgent3D::~CNavigationAgent3D()
{
}

void CNavigationAgent3D::SetUpdateComponent(CSceneComponent* Component)
{
	m_UpdateComponent = Component;
}

void CNavigationAgent3D::SetMoveSpeed(float Speed)
{
	m_MoveSpeed = Speed;
}

void CNavigationAgent3D::AddPathList(const std::list<Vector3>& PathList)
{
	CDataStream stream;
	unsigned char Buffer[1024] = {};

	stream.SetBuffer(Buffer);

	int	Count = (int)PathList.size();

	stream.AddData<int>(&Count, sizeof(int));

	auto iter = PathList.begin();
	auto iterEnd = PathList.end();

	for (; iter != iterEnd; iter++)
	{
		stream.AddData<Vector3>(&(*iter), sizeof(Vector3));
	}

	m_FindQueue.push(0, stream.GetSize(), Buffer);
}

bool CNavigationAgent3D::Move(const Vector3& TargetPos)
{
	if (!m_UpdateComponent)
	{
		return false;
	}

	m_Scene->GetNavigationManager()->Move(this, TargetPos);

	return true;
}

void CNavigationAgent3D::Destroy()
{
	CObjectComponent::Destroy();
}

void CNavigationAgent3D::Start()
{
	CObjectComponent::Start();

	if (!m_UpdateComponent)
		m_UpdateComponent = m_Owner->GetRootComponent();
}

bool CNavigationAgent3D::Init()
{
	if (!CObjectComponent::Init())
	{
		return false;
	}

	return true;
}

void CNavigationAgent3D::Update(float DeltaTime)
{
	CObjectComponent::Update(DeltaTime);

	if (!m_FindQueue.empty())
	{
		m_PathList.clear();

		int	Header = 0, Size = 0;
		unsigned char Buffer[1024] = {};

		m_FindQueue.pop(&Header, &Size, Buffer);

		CDataStream stream;

		stream.SetBuffer(Buffer);

		int Count = 0;

		stream.GetData<int>(&Count, sizeof(int));

		for (int i = 0; i < Count; i++)
		{
			Vector3	Pos;
			stream.GetData<Vector3>(&Pos, sizeof(Vector3));

			m_PathList.push_back(Pos);
		}
	}

	if (!m_PathList.empty() && m_UpdateComponent)
	{
		Vector3	TargetPos = m_PathList.front();
		Vector3	SrcPos = m_UpdateComponent->GetWorldPos();

		float Dist = SrcPos.Distance(TargetPos);

		Vector3	Dir = TargetPos - SrcPos;
		Dir.Normalize();

		float Speed = m_MoveSpeed * DeltaTime;

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

void CNavigationAgent3D::PostUpdate(float DeltaTime)
{
	CObjectComponent::PostUpdate(DeltaTime);
}

void CNavigationAgent3D::Render()
{
	CObjectComponent::Render();
}

CNavigationAgent3D* CNavigationAgent3D::Clone() const
{
	return new CNavigationAgent3D(*this);
}

void CNavigationAgent3D::Save(FILE* File)
{
	CObjectComponent::Save(File);
}

void CNavigationAgent3D::Load(FILE* File)
{
	CObjectComponent::Load(File);
}
