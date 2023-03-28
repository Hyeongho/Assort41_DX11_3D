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
	virtual void Tiki_Die(); // �ı�ó��, ��� �� Destroy ó���� TikiBase�� Tiki_Die �Լ� ȣ���� ��������.
};

