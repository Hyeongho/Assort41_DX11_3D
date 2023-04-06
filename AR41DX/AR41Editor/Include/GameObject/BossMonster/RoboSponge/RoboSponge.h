#pragma once

#include "../BossMonster.h"
#include "RoboSponge_Knob.h"

enum class ERS_Pattern {
	AttackVertic,
	AttackHoriz,
	AttackWords,
	End
};

enum class ELRCheck {
	Left,
	Right,
	End
};

class CRoboSponge :
	public CBossMonster
{
	friend class CScene;

protected:
	CRoboSponge();
	CRoboSponge(const CRoboSponge& Obj);
	virtual ~CRoboSponge();

private:
	bool			m_ActionStart;
	
	// Move ����
	Vector3			m_MapCenterPoint;

	bool			m_ChasePlayer;		// Player�� �ٶ󺸴� �׼��� Ʈ����
	bool			m_MoveToCenter;		// Map �߾����� ���ư��� �׼��� Ʈ����
	bool			m_MoveToPlatform;	// Target Platform���� ���ϴ� �׼��� Ʈ����
	std::string		m_TargetPlatformName; // �پ��� �׼��� ������� �����Ǵ� Ÿ�� �÷����� ����
	int				m_TargetPlatformIdx; // �پ��� �׼��� ������� �����Ǵ� Ÿ�� �÷����� ����

	// Attack ����
	ELRCheck		m_LR; // ���� �������� �÷��̾ ��/�쿡 �ִ����� �Ǵ��Ͽ� ����
	float			m_CenterWaitTimer;
	bool			m_CenterWait;
	
	ERS_Pattern		m_NowPattern;


	// AttackHoriz ����
	bool			m_AttackHorizEnd;
	float			m_AttackHorizEndTimer;

	// AttackVertic ����
	int				m_VerticAttackMax;

	// AtackWords ����
	bool			m_AttackWords;
	int				m_AttackWordsCount; // KAE RAH TAE �� � ������ �ؾ��ϴ����� �˻��ϱ� ���� Int

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRoboSponge* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

public:
	void ActionStart();

private:
	void AttackWords();			// AttackWords �ൿ ����
	void AttackVertric();		// AttackVertic �ൿ ����
	void AttackVerticEnd();		// AttackVertic�� ���� ��, ������ ���� ���� Ȥ�� ���ư��⸦ ����
	void AttackHoriz();			// AttackHoriz �ൿ ����
	void AttackHorizEnd();		// AttackHoriz�� ���� ��, �÷��̾��� ������ ���� ������ ��� �ൿ�� Ʈ���� ������ True��
	void MoveToPlatform();		// Ÿ�� �÷������� �̵��ϴ� �ൿ�� Ʈ���� ������ True��
	void MoveToCenter();		// Center�� �̵��ϴ� �ൿ�� Ʈ���� ������ True��
	void ChasePlayer();			// �÷��̾ �����ϴ� �ൿ�� Ʈ���� ������ True��
	void RollTargetPlatform();	// Ÿ�� �÷���(m_TargetPlatformName)�� ������. �� ��, �¿�θ� ������.
	void FindTargetPlatform();	// Ÿ�� �÷���(m_TargetPlatformName)�� Ž���Ѵ�. Ÿ�� �÷����� �÷��̾�� ���� ����� �÷����̴�. 
	void LookPlayer();			// �÷��̾ �ٶ󺸰� �Ѵ�.
	void CountCenterWait();		// �׼� �� ���� �׼Ǳ��� ����Ű�� �Լ�.
	void RobotDeath();

	void PlayPattern(const ERS_Pattern& Pattern);	// ���� ����

public:
	void Damaged();
	void Groggy();
	void SetMapCenter(const Vector3& MapCenterPoint) { m_MapCenterPoint = MapCenterPoint; }
	void SetMapCenter(float x, float y, float z) { SetMapCenter(Vector3(x, y, z)); }
	void SetMapCenter(const Vector2& MapCenterPoint) { SetMapCenter(Vector3(MapCenterPoint.x, 0.f, MapCenterPoint.y)); }
	void SetMapCenter(float x, float y) { SetMapCenter(Vector2(x, y)); }

	bool GetBossAction() { return m_ActionStart; }

private : // Animation
	void CreateAnim();
	void ChangeAnim_AttackHorizL();
	void ChangeAnim_AttackHorizLPose();
	void ChangeAnim_AttackHorizR();
	void ChangeAnim_AttackHorizRPose();
	void ChangeAnim_AttackVerticLLoop();
	void ChangeAnim_AttackVerticLStart();
	void ChangeAnim_AttackVerticRLoop();
	void ChangeAnim_AttackVerticRStart();
	void ChangeAnim_AttackWordsLoop();
	void ChangeAnim_AttackWordsStart();
	void ChangeAnim_AttackWordsConnect();
	void ChangeAnim_AttackWordsConnect2();
	void ChangeAnim_Hit1();
	void ChangeAnim_Hit2();
	void ChangeAnim_Hit2Pose();
	void ChangeAnim_Idle();
	void ChangeAnim_IdleReverse();
	void ChangeAnim_VerticLHold();
	void ChangeAnim_VerticRHold();
	void ChangeAnim_Victory();
	void ChangeAnim_Death();

private:
	void SetHp1();
	void SetHp4(); 


};

