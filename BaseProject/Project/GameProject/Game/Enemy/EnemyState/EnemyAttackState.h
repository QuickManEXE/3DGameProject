#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

class EnemyAttackState : public State<Enemy> {
private:
	float attack_cooldown;
public:
	EnemyAttackState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};