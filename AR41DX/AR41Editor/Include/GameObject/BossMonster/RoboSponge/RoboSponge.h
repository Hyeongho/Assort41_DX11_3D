#pragma once

#include "../BossMonster.h"

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
	// Move 관련
	bool			m_MoveToPlatForm;
	Vector3			m_TargetPlatformPos;


	// Attack 관련
	ELRCheck		m_LR; // 현재 기준으로 플레이어가 좌/우에 있는지를 판단하여 공격
	std::string		m_TargetPlatformName; // 공격 타겟인 플랫폼의 이름을 저장



	// AtackWords 관련
	int			m_AttackWordsCount; // KAE RAH TAE 중 어떤 공격을 해야하는지를 검사하기 위한 Int

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRoboSponge* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private:
	void AttackWords();
	void AttackVertric();
	void AttackHoriz();
	void MoveToPlayer();
	void MoveToCenter();

public:
	void Damaged();
	void Groggy();

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
	void ChangeAnim_Hit1();
	void ChangeAnim_Hit2();
	void ChangeAnim_Hit2Pose();
	void ChangeAnim_Idle();
	void ChangeAnim_VerticLHold();
	void ChangeAnim_VerticRHold();
	void ChangeAnim_Victory();


};

