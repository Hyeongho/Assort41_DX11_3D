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

			// -1�̸� �� ��ü�� ��ġ�� ������ 1����� �ǹ��̱� ������ ������ ó���ؾ� �Ѵ�.
			// �׷��� -1�� �ƴϰ� ��ġ�� �ε����� ���� ���� �ε����� ���� ������ �ε������� �۴ٸ�
			// �̹� �տ��� �� ��ü�� �浹ó���� �����ߴٴ� �ǹ��̹Ƿ� ���� ���������� �浹ó���� �н��Ѵ�.
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
				// ���������ӿ� �浹������ ���ٸ� ������ش�.
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

			// ���� �����ӿ� �浹�� ��Ͽ� Dest�� �ִٸ� �� ���� ���������ӿ� �浹�� �Ǿ��ٰ� ���� �������Եȴٴ�
			// ���� �ǹ��Ѵ�.
			else if (Src->CheckPrevCollision(Dest))
			{
				// ���������ӿ� �浹�� ��Ͽ��� ���θ� �����Ѵ�.
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
