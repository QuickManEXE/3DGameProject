#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CoverState : public State<Player> {
	//右を向いているか左を向いているか
	bool is_right;
public:
	CoverState(Player* owner);

	void Enter();

	void Execute();

	void Move();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);
};