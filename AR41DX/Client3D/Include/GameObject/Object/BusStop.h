#pragma once

#include "GameObject\GameObject.h"

class CBusStop
	: public CGameObject
{
	friend class CScene;

protected:
	CBusStop();
	CBusStop(const CBusStop& Obj);
	virtual ~CBusStop();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderCube>			m_Cube;

private:
	bool	m_BusExist;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CBusStop* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	void CallBus();
};

