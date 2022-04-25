#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../FireElemental.h"

class EnemyMoveState : public State<FireElemental> {
private:
	//UŒ‚‚Ì”ÍˆÍ
	float attack_range;

	//‰ñ“]‚Ì–Ú•W’l
	CVector3D rot_target;
public:
	EnemyMoveState(FireElemental* owner);

	void Enter();

	void Execute();

	void Exit();

	void Move();

};