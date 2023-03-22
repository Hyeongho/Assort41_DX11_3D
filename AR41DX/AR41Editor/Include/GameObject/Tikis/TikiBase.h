#pragma once

#include "../Monster.h"

class CTikiBase :
	public CMonster
{
	friend class CScene;

protected:
	CTikiBase();
	CTikiBase(const CTikiBase& Obj);
	virtual ~CTikiBase();

protected:
	CSharedPtr<class CColliderOBB3D>			m_Collider;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTikiBase* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected:
	virtual void Tiki_Die(); // 파괴처리, 상속 시 Destroy 처리로 TikiBase의 Tiki_Die 함수 호출은 마지막에.
};

