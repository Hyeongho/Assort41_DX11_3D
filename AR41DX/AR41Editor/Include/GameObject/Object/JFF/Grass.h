#pragma once

#include "GameObject\GameObject.h"

class CGrass :
	public CGameObject
{
	friend class CScene;

protected:
	CGrass();
	CGrass(const CGrass& Obj);
	virtual ~CGrass();

private:
	CSharedPtr<class CStaticMeshComponent>		m_Mesh;

private:
	// Grass ����� ������
	bool		m_RotLeft;
	bool		m_RotRight;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CGrass* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

