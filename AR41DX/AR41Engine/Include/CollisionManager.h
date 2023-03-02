#pragma once

#include "EngineInfo.h"

class CCollisionManager
{
private:
	std::unordered_map<std::string, CollisionProfile*> m_mapProfile;
	std::vector<CollisionChannel*>	m_vecChannel;

public:
	bool Init();

	bool CreateProfile(const std::string& Name, const std::string& ChannelName, bool Enable, ECollision_Interaction BaseInteraction = ECollision_Interaction::Collision);
	bool SetCollisionInteraction(const std::string& Name, const std::string& ChannelName, ECollision_Interaction Interaction);
	bool CreateChannel(const std::string& Name, ECollision_Interaction Interaction = ECollision_Interaction::Collision);
	CollisionProfile* FindProfile(const std::string& Name);


public:
	bool CollisionBox2DToBox2D(Vector2& HitPoint, class CColliderBox2D* Src, class CColliderBox2D* Dest);
	bool CollisionSphere2DToSphere2D(Vector2& HitPoint, class CColliderSphere2D* Src, class CColliderSphere2D* Dest);
	bool CollisionOBB2DToOBB2D(Vector2& HitPoint, class CColliderOBB2D* Src, class CColliderOBB2D* Dest);
	bool CollisionBox2DToSphere2D(Vector2& HitPoint, class CColliderBox2D* Src, class CColliderSphere2D* Dest);
	bool CollisionBox2DToOBB2D(Vector2& HitPoint, class CColliderBox2D* Src, class CColliderOBB2D* Dest);
	bool CollisionSphere2DToOBB2D(Vector2& HitPoint, class CColliderSphere2D* Src, class CColliderOBB2D* Dest);
	bool CollisionBox2DToPixel(Vector2& HitPoint, class CColliderBox2D* Src, class CColliderPixel* Dest);
	bool CollisionSphere2DToPixel(Vector2& HitPoint, class CColliderSphere2D* Src, class CColliderPixel* Dest);
	bool CollisionOBB2DToPixel(Vector2& HitPoint, class CColliderOBB2D* Src, class CColliderPixel* Dest);
	bool CollisionPixelToPixel(Vector2& HitPoint, class CColliderPixel* Src, class CColliderPixel* Dest);


	bool CollisionPointToBox2D(Vector2& HitPoint, const Vector2& Src, class CColliderBox2D* Dest);
	bool CollisionPointToSphere2D(Vector2& HitPoint, const Vector2& Src, class CColliderSphere2D* Dest);
	bool CollisionPointToOBB2D(Vector2& HitPoint, const Vector2& Src, class CColliderOBB2D* Dest);
	bool CollisionPointToPixel(Vector2& HitPoint, const Vector2& Src, class CColliderPixel* Dest);

public:
	bool CollisionBox2DToBox2D(Vector2& HitPoint, const Box2DInfo& Src, const Box2DInfo& Dest);
	bool CollisionSphere2DToSphere2D(Vector2& HitPoint, const Sphere2DInfo& Src, const Sphere2DInfo& Dest);
	bool CollisionOBB2DToOBB2D(Vector2& HitPoint, const OBB2DInfo& Src, const OBB2DInfo& Dest);
	bool CollisionBox2DToSphere2D(Vector2& HitPoint, const Box2DInfo& Src, const Sphere2DInfo& Dest);
	bool CollisionBox2DToOBB2D(Vector2& HitPoint, const Box2DInfo& Src, const OBB2DInfo& Dest);
	bool CollisionSphere2DToOBB2D(Vector2& HitPoint, const Sphere2DInfo& Src, const OBB2DInfo& Dest);
	bool CollisionBox2DToPixel(Vector2& HitPoint, const Box2DInfo& Src, const PixelInfo& Dest);
	bool CollisionSphere2DToPixel(Vector2& HitPoint, const Sphere2DInfo& Src, const PixelInfo& Dest);
	bool CollisionOBB2DToPixel(Vector2& HitPoint, const OBB2DInfo& Src, const PixelInfo& Dest);
	bool CollisionPixelToPixel(Vector2& HitPoint, const PixelInfo& Src, const PixelInfo& Dest);

	bool CollisionPointToBox2D(Vector2& HitPoint, const Vector2& Src, const Box2DInfo& Dest);
	bool CollisionPointToSphere2D(Vector2& HitPoint, const Vector2& Src, const Sphere2DInfo& Dest);
	bool CollisionPointToOBB2D(Vector2& HitPoint, const Vector2& Src, const OBB2DInfo& Dest);
	bool CollisionPointToPixel(Vector2& HitPoint, const Vector2& Src, const PixelInfo& Dest);

public:
	bool CollisionCubeToCube(Vector3& HitPoint, class CColliderCube* Src, class CColliderCube* Dest);
	bool CollisionCubeToOBB3D(Vector3& HitPoint, class CColliderCube* Src, class CColliderOBB3D* Dest);

	bool CollisionOBB3DToOBB3D(Vector3& HitPoint, class CColliderOBB3D* Src, class CColliderOBB3D* Dest);
	bool CollisionOBB3DToCube(Vector3& HitPoint, class CColliderOBB3D* Src, class CColliderCube* Dest);

	bool CollisionCubeToCube(Vector3& HitPoint, const CubeInfo& Src, const CubeInfo& Dest);
	bool CollisionCubeToOBB3D(Vector3& HitPoint, const CubeInfo& Src, const OBB3DInfo& Dest);

	bool CollisionOBB3DToOBB3D(Vector3& HitPoint, const OBB3DInfo& Src, const OBB3DInfo& Dest);
	bool CollisionOBB3DToCube(Vector3& HitPoint, const OBB3DInfo& Src, const CubeInfo& Dest);

	bool CollisionRayToSphere(PickingResult& result, const Ray& ray, const Vector3& Center, float Radius);

private:
	Box2DInfo ConvertBox2DInfo(const Sphere2DInfo& Info);
	Box2DInfo ConvertBox2DInfo(const OBB2DInfo& Info);
	Box2DInfo OverlapBox2D(const Box2DInfo& Src, const Box2DInfo& Dest);
	Box2DInfo OverlapBox2D(const Box2DInfo& Src, const Sphere2DInfo& Dest);
	Box2DInfo OverlapBox2D(const Box2DInfo& Src, const OBB2DInfo& Dest);
	void ComputeHitPoint(Vector2& HitPoint, const Box2DInfo& Src, const Box2DInfo& Dest);

	CubeInfo ConvertCubeInfo(const OBB3DInfo& Info);
	CubeInfo OverlapCube(const CubeInfo& Src, const CubeInfo& Dest);
	CubeInfo OverlapCube(const CubeInfo& Src, const OBB3DInfo& Dest);
	void ComputeHitPoint(Vector3& HitPoint, const CubeInfo& Src, const CubeInfo& Dest);

	DECLARE_SINGLE(CCollisionManager)
};

