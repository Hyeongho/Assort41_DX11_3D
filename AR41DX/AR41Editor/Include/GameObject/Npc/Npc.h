#pragma once

#include "GameObject\GameObject.h"


class CNpc
	: public CGameObject
{
	friend class CScene;

protected:
	CNpc();
	CNpc(const CNpc& Obj);
	virtual ~CNpc();

protected:
	CSharedPtr<class CAnimationMeshComponent>	m_AnimMesh;
	CSharedPtr<class CAnimation>				m_Animation;

	CSharedPtr<class CStaticMeshComponent>		m_StaticMesh;
	
protected :
	int			m_DialogCount;
	ENpcList	m_NpcType;
	EMapList	m_NpcMapPos;
	bool		m_EnableDialog;
	MeshType	m_NpcMeshType;

public:
	void SetMapPos(EMapList Map) { m_NpcMapPos = Map; }

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CNpc* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	virtual void ChangeAnimByName(const std::string& Name);
	virtual void StartDialog();
};

