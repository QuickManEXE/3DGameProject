#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

class FallState : public State<Enemy> {
private:

	bool is_fall;

public:
	FallState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* task);
};