#pragma once

#include "GameObject\GameObject.h"

class CPool :
    public CGameObject
{
	friend class CScene;

protected:
	CPool();
	CPool(const CPool& Obj);
	virtual ~CPool();

private:
	CSharedPtr<class CColliderCube> m_Cube;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPool* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

