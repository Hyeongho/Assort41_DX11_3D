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
	
	// Move 관련
	Vector3			m_MapCenterPoint;

	bool			m_ChasePlayer;		// Player를 바라보는 액션의 트리거
	bool			m_MoveToCenter;		// Map 중앙으로 돌아가는 액션의 트리거
	bool			m_MoveToPlatform;	// Target Platform으로 향하는 액션의 트리거
	std::string		m_TargetPlatformName; // 다양한 액션의 대상으로 지정되는 타겟 플랫폼을 선택
	int				m_TargetPlatformIdx; // 다양한 액션의 대상으로 지정되는 타겟 플랫폼을 선택

	// Attack 관련
	ELRCheck		m_LR; // 현재 기준으로 플레이어가 좌/우에 있는지를 판단하여 공격
	float			m_CenterWaitTimer;
	bool			m_CenterWait;
	
	ERS_Pattern		m_NowPattern;


	// AttackHoriz 관련
	bool			m_AttackHorizEnd;
	float			m_AttackHorizEndTimer;

	// AttackVertic 관련
	int				m_VerticAttackMax;

	// AtackWords 관련
	bool			m_AttackWords;
	int				m_AttackWordsCount; // KAE RAH TAE 중 어떤 공격을 해야하는지를 검사하기 위한 Int

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
	void AttackWords();			// AttackWords 행동 실행
	void AttackVertric();		// AttackVertic 행동 실행
	void AttackVerticEnd();		// AttackVertic을 끝낸 후, 보스의 연속 공격 혹은 돌아가기를 결정
	void AttackHoriz();			// AttackHoriz 행동 실행
	void AttackHorizEnd();		// AttackHoriz를 끝낸 후, 플레이어의 공격을 위한 보스의 대기 행동의 트리거 변수를 True로
	void MoveToPlatform();		// 타겟 플랫폼으로 이동하는 행동의 트리거 변수를 True로
	void MoveToCenter();		// Center로 이동하는 행동의 트리거 변수를 True로
	void ChasePlayer();			// 플레이어를 추적하는 행동의 트리거 변수를 True로
	void RollTargetPlatform();	// 타겟 플랫폼(m_TargetPlatformName)을 돌린다. 이 때, 좌우로만 돌린다.
	void FindTargetPlatform();	// 타겟 플랫폼(m_TargetPlatformName)을 탐지한다. 타겟 플랫폼은 플레이어와 가장 가까운 플랫폼이다. 
	void LookPlayer();			// 플레이어를 바라보게 한다.
	void CountCenterWait();		// 액션 후 다음 액션까지 대기시키는 함수.
	void RobotDeath();

	void PlayPattern(const ERS_Pattern& Pattern);	// 패턴 실행

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

