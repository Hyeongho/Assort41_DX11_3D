#pragma once

#include "GameObject/GameObject.h"

class CPlayer :
    public CGameObject
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& Obj);
	virtual ~CPlayer();

private:
	CSharedPtr<class CAnimationMeshComponent>	m_Mesh;
	CSharedPtr<class CCameraComponent>	m_Camera;
	CSharedPtr<class CTargetArm>		m_Arm;
	CSharedPtr<class CAnimation>		m_Animation;
	CSharedPtr<class CRigidBody>	m_Rigid;
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CPlayer* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void MoveFront();
	void MoveBack();
	void MoveLeft();
	void MoveRight();
	void Jump();
	void AttackKey();
	void CameraRotationKey();
};

