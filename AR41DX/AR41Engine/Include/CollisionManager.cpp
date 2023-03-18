#include "CollisionManager.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderCube.h"
#include "Component/ColliderOBB3D.h"
#include "Component/ColliderSphere3D.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	{
		auto	iter = m_mapProfile.begin();
		auto	iterEnd = m_mapProfile.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE(iter->second);
		}
	}

	{
		auto	iter = m_vecChannel.begin();
		auto	iterEnd = m_vecChannel.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE((* iter));
		}
	}
}

bool CCollisionManager::Init()
{
	CreateChannel("Default", ECollision_Interaction::Collision);
	CreateChannel("Mouse", ECollision_Interaction::Collision);
	CreateChannel("Player", ECollision_Interaction::Collision);
	CreateChannel("Monster", ECollision_Interaction::Collision);
	CreateChannel("DetectArea", ECollision_Interaction::Collision);

	CreateProfile("Default", "Default", true);
	CreateProfile("Mouse", "Mouse", true);
	CreateProfile("Player", "Player", true);
	CreateProfile("Monster", "Monster", true);
	CreateProfile("DetectArea", "DetectArea", true);

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

bool CCollisionManager::CheckCollisionInteraction(const std::string& name, const std::string& channelName,
	ECollision_Interaction interaction)
{
	CollisionProfile* profile = FindProfile(name);
	if (!profile)
	{
		return false;
	}
	CollisionChannel* channel = nullptr;
	size_t	count = m_vecChannel.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (m_vecChannel[i]->Name == channelName)
		{
			channel = m_vecChannel[i];
			break;
		}
	}
	if (!channel)
	{
		return false;
	}
	return profile->vecCollisionInteraction[(int)channel->Channel] == interaction;
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

	for (; iter != iterEnd; iter++)
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

void CCollisionManager::GetProfileNames(std::vector<std::string>& vecNames)
{
	auto	iter = m_mapProfile.begin();
	auto	iterEnd = m_mapProfile.end();
	for (; iter != iterEnd; ++iter)
	{
		vecNames.push_back(iter->first);
	}
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

	SrcDist = abs(Axis.Dot(Src.Axis[AXIS2D_X]) * Src.Length[AXIS2D_X]) + abs(Axis.Dot(Src.Axis[AXIS2D_Y]) * Src.Length[AXIS2D_Y]);
	DestDist = Dest.Length[AXIS2D_Y];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Box2DInfo	SrcInfo = ConvertBox2DInfo(Src);
	Box2DInfo	DestInfo = ConvertBox2DInfo(Dest);

	ComputeHitPoint(HitPoint, SrcInfo, DestInfo);

	return true;
}

bool CCollisionManager::CollisionBox2DToSphere2D(Vector2& HitPoint, const Box2DInfo& Src, const Sphere2DInfo& Dest)
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
			Box2DInfo OverlapBox = ConvertBox2DInfo(Dest);

			ComputeHitPoint(HitPoint, Src, OverlapBox);

			return true;
		}
	}

	return false;
}

bool CCollisionManager::CollisionBox2DToOBB2D(Vector2& HitPoint, const Box2DInfo& Src, const OBB2DInfo& Dest)
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
	// 픽셀충돌체를 구성하는 사각형과 충돌처리를 먼저 한다.
	if (!CollisionBox2DToBox2D(HitPoint, Src, Dest.Box2D))
		return false;

	// 두 사각형이 겹쳐지는 영역을 구하고 해당 영역의 픽셀정보를 확인하여 충돌이 되었는지를 판단한다.
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

	// Bottom과 Top을 뒤집어서 이미지에 맞춰준다.
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
	// 픽셀충돌체를 구성하는 사각형과 충돌처리를 먼저 한다.
	if (!CollisionBox2DToSphere2D(HitPoint, Dest.Box2D, Src))
		return false;

	// 두 사각형이 겹쳐지는 영역을 구하고 해당 영역의 픽셀정보를 확인하여 충돌이 되었는지를 판단한다.
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

	// Bottom과 Top을 뒤집어서 이미지에 맞춰준다.
	OverlapInfo.Bottom = Height - OverlapInfo.Bottom;
	OverlapInfo.Top = Height - OverlapInfo.Top;

	bool	Collision = false;

	for (int y = (int)OverlapInfo.Top; y < (int)OverlapInfo.Bottom; ++y)
	{
		for (int x = (int)OverlapInfo.Left; x < (int)OverlapInfo.Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			// 픽셀의 인덱스를 이용하여 월드공간으로 변환해준다.
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
	// 픽셀충돌체를 구성하는 사각형과 충돌처리를 먼저 한다.
	if (!CollisionBox2DToOBB2D(HitPoint, Dest.Box2D, Src))
		return false;

	// 두 사각형이 겹쳐지는 영역을 구하고 해당 영역의 픽셀정보를 확인하여 충돌이 되었는지를 판단한다.
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

	// Bottom과 Top을 뒤집어서 이미지에 맞춰준다.
	OverlapInfo.Bottom = Height - OverlapInfo.Bottom;
	OverlapInfo.Top = Height - OverlapInfo.Top;

	bool	Collision = false;

	for (int y = (int)OverlapInfo.Top; y < (int)OverlapInfo.Bottom; ++y)
	{
		for (int x = (int)OverlapInfo.Left; x < (int)OverlapInfo.Right; ++x)
		{
			int	Index = y * (int)Dest.Width * 4 + x * 4;

			// 픽셀의 인덱스를 이용하여 월드공간으로 변환해준다.
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
	// 픽셀충돌체를 구성하는 사각형과 충돌처리를 먼저 한다.
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

bool CCollisionManager::CollisionCubeToCube(Vector3& HitPoint,  CColliderCube* Src, CColliderCube* Dest)
{
	if (CollisionCubeToCube(HitPoint, Src->GetCubeInfo(), Dest->GetCubeInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionCubeToOBB3D(Vector3& HitPoint, CColliderCube* Src, CColliderOBB3D* Dest)
{
	if (CollisionCubeToOBB3D(HitPoint, Src->GetCubeInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionCubeToSphere3D(Vector3& HitPoint, CColliderCube* Src, CColliderSphere3D* Dest)
{
	if (CollisionCubeToSphere3D(HitPoint, Src->GetCubeInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionOBB3DToOBB3D(Vector3& HitPoint, CColliderOBB3D* Src, CColliderOBB3D* Dest)
{
	if (CollisionOBB3DToOBB3D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionOBB3DToCube(Vector3& HitPoint, CColliderOBB3D* Src, CColliderCube* Dest)
{
	if (CollisionOBB3DToCube(HitPoint, Src->GetInfo(), Dest->GetCubeInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionOBB3DToSphere3D(Vector3& HitPoint, CColliderOBB3D* Src, CColliderSphere3D* Dest)
{
	if (CollisionOBB3DToSphere3D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionSphere3DToCube(Vector3& HitPoint, CColliderSphere3D* Src, CColliderCube* Dest)
{
	if (CollisionSphere3DToCube(HitPoint, Src->GetInfo(), Dest->GetCubeInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionSphere3DToOBB3D(Vector3& HitPoint, CColliderSphere3D* Src, CColliderOBB3D* Dest)
{
	if (CollisionSphere3DToOBB3D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionSphere3DToSphere3D(Vector3& HitPoint, CColliderSphere3D* Src, CColliderSphere3D* Dest)
{
	if (CollisionSphere3DToSphere3D(HitPoint, Src->GetInfo(), Dest->GetInfo()))
	{
		Dest->m_HitPoint = Vector3(HitPoint.x, HitPoint.y, HitPoint.z);
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionCubeToCube(Vector3& HitPoint, const CubeInfo& Src, const CubeInfo& Dest)
{
	if (Src.Left > Dest.Right)
		return false;

	else if (Src.Right < Dest.Left)
		return false;

	else if (Src.Bottom > Dest.Top)
		return false;

	else if (Src.Top < Dest.Bottom)
		return false;

	else if (Src.Front > Dest.Back)
		return false;

	else if (Src.Back < Dest.Front)
		return false;

	ComputeHitPoint(HitPoint, Src, Dest);

	return true;
}

bool CCollisionManager::CollisionCubeToOBB3D(Vector3& HitPoint, const CubeInfo& Src, const OBB3DInfo& Dest)
{
	OBB3DInfo SrcInfo;

	SrcInfo.Center = Vector3((Src.Left + Src.Right) / 2.f, (Src.Top + Src.Bottom) / 2.f, (Src.Front + Src.Back) / 2.f);
	SrcInfo.Axis[AXIS_X] = Vector3(1.f, 0.f, 0.f);
	SrcInfo.Axis[AXIS_Y] = Vector3(0.f, 1.f, 0.f);
	SrcInfo.Axis[AXIS_Z] = Vector3(0.f, 0.f, 1.f);

	SrcInfo.Length[AXIS_X] = (Src.Right - Src.Left) / 2.f;
	SrcInfo.Length[AXIS_Y] = (Src.Top - Src.Bottom) / 2.f;
	SrcInfo.Length[AXIS_Z] = (Src.Back - Src.Front) / 2.f;

	return CollisionOBB3DToOBB3D(HitPoint, SrcInfo, Dest);
}

bool CCollisionManager::CollisionCubeToSphere3D(Vector3& HitPoint, const CubeInfo& Src, const Sphere3DInfo& Dest)
{
	if ((Src.Left <= Dest.Center.x && Dest.Center.x <= Src.Right) ||
		(Src.Bottom <= Dest.Center.y && Dest.Center.y <= Src.Top) ||
		(Src.Front <= Dest.Center.z && Dest.Center.z <= Src.Back))
	{
		CubeInfo Info = Src;
		Info.Left -= Dest.Radius;
		Info.Bottom -= Dest.Radius;
		Info.Right += Dest.Radius;
		Info.Top += Dest.Radius;
		Info.Front -= Dest.Radius;
		Info.Back += Dest.Radius;

		if (Info.Left > Dest.Center.x)
			return false;

		else if (Info.Bottom > Dest.Center.y)
			return false;

		else if (Info.Front > Dest.Center.z)
			return false;

		else if (Info.Right < Dest.Center.x)
			return false;

		else if (Info.Top < Dest.Center.y)
			return false;

		else if (Info.Back < Dest.Center.z)
			return false;

		CubeInfo	OverlapBox = ConvertCubeInfo(Dest);

		ComputeHitPoint(HitPoint, Src, OverlapBox);

		return true;
	}

	Vector3	Pos[8] =
	{
		Vector3(Src.Left, Src.Bottom, Src.Front),
		Vector3(Src.Left, Src.Bottom, Src.Back),
		Vector3(Src.Left, Src.Top, Src.Front),
		Vector3(Src.Left, Src.Top, Src.Back),
		Vector3(Src.Right, Src.Bottom, Src.Front),
		Vector3(Src.Right, Src.Bottom, Src.Back),
		Vector3(Src.Right, Src.Top, Src.Front),
		Vector3(Src.Right, Src.Top, Src.Back),
	};

	for (int i = 0; i < 8; ++i)
	{
		float Dist = Dest.Center.Distance(Pos[i]);

		if (Dist <= Dest.Radius)
		{
			CubeInfo OverlapBox = ConvertCubeInfo(Dest);

			ComputeHitPoint(HitPoint, Src, OverlapBox);

			return true;
		}
	}

	return false;
}

bool CCollisionManager::CollisionOBB3DToOBB3D(Vector3& HitPoint, const OBB3DInfo& Src, const OBB3DInfo& Dest)
{
	Vector3	CenterLine = Src.Center - Dest.Center;

	Vector3	Axis = Src.Axis[AXIS_X];

	float CenterProjDist = abs(CenterLine.Dot(Axis));

	float SrcDist, DestDist;

	SrcDist = Src.Length[AXIS_X];

	DestDist = abs(Axis.Dot(Dest.Axis[AXIS_X]) * Dest.Length[AXIS_X]) +
			   abs(Axis.Dot(Dest.Axis[AXIS_Y]) * Dest.Length[AXIS_Y])+
			   abs(Axis.Dot(Dest.Axis[AXIS_Z]) * Dest.Length[AXIS_Z]);

	if (CenterProjDist > SrcDist + DestDist)
	{
		return false;
	}

	Axis = Src.Axis[AXIS_Y];

	CenterProjDist = abs(CenterLine.Dot(Axis));

	SrcDist = Src.Length[AXIS_Y];

	DestDist = abs(Axis.Dot(Dest.Axis[AXIS_X]) * Dest.Length[AXIS_X]) +
			   abs(Axis.Dot(Dest.Axis[AXIS_Y]) * Dest.Length[AXIS_Y])+
			   abs(Axis.Dot(Dest.Axis[AXIS_Z]) * Dest.Length[AXIS_Z]);

	if (CenterProjDist > SrcDist + DestDist)
	{
		return false;
  }

	Axis = Src.Axis[AXIS_Z];

	CenterProjDist = abs(CenterLine.Dot(Axis));

	SrcDist = Src.Length[AXIS_Z];

	DestDist = abs(Axis.Dot(Dest.Axis[AXIS_X]) * Dest.Length[AXIS_X]) + 
			   abs(Axis.Dot(Dest.Axis[AXIS_Y]) * Dest.Length[AXIS_Y])+
			   abs(Axis.Dot(Dest.Axis[AXIS_Z]) * Dest.Length[AXIS_Z]);

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS_X];

	CenterProjDist = abs(CenterLine.Dot(Axis));

	SrcDist = abs(Axis.Dot(Src.Axis[AXIS_X]) * Src.Length[AXIS_X]) +
			  abs(Axis.Dot(Src.Axis[AXIS_Y]) * Src.Length[AXIS_Y])+
			  abs(Axis.Dot(Src.Axis[AXIS_Z]) * Src.Length[AXIS_Z]);

	DestDist = Dest.Length[AXIS_X];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS_Y];

	CenterProjDist = abs(CenterLine.Dot(Axis));


	SrcDist = abs(Axis.Dot(Src.Axis[AXIS_X]) * Src.Length[AXIS_X]) + 
			  abs(Axis.Dot(Src.Axis[AXIS_Y]) * Src.Length[AXIS_Y])+
			  abs(Axis.Dot(Src.Axis[AXIS_Z]) * Src.Length[AXIS_Z]);

	DestDist = Dest.Length[AXIS_Y];

	if (CenterProjDist > SrcDist + DestDist)
	{
		return false;
	}

	Axis = Dest.Axis[AXIS_Z];

	CenterProjDist = abs(CenterLine.Dot(Axis));

	SrcDist = abs(Axis.Dot(Src.Axis[AXIS_X]) * Src.Length[AXIS_X]) + 
			  abs(Axis.Dot(Src.Axis[AXIS_Y]) * Src.Length[AXIS_Y])+
			  abs(Axis.Dot(Src.Axis[AXIS_Z]) * Src.Length[AXIS_Z]);

	DestDist = Dest.Length[AXIS_Z];

	if (CenterProjDist > SrcDist + DestDist)
	{
		return false;
	}

	CubeInfo SrcInfo = ConvertCubeInfo(Src);
	CubeInfo DestInfo = ConvertCubeInfo(Dest);

	ComputeHitPoint(HitPoint, SrcInfo, DestInfo);

	return true;
}

bool CCollisionManager::CollisionOBB3DToCube(Vector3& HitPoint, const OBB3DInfo& Src, const CubeInfo& Dest)
{
	OBB3DInfo DestInfo;

	DestInfo.Center = Vector3((Dest.Left + Dest.Right) / 2.f, (Dest.Top + Dest.Bottom) / 2.f, (Dest.Front + Dest.Back) / 2.f);
	DestInfo.Axis[AXIS_X] = Vector3(1.f, 0.f, 0.f);
	DestInfo.Axis[AXIS_Y] = Vector3(0.f, 1.f, 0.f);
	DestInfo.Axis[AXIS_Z] = Vector3(0.f, 0.f, 1.f);

	DestInfo.Length[AXIS_X] = (Dest.Right - Dest.Left) / 2.f;
	DestInfo.Length[AXIS_Y] = (Dest.Top - Dest.Bottom) / 2.f;
	DestInfo.Length[AXIS_Z] = (Dest.Back - Dest.Front) / 2.f;

	return CollisionOBB3DToOBB3D(HitPoint, Src, DestInfo);
}

bool CCollisionManager::CollisionOBB3DToSphere3D(Vector3& HitPoint, const OBB3DInfo& Src, const Sphere3DInfo& Dest)
{
	Vector3	CenterLine = Dest.Center - Src.Center;

	float CenterProjDist = CenterLine.Length();

	Vector3	Axis = CenterLine;
	Axis.Normalize();

	float SrcDist, DestDist;

	SrcDist = Dest.Radius;
	DestDist = abs(Axis.Dot(Src.Axis[AXIS_X]) * Src.Length[AXIS_X]) +
		abs(Axis.Dot(Src.Axis[AXIS_Y]) * Src.Length[AXIS_Y]) + 
		abs(Axis.Dot(Src.Axis[AXIS_Z]) * Src.Length[AXIS_Z]);

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Src.Axis[AXIS_X];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Src.Length[AXIS_X];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Src.Axis[AXIS_Y];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Src.Length[AXIS_Y];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Src.Axis[AXIS_Z];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Src.Length[AXIS_Z];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	CubeInfo SrcInfo, DestInfo;
	SrcInfo = ConvertCubeInfo(Src);
	DestInfo = ConvertCubeInfo(Dest);

	ComputeHitPoint(HitPoint, SrcInfo, DestInfo);

	return true;
}

bool CCollisionManager::CollisionSphere3DToCube(Vector3& HitPoint, const Sphere3DInfo& Src, const CubeInfo& Dest)
{
	if ((Dest.Left <= Src.Center.x && Src.Center.x <= Dest.Right) ||
		(Dest.Bottom <= Src.Center.y && Src.Center.y <= Dest.Top) ||
		(Dest.Front <= Src.Center.z && Src.Center.z <= Dest.Back))
	{
		CubeInfo Info = Dest;
		Info.Left -= Src.Radius;
		Info.Bottom -= Src.Radius;
		Info.Right += Src.Radius;
		Info.Top += Src.Radius;
		Info.Front -= Src.Radius;
		Info.Back += Src.Radius;

		if (Info.Left > Src.Center.x)
			return false;

		else if (Info.Bottom > Src.Center.y)
			return false;

		else if (Info.Front > Src.Center.z)
			return false;

		else if (Info.Right < Src.Center.x)
			return false;

		else if (Info.Top < Src.Center.y)
			return false;

		else if (Info.Back < Src.Center.z)
			return false;

		CubeInfo OverlapBox = ConvertCubeInfo(Src);

		ComputeHitPoint(HitPoint, Dest, OverlapBox);

		return true;
	}

	Vector3	Pos[8] =
	{
		Vector3(Dest.Left, Dest.Bottom, Dest.Front),
		Vector3(Dest.Left, Dest.Bottom, Dest.Back),
		Vector3(Dest.Left, Dest.Top, Dest.Front),
		Vector3(Dest.Left, Dest.Top, Dest.Back),
		Vector3(Dest.Right, Dest.Bottom, Dest.Front),
		Vector3(Dest.Right, Dest.Bottom, Dest.Back),
		Vector3(Dest.Right, Dest.Top, Dest.Front),
		Vector3(Dest.Right, Dest.Top, Dest.Back),
	};

	for (int i = 0; i < 8; ++i)
	{
		float Dist = Src.Center.Distance(Pos[i]);

		if (Dist <= Src.Radius)
		{
			CubeInfo OverlapBox = ConvertCubeInfo(Src);

			ComputeHitPoint(HitPoint, Dest, OverlapBox);

			return true;
		}
	}

	return false;
}

bool CCollisionManager::CollisionSphere3DToOBB3D(Vector3& HitPoint, const Sphere3DInfo& Src, const OBB3DInfo& Dest)
{
	Vector3	CenterLine = Dest.Center - Src.Center;

	float CenterProjDist = CenterLine.Length();

	Vector3	Axis = CenterLine;
	Axis.Normalize();

	float SrcDist, DestDist;

	SrcDist = Src.Radius;
	DestDist = abs(Axis.Dot(Dest.Axis[AXIS_X]) * Dest.Length[AXIS_X]) +
		abs(Axis.Dot(Dest.Axis[AXIS_Y]) * Dest.Length[AXIS_Y]) +
		abs(Axis.Dot(Dest.Axis[AXIS_Z]) * Dest.Length[AXIS_Z]);

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS_X];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Dest.Length[AXIS_X];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS_Y];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Dest.Length[AXIS_Y];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	Axis = Dest.Axis[AXIS_Z];

	CenterProjDist = abs(Axis.Dot(CenterLine));

	DestDist = Dest.Length[AXIS_Z];

	if (CenterProjDist > SrcDist + DestDist)
		return false;

	CubeInfo SrcInfo, DestInfo;

	SrcInfo = ConvertCubeInfo(Src);
	DestInfo = ConvertCubeInfo(Dest);

	ComputeHitPoint(HitPoint, SrcInfo, DestInfo);

	return true;
}

bool CCollisionManager::CollisionSphere3DToSphere3D(Vector3& HitPoint, const Sphere3DInfo& Src, const Sphere3DInfo& Dest)
{
	float Dist = Src.Center.Distance(Dest.Center);

	bool result = Dist <= Src.Radius + Dest.Radius;

	HitPoint = (Src.Center + Dest.Center) / 2.f;

	return result;
}

bool CCollisionManager::CollisionRayToSphere(PickingResult& result, const Ray& ray, const Vector3& Center, float Radius)
{
	Vector3	M = ray.Pos - Center;

	float b = 2.f * M.Dot(ray.Dir);
	float c = M.Dot(M) - Radius * Radius;

	float	Det = b * b - 4.f * c;

	if (Det < 0.f)
		return false;

	Det = sqrtf(Det);

	float	t1, t2;

	t1 = (-b + Det) / 2.f;
	t2 = (-b - Det) / 2.f;

	if (t1 < 0.f && t2 < 0.f)
		return false;

	result.Distance = min(t1, t2);

	if (result.Distance < 0.f)
		result.Distance = max(t1, t2);
	result.HitPoint = ray.Pos + ray.Dir * result.Distance;

	return true;
}

Box2DInfo CCollisionManager::ConvertBox2DInfo(const Sphere2DInfo& Info)
{
	Box2DInfo result;

	result.Left = Info.Center.x - Info.Radius;
	result.Bottom = Info.Center.y - Info.Radius;
	result.Right = Info.Center.x + Info.Radius;
	result.Top = Info.Center.y + Info.Radius;

	return result;
}

Box2DInfo CCollisionManager::ConvertBox2DInfo(const OBB2DInfo& Info)
{
	Box2DInfo result;

	Vector2	Pos[4];

	Pos[0] = Info.Center - Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] + Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

	Pos[1] = Info.Center + Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] + Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

	Pos[2] = Info.Center - Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] - Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

	Pos[3] = Info.Center + Info.Axis[AXIS2D_X] * Info.Length[AXIS2D_X] - Info.Axis[AXIS2D_Y] * Info.Length[AXIS2D_Y];

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

CubeInfo CCollisionManager::ConvertCubeInfo(const Sphere3DInfo& Info)
{
	CubeInfo result;

	result.Left = Info.Center.x - Info.Radius;
	result.Bottom = Info.Center.y - Info.Radius;
	result.Front = Info.Center.z - Info.Radius;
	result.Right = Info.Center.x + Info.Radius;
	result.Top = Info.Center.y + Info.Radius;
	result.Back = Info.Center.y + Info.Radius;

	return result;
}

CubeInfo CCollisionManager::ConvertCubeInfo(const OBB3DInfo& Info)
{
	CubeInfo result;

	Vector3	Pos[8];

	Pos[0] = Info.Center - Info.Axis[AXIS_X] * Info.Length[AXIS_X] + Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] + Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	Pos[1] = Info.Center + Info.Axis[AXIS_X] * Info.Length[AXIS_X] - Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] + Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	Pos[2] = Info.Center + Info.Axis[AXIS_X] * Info.Length[AXIS_X] + Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] - Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	Pos[3] = Info.Center - Info.Axis[AXIS_X] * Info.Length[AXIS_X] - Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] + Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	Pos[4] = Info.Center - Info.Axis[AXIS_X] * Info.Length[AXIS_X] + Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] - Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	Pos[5] = Info.Center + Info.Axis[AXIS_X] * Info.Length[AXIS_X] - Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] - Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	Pos[6] = Info.Center + Info.Axis[AXIS_X] * Info.Length[AXIS_X] + Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] + Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	Pos[7] = Info.Center - Info.Axis[AXIS_X] * Info.Length[AXIS_X] - Info.Axis[AXIS_Y] * Info.Length[AXIS_Y] - Info.Axis[AXIS_Z] * Info.Length[AXIS_Z];

	result.Left = Pos[0].x;
	result.Bottom = Pos[0].y;
	result.Front = Pos[0].z;

	result.Right = Pos[0].x;
	result.Top = Pos[0].y;
	result.Back = Pos[0].z;

	for (int i = 1; i < 8; ++i)
	{
		result.Left = result.Left > Pos[i].x ? Pos[i].x : result.Left;
		result.Bottom = result.Bottom > Pos[i].y ? Pos[i].y : result.Bottom;
		result.Front = result.Front > Pos[i].z ? Pos[i].z : result.Front;

		result.Right = result.Right < Pos[i].x ? Pos[i].x : result.Right;
		result.Top = result.Top < Pos[i].y ? Pos[i].y : result.Top;
		result.Back = result.Back < Pos[i].z ? Pos[i].z : result.Back;
	}

	return result;
}

CubeInfo CCollisionManager::OverlapCube(const CubeInfo& Src, const CubeInfo& Dest)
{
	CubeInfo Info;

	Info.Left = Src.Left > Dest.Left ? Src.Left : Dest.Left;
	Info.Bottom = Src.Bottom > Dest.Bottom ? Src.Bottom : Dest.Bottom;
	Info.Right = Src.Right < Dest.Right ? Src.Right : Dest.Right;
	Info.Top = Src.Top < Dest.Top ? Src.Top : Dest.Top;
	Info.Front = Src.Front < Dest.Front ? Src.Front : Dest.Front;
	Info.Back = Src.Back < Dest.Back ? Src.Back : Dest.Back;

	return Info;
}

CubeInfo CCollisionManager::OverlapCube(const CubeInfo& Src, const OBB3DInfo& Dest)
{
	CubeInfo Info, DestInfo;

	DestInfo = ConvertCubeInfo(Dest);

	Info.Left = Src.Left > DestInfo.Left ? Src.Left : DestInfo.Left;
	Info.Bottom = Src.Bottom > DestInfo.Bottom ? Src.Bottom : DestInfo.Bottom;
	Info.Right = Src.Right < DestInfo.Right ? Src.Right : DestInfo.Right;
	Info.Top = Src.Top < DestInfo.Top ? Src.Top : DestInfo.Top;
	Info.Front = Src.Front < DestInfo.Front ? Src.Front : DestInfo.Front;
	Info.Back = Src.Back < DestInfo.Back ? Src.Back : DestInfo.Back;

	return Info;
}

CubeInfo CCollisionManager::OverlapCube(const CubeInfo& Src, const Sphere3DInfo& Dest)
{
	CubeInfo	Info, DestInfo;

	DestInfo = ConvertCubeInfo(Dest);

	Info.Left = Src.Left > DestInfo.Left ? Src.Left : DestInfo.Left;
	Info.Bottom = Src.Bottom > DestInfo.Bottom ? Src.Bottom : DestInfo.Bottom;
	Info.Right = Src.Right < DestInfo.Right ? Src.Right : DestInfo.Right;
	Info.Top = Src.Top < DestInfo.Top ? Src.Top : DestInfo.Top;
	Info.Front = Src.Front < DestInfo.Front ? Src.Front : DestInfo.Front;
	Info.Back = Src.Back < DestInfo.Back ? Src.Back : DestInfo.Back;

	return Info;
}

void CCollisionManager::ComputeHitPoint(Vector3& HitPoint, const CubeInfo& Src, const CubeInfo& Dest)
{
	float Left = Src.Left > Dest.Left ? Src.Left : Dest.Left;
	float Bottom = Src.Bottom > Dest.Bottom ? Src.Bottom : Dest.Bottom;
	float Right = Src.Right < Dest.Right ? Src.Right : Dest.Right;
	float Top = Src.Top < Dest.Top ? Src.Top : Dest.Top;
	float Front = Src.Front < Dest.Front ? Src.Front : Dest.Front;
	float Back = Src.Back < Dest.Back ? Src.Back : Dest.Back;

	HitPoint.x = (Left + Right) / 2.f;
	HitPoint.y = (Top + Bottom) / 2.f;
	HitPoint.z = (Front + Back) / 2.f;
}
