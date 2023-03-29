#pragma once

#include "../BossMonster.h"

class CRoboSponge :
	public CBossMonster
{
	friend class CScene;

protected:
	CRoboSponge();
	CRoboSponge(const CRoboSponge& Obj);
	virtual ~CRoboSponge();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CRoboSponge* Clone() const;
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);

private : // Animation
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

