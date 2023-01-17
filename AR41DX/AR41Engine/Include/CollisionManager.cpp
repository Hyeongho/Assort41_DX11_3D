#include "CollisionManager.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderPixel.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	{
		auto	iter = m_mapProfile.begin();
		auto	iterEnd = m_mapProfile.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE(iter->second);
		}
	}

	{
		auto	iter = m_vecChannel.begin();
		auto	iterEnd = m_vecChannel.end();

		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((* iter));
		}
	}
}

bool CCollisionManager::Init()
{
	CreateChannel("Default", ECollision_Interaction::Collision);
	CreateChannel("Mouse", ECollision_Interaction::Collision);

	CreateProfile("Default", "Default", true);
	CreateProfile("Mouse", "Mouse", true);

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& Name, 
	const std::string& ChannelName, bool Enable, 
	ECollision_Interaction BaseInteraction)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (Profile)
		return false;

	CollisionChannel* Channel = nullptr;

	size_t	Count = m_vecChannel.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecChannel[i]->Name == ChannelName)
		{
			Channel = m_vecChannel[i];
			break;
		}
	}

	if (!Channel)
		return false;

	Profile = new CollisionProfile;

	Profile->Name = Name;
	Profile->Channel = Channel;
	Profile->Enable = Enable;
	Profile->vecCollisionInteraction.resize(m_vecChannel.size());

	Count = Profile->vecCollisionInteraction.size();

	for (size_t i = 0; i < Count; ++i)
	{
		Profile->vecCollisionInteraction[i] = BaseInteraction;
	}

	m_mapProfile.insert(std::make_pair(Name, Profile));

	return true;
}

bool CCollisionManager::SetCollisionInteraction(const std::string& Name, 
	const std::string& ChannelName, ECollision_Interaction Interaction)
{
	CollisionProfile* Profile = FindProfile(Name);

	if (!Profile)
		return false;

	CollisionChannel* Channel = nullptr;

	size_t	Count = m_vecChannel.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecChannel[i]->Name == ChannelName)
		{
			Channel = m_vecChannel[i];
			break;
		}
	}

	if (!Channel)
		return false;

	Profile->vecCollisionInteraction[(int)Channel->Channel] = Interaction;

	return true;
}

bool CCollisionManager::CreateChannel(const std::string& Name, 
	ECollision_Interaction Interaction)
{
	size_t	Count = m_vecChannel.size();

	for (size_t i = 0; i < Count; ++i)
	{
		if (m_vecChannel[i]->Name == Name)
			return false;
	}

	CollisionChannel* NewChannel = new CollisionChannel;

	NewChannel->Name = Name;
	NewChannel->Channel = (ECollision_Channel)m_vecChannel.size();
	NewChannel->Interaction = Interaction;

	m_vecChannel.push_back(NewChannel);

	auto	iter = m_mapProfile.begin();
	auto	iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->vecCollisionInteraction.push_back(Interaction);
	}

	return true;
}

CollisionProfile* CCollisionManager::FindProfile(const std::string& Name)
{
	auto	iter = m_mapProfile.find(Name);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}

bool CCollisionManager::CollisionBox2DToBox2D(Vector2& HitPoint, CColliderBox2D* Src, CColliderBox2D* Dest)
{
	if (CollisionBox2DToBox2D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionSphere2DToSphere2D(Vector2& HitPoint, CColliderSphere2D* Src, 
	CColliderSphere2D* Dest)
{
	if (CollisionSphere2DToSphere2D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionOBB2DToOBB2D(Vector2& HitPoint, CColliderOBB2D* Src, CColliderOBB2D* Dest)
{
	if (CollisionOBB2DToOBB2D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBox2DToSphere2D(Vector2& HitPoint, CColliderBox2D* Src, CColliderSphere2D* Dest)
{
	if (CollisionBox2DToSphere2D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBox2DToOBB2D(Vector2& HitPoint, CColliderBox2D* Src, CColliderOBB2D* Dest)
{
	if (CollisionBox2DToOBB2D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionSphere2DToOBB2D(Vector2& HitPoint, CColliderSphere2D* Src, CColliderOBB2D* Dest)
{
	if (CollisionSphere2DToOBB2D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBox2DToPixel(Vector2& HitPoint, CColliderBox2D* Src, CColliderPixel* Dest)
{
	if (CollisionBox2DToPixel(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionSphere2DToPixel(Vector2& HitPoint, CColliderSphere2D* Src, 
	CColliderPixel* Dest)
{
	if (CollisionSphere2DToPixel(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionOBB2DToPixel(Vector2& HitPoint, CColliderOBB2D* Src, 
	CColliderPixel* Dest)
{
	if (CollisionOBB2DToPixel(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionPixelToPixel(Vector2& HitPoint, CColliderPixel* Src, CColliderPixel* Dest)
{
	if (CollisionPixelToPixel(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionPointToBox2D(Vector2& HitPoint, const Vector2& Src, CColliderBox2D* Dest)
{
	if (CollisionPointToBox2D(HitPoint, Src, Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionPointToSphere2D(Vector2& HitPoint, const Vector2& Src, CColliderSphere2D* Dest)
{
	if (CollisionPointToSphere2D(HitPoint, Src, Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionPointToOBB2D(Vector2& HitPoint, const Vector2& Src, CColliderOBB2D* Dest)
{
	if (CollisionPointToOBB2D(HitPoint, Src, Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionPointToPixel(Vector2& HitPoint, const Vector2& Src, CColliderPixel* Dest)
{
	if (CollisionPointToPixel(HitPoint, Src, Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, 0.f);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBox2DToBox2D(Vector2& HitPoint, const Box2DInfo& Src,
	const Box2DInfo& Dest)
{
	if (Src.Left > Dest.Right)
		return false;

	else if (Src.Right < Dest.Left)
		return false;

	else if (Src.Bottom > Dest.Top)
		return false;

	else if (Src.Top < Dest.Bottom)
		return false;

	ComputeHitPoint(HitPoint, Src, Dest);

	return true;
}

bool CCollisionManager::CollisionSphere2DToSphere2D(Vector2& HitPoint, const Sphere2DInfo& Src,
	const Sphere2DInfo& Dest)
{
	float	Dist = Src.Center.Distance(Dest.Center);

	bool result = Dist <= Src.Radius + Dest.Radius;

	HitPoint = (Src.Center + Dest.Center) / 2.f;

	return result;
}

bool CCollisionManager::CollisionOBB2DToOBB2D(Vector2& HitPoint, const OBB2DInfo& Src, 
	const OBB2DInfo& Dest)
{
	Vector2	CenterLine = Src.Center - Dest.Center;

	Vector2	Axis = Src.Axis[AXIS2D_X];

	float CenterProjDist = abs(CenterLine.Dot(Axis));

	float SrcDist, DestDist;

	SrcDist = Src.Length[AXIS2D_X];
	DestDist = abs(Axis.Dot(Dest.Axis[AXIS2D_X]) * Dest.Length[AXIS2D_X]) +
		abs(Axis.Dot(Dest.Axis[AXIS2D_Y]) * Dest.Length[AXIS2D_Y]);

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Src.Axis[AXIS2D_Y];

	CenterProjDist = abs(CenterLine.Dot(Axis));

	SrcDist = Src.Length[AXIS2D_Y];
	DestDist = abs(Axis.Dot(Dest.Axis[AXIS2D_X]) * Dest.Length[AXIS2D_X]) +
		abs(Axis.Dot(Dest.Axis[AXIS2D_Y]) * Dest.Length[AXIS2D_Y]);

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS2D_X];

	CenterProjDist = abs(CenterLine.Dot(Axis));

	SrcDist = abs(Axis.Dot(Src.Axis[AXIS2D_X]) * Src.Length[AXIS2D_X]) +
		abs(Axis.Dot(Src.Axis[AXIS2D_Y]) * Src.Length[AXIS2D_Y]);
	DestDist = Dest.Length[AXIS2D_X];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS2D_Y];

	CenterProjDist = abs(CenterLine.Dot(Axis));

	SrcDist = abs(Axis.Dot(Src.Axis[AXIS2D_X]) * Src.Length[AXIS2D_X]) +
		abs(Axis.Dot(Src.Axis[AXIS2D_Y]) * Src.Length[AXIS2D_Y]);;
	DestDist = Dest.Length[AXIS2D_Y];

	if (CenterProjDist > SrcDist + DestDist)
		return false;


	Box2DInfo	SrcInfo = ConvertBox2DInfo(Src);
	Box2DInfo	DestInfo = ConvertBox2DInfo(Dest);

	ComputeHitPoint(HitPoint, SrcInfo, DestInfo);


	return true;
}

bool CCollisionManager::CollisionBox2DToSphere2D(Vector2& HitPoint, const Box2DInfo& Src,
	const Sphere2DInfo& Dest)
{
	if ((Src.Left <= Dest.Center.x && Dest.Center.x <= Src.Right) ||
		(Src.Bottom <= Dest.Center.y && Dest.Center.y <= Src.Top))
	{
		Box2DInfo	Info = Src;
		Info.Left -= Dest.Radius;
		Info.Bottom -= Dest.Radius;
		Info.Right += Dest.Radius;
		Info.Top += Dest.Radius;

		if (Info.Left > Dest.Center.x)
			return false;

		else if (Info.Bottom > Dest.Center.y)
			return false;

		else if (Info.Right < Dest.Center.x)
			return false;

		else if (Info.Top < Dest.Center.y)
			return false;

		Box2DInfo	OverlapBox = ConvertBox2DInfo(Dest);

		ComputeHitPoint(HitPoint, Src, OverlapBox);

		return true;
	}

	Vector2	Pos[4] =
	{
		Vector2(Src.Left, Src.Top),
		Vector2(Src.Right, Src.Top),
		Vector2(Src.Left, Src.Bottom),
		Vector2(Src.Right, Src.Bottom)
	};

	for (int i = 0; i < 4; ++i)
	{
		float Dist = Dest.Center.Distance(Pos[i]);

		if (Dist <= Dest.Radius)
		{
			Box2DInfo	OverlapBox = ConvertBox2DInfo(Dest);

			ComputeHitPoint(HitPoint, Src, OverlapBox);

			return true;
		}
	}

	return false;
}

bool CCollisionManager::CollisionBox2DToOBB2D(Vector2& HitPoint, const Box2DInfo& Src, 
	const OBB2DInfo& Dest)
{
	OBB2DInfo	SrcInfo;
	SrcInfo.Center = Vector2((Src.Left + Src.Right) / 2.f, (Src.Top + Src.Bottom) / 2.f);
	SrcInfo.Axis[AXIS2D_X] = Vector2(1.f, 0.f);
	SrcInfo.Axis[AXIS2D_Y] = Vector2(0.f, 1.f);
	SrcInfo.Length[AXIS2D_X] = (Src.Right - Src.Left) / 2.f;
	SrcInfo.Length[AXIS2D_Y] = (Src.Top - Src.Bottom) / 2.f;

	return CollisionOBB2DToOBB2D(HitPoint, SrcInfo, Dest);
}

bool CCollisionManager::CollisionSphere2DToOBB2D(Vector2& HitPoint, const Sphere2DInfo& Src, const OBB2DInfo& Dest)
{
	Vector2	CenterLine = Src.Center - Dest.Center;

	float CenterProjDist = CenterLine.Length();

	Vector2	Axis = CenterLine;
	Axis.Normalize();

	float SrcDist, DestDist;

	SrcDist = Src.Radius;
	DestDist = abs(Axis.Dot(Dest.Axis[AXIS2D_X]) * Dest.Length[AXIS2D_X]) +
		abs(Axis.Dot(Dest.Axis[AXIS2D_Y]) * Dest.Length[AXIS2D_Y]);

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS2D_X];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Dest.Length[AXIS2D_X];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS2D_Y];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Dest.Length[AXIS2D_Y];

	if (CenterProjDist > SrcDist + DestDist)
		return false;



	Box2DInfo	SrcInfo, DestInfo;
	SrcInfo = ConvertBox2DInfo(Src);
	DestInfo = ConvertBox2DInfo(Dest);

	ComputeHitPoint(HitPoint, SrcInfo, DestInfo);

	return true;
}

bool CCollisionManager::CollisionBox2DToPixel(Vector2& HitPoint, const Box2DInfo& Src, 
	const PixelInfo& Dest)
{
	// �ȼ��浹ü�� �����ϴ� �簢���� �浹ó���� ���� �Ѵ�.
	if (!CollisionBox2DToBox2D(HitPoint, Src, Dest.Box2D))
		return false;

	// �� �簢���� �������� ������ ���ϰ� �ش� ������ �ȼ������� Ȯ���Ͽ� �浹�� �Ǿ������� �Ǵ��Ѵ�.
	Box2DInfo OverlapInfo = OverlapBox2D(Src, Dest.Box2D);

	OverlapInfo.Left -= Dest.Box2D.Left;
	OverlapInfo.Bottom -= Dest.Box2D.Bottom;
	OverlapInfo.Right -= Dest.Box2D.Left;
	OverlapInfo.Top -= Dest.Box2D.Bottom;

	float Width = (float)Dest.Width;
	float Height = (float)Dest.Height;

	OverlapInfo.Left = OverlapInfo.Left < 0.f ? 0.f : OverlapInfo.Left;
	OverlapInfo.Bottom = OverlapInfo.Bottom < 0.f ? 0.f : OverlapInfo.Bottom;

	OverlapInfo.Right = OverlapInfo.Right > Width ? Width - 1.f : OverlapInfo.Right;
	OverlapInfo.Top = OverlapInfo.Top > Height ? Height - 1.f : OverlapInfo.Top;

	// Bottom�� Top�� ����� �̹����� �����ش�.
	OverlapInfo.Bottom = Height - OverlapInfo.Bottom;
	OverlapInfo.Top = Height - OverlapInfo.Top;

	bool	Collision = false;

	for (int y = (int)OverlapInfo.Top; y < (int)OverlapInfo.Bottom; ++y)
	{
		for (int x = (int)OverlapInfo.Left; x < (int)OverlapInfo.Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			switch (Dest.PixelColorCollisionType)
			{
			case EPixelCollision_Type::Color_Ignore:
				if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
					Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
					Dest.Pixel[Index + 2] == Dest.TypeColor[2])
					continue;

				Collision = true;
				break;
			case EPixelCollision_Type::Color_Confirm:
				if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
					Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
					Dest.Pixel[Index + 2] == Dest.TypeColor[2])
					Collision = true;

				else
					continue;
				break;
			}

			switch (Dest.PixelAlphaCollisionType)
			{
			case EPixelCollision_Type::Alpha_Ignore:
				if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
					continue;

				Collision = true;
				break;
			case EPixelCollision_Type::Alpha_Confirm:
				if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
					Collision = true;

				else
					continue;
				break;
			}

			if (Collision)
				break;
		}

		if (Collision)
			break;
	}

	ComputeHitPoint(HitPoint, Src, Dest.Box2D);

	return Collision;
}

bool CCollisionManager::CollisionSphere2DToPixel(Vector2& HitPoint, const Sphere2DInfo& Src, 
	const PixelInfo& Dest)
{
	// �ȼ��浹ü�� �����ϴ� �簢���� �浹ó���� ���� �Ѵ�.
	if (!CollisionBox2DToSphere2D(HitPoint, Dest.Box2D, Src))
		return false;

	// �� �簢���� �������� ������ ���ϰ� �ش� ������ �ȼ������� Ȯ���Ͽ� �浹�� �Ǿ������� �Ǵ��Ѵ�.
	Box2DInfo OverlapInfo = OverlapBox2D(Dest.Box2D, Src);

	OverlapInfo.Left -= Dest.Box2D.Left;
	OverlapInfo.Bottom -= Dest.Box2D.Bottom;
	OverlapInfo.Right -= Dest.Box2D.Left;
	OverlapInfo.Top -= Dest.Box2D.Bottom;

	float Width = (float)Dest.Width;
	float Height = (float)Dest.Height;

	OverlapInfo.Left = OverlapInfo.Left < 0.f ? 0.f : OverlapInfo.Left;
	OverlapInfo.Bottom = OverlapInfo.Bottom < 0.f ? 0.f : OverlapInfo.Bottom;

	OverlapInfo.Right = OverlapInfo.Right > Width ? Width - 1.f : OverlapInfo.Right;
	OverlapInfo.Top = OverlapInfo.Top > Height ? Height - 1.f : OverlapInfo.Top;

	// Bottom�� Top�� ����� �̹����� �����ش�.
	OverlapInfo.Bottom = Height - OverlapInfo.Bottom;
	OverlapInfo.Top = Height - OverlapInfo.Top;

	bool	Collision = false;

	for (int y = (int)OverlapInfo.Top; y < (int)OverlapInfo.Bottom; ++y)
	{
		for (int x = (int)OverlapInfo.Left; x < (int)OverlapInfo.Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			// �ȼ��� �ε����� �̿��Ͽ� ����������� ��ȯ���ش�.
			Vector2	PixelWorldPos = Vector2(Dest.Box2D.Left, Dest.Box2D.Bottom) +
				Vector2((float)x, (float)Height - (float)y);

			if (!CollisionPointToSphere2D(HitPoint, PixelWorldPos, Src))
				continue;

			switch (Dest.PixelColorCollisionType)
			{
			case EPixelCollision_Type::Color_Ignore:
				if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
					Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
					Dest.Pixel[Index + 2] == Dest.TypeColor[2])
					continue;

				Collision = true;
				break;
			case EPixelCollision_Type::Color_Confirm:
				if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
					Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
					Dest.Pixel[Index + 2] == Dest.TypeColor[2])
					Collision = true;

				else
					continue;
				break;
			}

			switch (Dest.PixelAlphaCollisionType)
			{
			case EPixelCollision_Type::Alpha_Ignore:
				if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
					continue;

				Collision = true;
				break;
			case EPixelCollision_Type::Alpha_Confirm:
				if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
					Collision = true;

				else
					continue;
				break;
			}

			if (Collision)
				break;
		}

		if (Collision)
			break;
	}

	ComputeHitPoint(HitPoint, OverlapInfo, Dest.Box2D);

	return Collision;
}

bool CCollisionManager::CollisionOBB2DToPixel(Vector2& HitPoint, const OBB2DInfo& Src,
	const PixelInfo& Dest)
{
	// �ȼ��浹ü�� �����ϴ� �簢���� �浹ó���� ���� �Ѵ�.
	if (!CollisionBox2DToOBB2D(HitPoint, Dest.Box2D, Src))
		return false;

	// �� �簢���� �������� ������ ���ϰ� �ش� ������ �ȼ������� Ȯ���Ͽ� �浹�� �Ǿ������� �Ǵ��Ѵ�.
	Box2DInfo OverlapInfo = OverlapBox2D(Dest.Box2D, Src);

	OverlapInfo.Left -= Dest.Box2D.Left;
	OverlapInfo.Bottom -= Dest.Box2D.Bottom;
	OverlapInfo.Right -= Dest.Box2D.Left;
	OverlapInfo.Top -= Dest.Box2D.Bottom;

	float Width = (float)Dest.Width;
	float Height = (float)Dest.Height;

	OverlapInfo.Left = OverlapInfo.Left < 0.f ? 0.f : OverlapInfo.Left;
	OverlapInfo.Bottom = OverlapInfo.Bottom < 0.f ? 0.f : OverlapInfo.Bottom;

	OverlapInfo.Right = OverlapInfo.Right > Width ? Width - 1.f : OverlapInfo.Right;
	OverlapInfo.Top = OverlapInfo.Top > Height ? Height - 1.f : OverlapInfo.Top;

	// Bottom�� Top�� ����� �̹����� �����ش�.
	OverlapInfo.Bottom = Height - OverlapInfo.Bottom;
	OverlapInfo.Top = Height - OverlapInfo.Top;

	bool	Collision = false;

	for (int y = (int)OverlapInfo.Top; y < (int)OverlapInfo.Bottom; ++y)
	{
		for (int x = (int)OverlapInfo.Left; x < (int)OverlapInfo.Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			// �ȼ��� �ε����� �̿��Ͽ� ����������� ��ȯ���ش�.
			Vector2	PixelWorldPos = Vector2(Dest.Box2D.Left, Dest.Box2D.Bottom) +
				Vector2((float)x, (float)Height - (float)y);

			if (!CollisionPointToOBB2D(HitPoint, PixelWorldPos, Src))
				continue;

			switch (Dest.PixelColorCollisionType)
			{
			case EPixelCollision_Type::Color_Ignore:
				if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
					Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
					Dest.Pixel[Index + 2] == Dest.TypeColor[2])
					continue;

				Collision = true;
				break;
			case EPixelCollision_Type::Color_Confirm:
				if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
					Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
					Dest.Pixel[Index + 2] == Dest.TypeColor[2])
					Collision = true;

				else
					continue;
				break;
			}

			switch (Dest.PixelAlphaCollisionType)
			{
			case EPixelCollision_Type::Alpha_Ignore:
				if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
					continue;

				Collision = true;
				break;
			case EPixelCollision_Type::Alpha_Confirm:
				if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
					Collision = true;

				else
					continue;
				break;
			}

			if (Collision)
				break;
		}

		if (Collision)
			break;
	}

	ComputeHitPoint(HitPoint, OverlapInfo, Dest.Box2D);

	return Collision;
}

bool CCollisionManager::CollisionPixelToPixel(Vector2& HitPoint, const PixelInfo& Src, const PixelInfo& Dest)
{
	return false;
}

bool CCollisionManager::CollisionPointToBox2D(Vector2& HitPoint, const Vector2& Src, 
	const Box2DInfo& Dest)
{
	if (Src.x < Dest.Left)
		return false;

	else if (Src.x > Dest.Right)
		return false;

	else if (Src.y < Dest.Bottom)
		return false;

	else if (Src.y > Dest.Top)
		return false;

	HitPoint = Src;

	return true;
}

bool CCollisionManager::CollisionPointToSphere2D(Vector2& HitPoint, const Vector2& Src, 
	const Sphere2DInfo& Dest)
{
	bool result = Src.Distance(Dest.Center) <= Dest.Radius;

	if (result)
		HitPoint = Src;

	return result;
}

bool CCollisionManager::CollisionPointToOBB2D(Vector2& HitPoint, const Vector2& Src, 
	const OBB2DInfo& Dest)
{
	Vector2	CenterLine = Src - Dest.Center;

	Vector2	Axis = Dest.Axis[AXIS2D_X];

	float CenterProjDist = abs(Axis.Dot(CenterLine));

	if (CenterProjDist > Dest.Length[AXIS2D_X])
		return false;

	Axis = Dest.Axis[AXIS2D_Y];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	if (CenterProjDist > Dest.Length[AXIS2D_Y])
		return false;

	HitPoint = Src;

	return true;
}

bool CCollisionManager::CollisionPointToPixel(Vector2& HitPoint, const Vector2& Src, 
	const PixelInfo& Dest)
{
	// �ȼ��浹ü�� �����ϴ� �簢���� �浹ó���� ���� �Ѵ�.
	if (!CollisionPointToBox2D(HitPoint, Src, Dest.Box2D))
		return false;

	Vector2	ConvertSrc = Src;

	ConvertSrc.x -= Dest.Box2D.Left;
	ConvertSrc.y -= Dest.Box2D.Bottom;

	ConvertSrc.y = Dest.Height - ConvertSrc.y;

	bool	Collision = false;

	int	Index = (int)ConvertSrc.y * (int)Dest.Width * 4 + (int)ConvertSrc.x * 4;

	switch (Dest.PixelColorCollisionType)
	{
	case EPixelCollision_Type::Color_Ignore:
		if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
			Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
			Dest.Pixel[Index + 2] == Dest.TypeColor[2])
			Collision = false;

		else
			Collision = true;
		break;
	case EPixelCollision_Type::Color_Confirm:
		if (Dest.Pixel[Index] == Dest.TypeColor[0] &&
			Dest.Pixel[Index + 1] == Dest.TypeColor[1] &&
			Dest.Pixel[Index + 2] == Dest.TypeColor[2])
			Collision = true;

		else
			Collision = false;
		break;
	}

	switch (Dest.PixelAlphaCollisionType)
	{
	case EPixelCollision_Type::Alpha_Ignore:
		if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
			Collision = false;

		else
			Collision = true;
		break;
	case EPixelCollision_Type::Alpha_Confirm:
		if (Dest.Pixel[Index + 3] == Dest.TypeColor[3])
			Collision = true;

		else
			Collision = false;
		break;
	}

	HitPoint = Src;

	return Collision;
}

Box2DInfo CCollisionManager::ConvertBox2DInfo(const Sphere2DInfo& Info)
{
	Box2DInfo	result;

	result.Left = Info.Center.x - Info.Radius;
	result.Bottom = Info.Center.y - Info.Radius;
	result.Right = Info.Center.x + Info.Radius;
	result.Top = Info.Center.y + Info.Radius;

	return result;
}

Box2DInfo CCollisionManager::ConvertBox2DInfo(const OBB2DInfo& Info)
{
	Box2DInfo	result;

	Vector2	Pos[4];

	Pos[0] = Info.Center - Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] +
		Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

	Pos[1] = Info.Center + Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] +
		Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

	Pos[2] = Info.Center - Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] -
		Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

	Pos[3] = Info.Center + Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] -
		Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

	result.Left = Pos[0].x;
	result.Bottom = Pos[0].y;

	result.Right = Pos[0].x;
	result.Top = Pos[0].y;

	for (int i = 1; i < 4; ++i)
	{
		result.Left = result.Left > Pos[i].x ? Pos[i].x : result.Left;
		result.Bottom = result.Bottom > Pos[i].y ? Pos[i].y : result.Bottom;

		result.Right = result.Right < Pos[i].x ? Pos[i].x : result.Right;
		result.Top = result.Top < Pos[i].y ? Pos[i].y : result.Top;
	}

	return result;
}

Box2DInfo CCollisionManager::OverlapBox2D(const Box2DInfo& Src, const Box2DInfo& Dest)
{
	Box2DInfo	Info;

	Info.Left = Src.Left > Dest.Left ? Src.Left : Dest.Left;
	Info.Bottom = Src.Bottom > Dest.Bottom ? Src.Bottom : Dest.Bottom;
	Info.Right = Src.Right < Dest.Right ? Src.Right : Dest.Right;
	Info.Top = Src.Top < Dest.Top ? Src.Top : Dest.Top;

	return Info;
}

Box2DInfo CCollisionManager::OverlapBox2D(const Box2DInfo& Src, const Sphere2DInfo& Dest)
{
	Box2DInfo	Info, DestInfo;

	DestInfo = ConvertBox2DInfo(Dest);

	Info.Left = Src.Left > DestInfo.Left ? Src.Left : DestInfo.Left;
	Info.Bottom = Src.Bottom > DestInfo.Bottom ? Src.Bottom : DestInfo.Bottom;
	Info.Right = Src.Right < DestInfo.Right ? Src.Right : DestInfo.Right;
	Info.Top = Src.Top < DestInfo.Top ? Src.Top : DestInfo.Top;

	return Info;
}

Box2DInfo CCollisionManager::OverlapBox2D(const Box2DInfo& Src, const OBB2DInfo& Dest)
{
	Box2DInfo	Info, DestInfo;

	DestInfo = ConvertBox2DInfo(Dest);

	Info.Left = Src.Left > DestInfo.Left ? Src.Left : DestInfo.Left;
	Info.Bottom = Src.Bottom > DestInfo.Bottom ? Src.Bottom : DestInfo.Bottom;
	Info.Right = Src.Right < DestInfo.Right ? Src.Right : DestInfo.Right;
	Info.Top = Src.Top < DestInfo.Top ? Src.Top : DestInfo.Top;

	return Info;
}

void CCollisionManager::ComputeHitPoint(Vector2& HitPoint, const Box2DInfo& Src, const Box2DInfo& Dest)
{
	float Left = Src.Left > Dest.Left ? Src.Left : Dest.Left;
	float Bottom = Src.Bottom > Dest.Bottom ? Src.Bottom : Dest.Bottom;
	float Right = Src.Right < Dest.Right ? Src.Right : Dest.Right;
	float Top = Src.Top < Dest.Top ? Src.Top : Dest.Top;

	HitPoint.x = (Left + Right) / 2.f;
	HitPoint.y = (Top + Bottom) / 2.f;
}
