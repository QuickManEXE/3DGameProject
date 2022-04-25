#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

class EnemyHangWallState : public State<Enemy> {
private:
	float t;
public:
	EnemyHangWallState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};