#pragma once

#include "GameObject/GameObject.h"

class CCannon :
    public CGameObject
{
	friend class CScene;

protected:
	CCannon();
	CCannon(const CCannon& Obj);
	virtual ~CCannon();

private:
	CSharedPtr<class CAnimationMeshComponent> m_Mesh;
	CSharedPtr<class CAnimation> m_Animation;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCannon* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void Fire();
};

