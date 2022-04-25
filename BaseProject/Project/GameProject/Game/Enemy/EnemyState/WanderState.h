#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

class WanderState : public State<Enemy> {

	std::list<Node*> route;

	CVector3D next_pos;
public:
	WanderState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();

	void Move();

	void Render();

	void CollisionCheck(CollisionTask* task);
};