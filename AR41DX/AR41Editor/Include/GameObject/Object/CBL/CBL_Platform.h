#pragma once

#include "GameObject\GameObject.h"

enum class ERollDir {
	Left,
	Right,
	Front,
	Back
};

class CCBL_Platform
	: public CGameObject
{
	friend class CScene;

protected:
	CCBL_Platform();
	CCBL_Platform(const CCBL_Platform& Obj);
	virtual ~CCBL_Platform();

protected:
	CSharedPtr<class CStaticMeshComponent>	m_Mesh;
	CSharedPtr<class CColliderOBB3D>		m_Collider;

private :
	bool		m_Roll;
	ERollDir	m_RollDir;
	float		m_TotalRollDegree;
	int			m_RollCount;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CCBL_Platform* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private: // Collision
	void Collision_Rolling(const CollisionResult& result);

public :
	void SetLookBoss(const Vector3& BossPos); // 보스를 바라보도록(Z축이 보스와 직교하도록) 설정
	void Roll(const ERollDir& RollDir);
	bool GetRoll() const { return m_Roll; }

private :
	void DebugF1();
	void DebugF2();
	void DebugF3();
	void DebugF4();
};

