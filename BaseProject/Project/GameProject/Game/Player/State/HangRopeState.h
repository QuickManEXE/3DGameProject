#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class HangRopeState : public State<Player> {

public:
	HangRopeState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void RopeIdle();

	void ReleaseRope();

	void Render();
};