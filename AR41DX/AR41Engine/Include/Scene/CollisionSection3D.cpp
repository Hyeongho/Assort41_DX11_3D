#include "CollisionSection3D.h"
#include "../Component/Collider3D.h"

CCollisionSection3D::CCollisionSection3D()
{
}

CCollisionSection3D::~CCollisionSection3D()
{
}

void CCollisionSection3D::AddCollider(CCollider3D* Collider)
{
	m_ColliderList.push_back(Collider);

	Collider->AddSectionIndex(m_Index);
}

void CCollisionSection3D::Collision(float DeltaTime)
{
	if (m_ColliderList.size() < 2)
	{
		return;
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	iterEnd--;

	auto iter1End = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		CCollider* Src = *iter;

		auto iter1 = iter;
		iter1++;

		for (; iter1 != iter1End; iter1++)
		{
			CCollider* Dest = *iter1;

			int	OverlapSection = Src->CheckOverlapSection(Dest);

			// -1이면 두 물체는 겹치는 영역이 1개라는 의미이기 때문에 무조건 처리해야 한다.
			// 그래서 -1이 아니고 겹치는 인덱스중 가장 작은 인덱스가 현재 영역의 인덱스보다 작다면
			// 이미 앞에서 두 물체는 충돌처리를 수행했다는 의미이므로 현재 영역에서는 충돌처리를 패스한다.
			if (OverlapSection != -1 && OverlapSection < m_Index)
			{
				continue;
			}

			CollisionProfile* SrcProfile = Src->GetCollisionProfile();
			CollisionProfile* DestProfile = Dest->GetCollisionProfile();

			if (SrcProfile->vecCollisionInteraction[(int)DestProfile->Channel->Channel] ==
				ECollision_Interaction::Ignore ||
				DestProfile->vecCollisionInteraction[(int)SrcProfile->Channel->Channel] ==
				ECollision_Interaction::Ignore)
				continue;

			if (Src->Collision(Dest))
			{
				// 이전프레임에 충돌한적이 없다면 등록해준다.
				if (!Src->CheckPrevCollision(Dest))
				{
					Src->AddPrevCollisionCollider(Dest);
					Dest->AddPrevCollisionCollider(Src);

					Src->SetCollisionResultDest(Dest);
					Dest->SetCollisionResultDest(Src);

					Src->CallCollisionCallback(ECollision_Result::Collision);
					Dest->CallCollisionCallback(ECollision_Result::Collision);
				}

			}

			// 이전 프레임에 충돌된 목록에 Dest가 있다면 이 둘은 이전프레임에 충돌이 되었다가 지금 떨어지게된다는
			// 것을 의미한다.
			else if (Src->CheckPrevCollision(Dest))
			{
				// 이전프레임에 충돌한 목록에서 서로를 제거한다.
				Src->DeletePrevCollisionCollider(Dest);
				Dest->DeletePrevCollisionCollider(Src);

				Src->SetCollisionResultDest(Dest);
				Dest->SetCollisionResultDest(Src);

				Src->CallCollisionCallback(ECollision_Result::Release);
				Dest->CallCollisionCallback(ECollision_Result::Release);
			}
		}
	}
}
