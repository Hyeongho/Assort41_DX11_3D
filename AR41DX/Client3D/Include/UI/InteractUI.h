#pragma once

#include "UI/UIWindow.h"

enum class EInteractTarget {
	Npc,			// NPC
	Sponge_House,	// �������� ��
	Squid_House,	// ¡¡�� ��
	Patric_House,	// ���� ��
	Door,			// ��Ÿ ��
	Gate,			// ���ܱ�
	Teleport_Box,	// �����̵� �ڽ�
	Bus_Stop,		// ���� ������
	End
};

class CInteractUI :
	public CUIWindow
{
	friend class CSceneViewport;
	friend class CScene;

protected:
	CInteractUI();
	CInteractUI(const CInteractUI& Window);
	virtual ~CInteractUI();

protected:
	std::unordered_map<std::string, CSharedPtr<CUIWidget>> m_mapInteractUI;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CInteractUI* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private :
	void CreaeteAllUI();
	void CreateInteractUI();

public :
	void ActiveInteractUI();
	void InActiveInteractUI();

public :
	void SetTarget(EInteractTarget Target);

private :
	void KeyF1();
	void KeyF2();
	void KeyF3();
	void KeyF4();
	void KeyF5();
};