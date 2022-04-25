#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../FireElemental.h"

class EnemyIdleState : public State<FireElemental> {
private:
	//UŒ‚‚Ì”ÍˆÍ
	float attack_range;
public:
	EnemyIdleState(FireElemental* owner);

	void Enter();

	void Execute();

	void Exit();

	void Thinking();
};