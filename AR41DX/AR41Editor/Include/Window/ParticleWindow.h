#pragma once
#include "Editor\EditorWindow.h"
#include "Editor/EditorTree.h"

class CParticleWindow : public CEditorWindow
{
private:
	friend class CEditorGUIManager;
//������Ʈ ����
	class CEditorInput* m_SpawnTime;
	class CEditorInput* m_SpawnCountMax;
//��ƼŬ ����
	class CEditorInput* m_Name;	//��ƼŬ �̸�
//��ƼŬ ���� ����
	class CEditorCheckBox* m_ParticleSpawnEnable;
	class CEditorCheckBox* m_ParticleMoveEnable;
	class CEditorCheckBox* m_ParticleGravity;
	class CEditorCheckBox* m_ParticleMoveDirEnable;
	class CEditorInput* m_ParticleLifeTimeMin;
	class CEditorInput* m_ParticleLifeTimeMax;
	class CEditorInput* m_ParticleStartMin[3];
	class CEditorInput* m_ParticleStartMax[3];
	class CEditorInput* m_ParticleScaleMin[3];
	class CEditorInput* m_ParticleScaleMax[3];
	class CEditorInput* m_ParticleMoveDir[3];
	class CEditorInput* m_ParticleMoveAngle[3];
	class CEditorInput* m_ParticleColorMin[4];
	class CEditorInput* m_ParticleColorMax[4];
	CSharedPtr<class CParticleComponent>	m_SelectComponent;
	CSharedPtr<class CParticle>	m_SelectParticle;
//��ƼŬ ����Ʈ
	class CEditorListBox* m_ParticleList;
	std::string			m_SelectListItem;
	void ListClickCallback(int index, const std::string& item);
	void ParticleChangeCallback();
	void ParticleLoadCallback();
protected:
	CParticleWindow();
	virtual ~CParticleWindow();
public:
	void SetSelectComponent(class CParticleComponent* component);
	virtual bool Init();
	virtual void Update(float deltaTime);
};

