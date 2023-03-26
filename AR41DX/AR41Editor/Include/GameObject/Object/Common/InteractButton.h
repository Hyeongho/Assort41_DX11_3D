#pragma once

#include "GameObject\GameObject.h"

enum class EButtonDir {
	Virtical,
	Horizon,
	End
};

enum class EInteractObjectList {
	BusStop,
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
	EButtonDir				m_ButtonDir;
	EInteractObjectList		m_InterObjList;
	std::string				m_InterObjName;
	bool					m_IsPushed;

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
	void SetInteractObject(const EInteractObjectList& Object, const std::string& ObjName); // 인터랙트 대상 오브젝트 설정.
	void SetDir(const EButtonDir& Dir);	// 수직/수평 설정
};
