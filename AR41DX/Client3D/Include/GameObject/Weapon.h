#pragma once

#include "GameObject\GameObject.h"

class CWeapon :
    public CGameObject
{
	friend class CScene;

protected:
	CWeapon();
	CWeapon(const CWeapon& Obj);
	virtual ~CWeapon();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CAnimation> m_Anim;
	float m_Angle;
public:
	void SetAngle(float f)
	{
		m_Angle = f;
	}
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CWeapon* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
	bool SetMesh(const std::string& Name);
	void Lasso(const std::string& Name);
	void StartLassoAtk();
	void ResetIdle();
};

