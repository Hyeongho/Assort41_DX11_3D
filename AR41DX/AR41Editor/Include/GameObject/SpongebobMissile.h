#pragma once
#include "GameObject/GameObject.h"

class CSpongebobMissile : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CColliderOBB3D> m_Body;
	CSharedPtr<class CTargetArm> m_Arm;
	CSharedPtr<class CCameraComponent> m_Camera;
	CSharedPtr<class CBillboardComponent> m_Crosshair;
	CSharedPtr<class CAnimation> m_Anim;
	float m_Speed;
	float m_Angle;
	void CollisionOn(const CollisionResult& result);
protected:
	CSpongebobMissile();
	CSpongebobMissile(const CSpongebobMissile& obj);
	virtual ~CSpongebobMissile();
public:
	virtual void Destroy();
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CSpongebobMissile* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	void SetAngle(float f);
	void ResetIdle();
};

