#pragma once
#include "GameObject/GameObject.h"

class CPatrickObject : public CGameObject
{
private:
	friend class CScene;
	CSharedPtr<class CStaticMeshComponent> m_Mesh;
	CSharedPtr<class CColliderCube> m_Body;
	CSharedPtr<class CRigidBody> m_Rigid;
	float m_Speed;
	void CollisionObject(const CollisionResult& result);
protected:
	CPatrickObject();
	CPatrickObject(const CPatrickObject& obj);
	virtual ~CPatrickObject();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPatrickObject* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	void SetAngle(float f);
};

