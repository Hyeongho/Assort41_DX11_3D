#pragma once

#include "GameObject\GameObject.h"

class CTaxiDriver
	: public CGameObject
{
	friend class CScene;

protected:
	CTaxiDriver();
	CTaxiDriver(const CTaxiDriver& Obj);
	virtual ~CTaxiDriver();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTaxiDriver* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
};

