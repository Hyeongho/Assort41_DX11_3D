#pragma once

#include "GameObject/GameObject.h"

enum class EKnobState {
	Normal,
	Broken,
	End
};

class CRoboSponge_Knob :
	public CGameObject
{
	friend class CScene;

protected:
	CRoboSponge_Knob();
	CRoboSponge_Knob(const CRoboSponge_Knob& Obj);
	virtual ~CRoboSponge_Knob();

protected:
	CSharedPtr<class CStaticMeshComponent>		m_Mesh;
	CSharedPtr<class CColliderOBB3D>			m_Collider;

private:
	EKnobState m_KnobState;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRoboSponge_Knob* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void BreakKnob();

private: // Collision
	void Collision_PlayerAttack(const CollisionResult& result);

private :
	void DebugF1();
	void DebugF2();

};

