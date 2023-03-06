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
	Vector3 m_Dir;
	float m_Speed;
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBullet* Clone()    const;
	void SetDir(const Vector3& vec);
private:
	void CollisionBullet(const CollisionResult& result);
	void ResetIdle();	//아이들상태로 되돌리는 함수
};

