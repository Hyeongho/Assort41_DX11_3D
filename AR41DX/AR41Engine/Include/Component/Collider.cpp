#include "Collider.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Material/Material.h"
#include "../Engine.h"
#include "../Scene/SceneCollision.h"
#include "../CollisionManager.h"

CCollider::CCollider()
{
	SetTypeID<CCollider>();

	m_ComponentTypeName = "Collider";
	m_Result.Src = this;
	m_MouseCollision = false;
}

CCollider::CCollider(const CCollider& component) :
	CSceneComponent(component)
{
	m_Mesh = component.m_Mesh;
	m_Color = component.m_Color;
	m_ColliderType = component.m_ColliderType;
	m_Profile = component.m_Profile;
	m_Result.Src = this;
	m_MouseCollision = false;
}

CCollider::~CCollider()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->DeletePrevCollisionCollider(this);

		(*iter)->m_Result.Src = *iter;
		(*iter)->m_Result.Dest = this;
		(*iter)->CallCollisionCallback(ECollision_Result::Release);

		m_Result.Src = this;
		m_Result.Dest = *iter;
		CallCollisionCallback(ECollision_Result::Release);
	}
}

void CCollider::DeletePrevCollisionCollider(CCollider* Collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == Collider)
		{
			m_PrevCollisionList.erase(iter);
			return;
		}
	}
}

bool CCollider::CheckPrevCollision(CCollider* Collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

void CCollider::CheckPrevCollisionColliderSection()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd;)
	{
		bool	Check = false;

		auto	iterIndex = m_CurrentSectionList.begin();
		auto	iterIndexEnd = m_CurrentSectionList.end();

		for (; iterIndex != iterIndexEnd; iterIndex++)
		{
			// 이전 충돌물체의 충돌영역 목록을 반복한다.
			auto	iterDestIndex = (*iter)->m_CurrentSectionList.begin();
			auto	iterDestIndexEnd = (*iter)->m_CurrentSectionList.end();

			for (; iterDestIndex != iterDestIndexEnd; iterDestIndex++)
			{
				if (*iterIndex == *iterDestIndex)
				{
					Check = true;
					break;
				}
			}

			if (Check)
				break;
		}

		// 서로 겹치는 충돌영역이 없으면 이전 프레임에 충돌하고 있다가 서로 다른 영역으로 충돌체가 포함된 것이다.
		// 둘이 충돌이 해제되었다는 통보를 해주어야 한다.
		if (!Check)
		{
			// 충돌 해제 통보.
			m_Result.Src = this;
			m_Result.Dest = *iter;

			CallCollisionCallback(ECollision_Result::Release);

			(*iter)->m_Result.Src = *iter;
			(*iter)->m_Result.Dest = this;

			(*iter)->CallCollisionCallback(ECollision_Result::Release);

			(*iter)->DeletePrevCollisionCollider(this);
			iter = m_PrevCollisionList.erase(iter);
			iterEnd = m_PrevCollisionList.end();
			continue;
		}

		iter++;
	}
}

void CCollider::SendPrevCollisionEnd()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->DeletePrevCollisionCollider(this);

		(*iter)->m_Result.Src = *iter;
		(*iter)->m_Result.Dest = this;
		(*iter)->CallCollisionCallback(ECollision_Result::Release);

		m_Result.Src = this;
		m_Result.Dest = *iter;
		CallCollisionCallback(ECollision_Result::Release);
	}

	m_PrevCollisionList.clear();
}

void CCollider::CallCollisionCallback(ECollision_Result Type)
{
	if (m_CollisionCallback[(int)Type])
		m_CollisionCallback[(int)Type](m_Result);
}

void CCollider::CallCollisionMouseCallback(ECollision_Result Type)
{
	if (Type == ECollision_Result::Release)
		m_MouseCollision = false;

	if (m_CollisionMouseCallback[(int)Type])
		m_CollisionMouseCallback[(int)Type](m_MouseResult);
}

void CCollider::ClearFrame()
{
	m_CurrentSectionList.clear();
}

int CCollider::CheckOverlapSection(CCollider* Dest)
{
	int	OverlapCount = 0;
	int	MinIndex = INT_MAX;

	auto	iter1 = m_CurrentSectionList.begin();
	auto	iter1End = m_CurrentSectionList.end();

	for (; iter1 != iter1End; iter1++)
	{
		auto	iter2 = Dest->m_CurrentSectionList.begin();
		auto	iter2End = Dest->m_CurrentSectionList.end();

		for (; iter2 != iter2End; iter2++)
		{
			if (*iter1 == *iter2)
			{
				++OverlapCount;

				if (*iter1 < MinIndex)
					MinIndex = *iter1;

				break;
			}
		}
	}

	if (OverlapCount < 2)
		return -1;

	return MinIndex;
}

void CCollider::SetCollisionProfile(const std::string& Name)
{
	m_Profile = CCollisionManager::GetInst()->FindProfile(Name);
}

void CCollider::Start()
{
	CSceneComponent::Start();

	// Scene에 배치가 되고 Start가 호출되면 출력 목록으로 지정한다.
	/*if (CEngine::GetEditorMode())
		CRenderManager::GetInst()->AddRenderList(this);*/

	CRenderManager::GetInst()->AddRenderList(this);

	if (m_Scene)
	{
		m_Scene->GetCollisionManager()->AddCollider(this);
	}
}

bool CCollider::Init()
{
	if (!CSceneComponent::Init())
		return false;

	m_Profile = CCollisionManager::GetInst()->FindProfile("Default");


	/*if (CEngine::GetEditorMode())
	{
		m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");
	}*/

	m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");

	return true;
}

void CCollider::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CCollider::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CCollider::Render()
{
	CSceneComponent::Render();

	if (m_PrevCollisionList.empty() && !m_MouseCollision)
		m_Color = Vector4(0.f, 1.f, 0.f, 1.f);

	else
		m_Color = Vector4(1.f, 0.f, 0.f, 1.f);
}

void CCollider::Save(FILE* File)
{
	CSceneComponent::Save(File);

	fwrite(&m_ColliderType, sizeof(ECollider_Type), 1, File);

	int	Length = (int)m_Profile->Name.size();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_Profile->Name.c_str(), 1, Length, File);
}

void CCollider::Load(FILE* File)
{
	CSceneComponent::Load(File);

	fread(&m_ColliderType, sizeof(ECollider_Type), 1, File);

	int	Length = 0;
	char	Name[256] = {};
	fread(&Length, sizeof(int), 1, File);
	fread(Name, 1, Length, File);

	SetCollisionProfile(Name);

	if (CEngine::GetEditorMode())
	{
		m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");
	}
}

