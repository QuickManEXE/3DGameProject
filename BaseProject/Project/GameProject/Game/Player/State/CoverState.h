#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CoverState : public State<Player> {
	//�E�������Ă��邩���������Ă��邩
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