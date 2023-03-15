#pragma once

#include "GameObject/GameObject.h"

class CBullet :
    public CGameObject
{
	friend class CScene;

protected:
	CBullet();
	CBullet(const CBullet& Obj);
	virtual ~CBullet();

private:
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CColliderCube> m_Body;
	CSharedPtr<class CAnimation> m_Anim;
	float m_Speed;
	float m_Angle;
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBullet* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	void SetAngle(float f);
private:
	void CollisionBullet(const CollisionResult& result);
	void ResetIdle();
};

