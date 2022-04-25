#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class StayState : public State<Player> {
public:
	StayState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* _task);

};