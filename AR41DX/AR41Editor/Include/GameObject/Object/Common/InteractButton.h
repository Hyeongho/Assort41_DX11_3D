#pragma once

#include "GameObject\GameObject.h"

enum class ButtonDir {
	Virtical,
	Horizon,
	End
};

class CInteractButton
	: public CGameObject
{
	friend class CScene;

protected:
	CInteractButton();
	CInteractButton(const CInteractButton& Obj);
	virtual ~CInteractButton();

protected:
	CSharedPtr<class CStaticMeshComponent>		m_MeshButton;
	CSharedPtr<class CStaticMeshComponent>		m_MeshButtonPlate;
	CSharedPtr<class CColliderOBB3D>			m_Collider;

private:
	ButtonDir	m_ButtonDir;
	bool		m_IsPushed;

	void		(*m_Func)();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CInteractButton* Clone()    const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

protected: // Collision
	void Collision_Attacked(const CollisionResult& result);

public :
	template <typename T>
	void SetInteractFunc(void(T::* Func)()) {
		m_Func = Func;
	}
};
