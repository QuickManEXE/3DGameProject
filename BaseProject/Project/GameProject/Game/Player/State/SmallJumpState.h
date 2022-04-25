#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class SmallJumpState : public State<Player> {

public:
	SmallJumpState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);

};