#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class SwingJumpState : public State<Player> {

public:
	SwingJumpState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);

};