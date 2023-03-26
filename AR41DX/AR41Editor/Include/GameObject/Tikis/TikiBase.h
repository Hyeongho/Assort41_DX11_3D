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
	CSharedPtr<class CColliderOBB3D>			m_ColliderBottom;
	CSharedPtr<class CColliderOBB3D>			m_ColliderTop;

private :
	bool		m_OnTiki;
	bool		m_SetGravity;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CTikiBase* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public :
	void SetOnGravity(bool Gravity = true) { m_SetGravity = true; }

protected:
	virtual void Tiki_Die(); // �ı�ó��, ��� �� Destroy ó���� TikiBase�� Tiki_Die �Լ� ȣ���� ��������.

protected:
	void Collision_Tikis(const CollisionResult& result);
	void Release_Tikis(const CollisionResult& result);

private: // Debug key
	void DebugKeyF1();
	void DebugKeyF2();
};

