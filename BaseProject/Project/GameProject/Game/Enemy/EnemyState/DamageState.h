#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

class DamageState : public State<Enemy> {
public:
	DamageState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};