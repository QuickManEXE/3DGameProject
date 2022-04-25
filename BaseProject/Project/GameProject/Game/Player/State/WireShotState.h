#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class WireShotState : public State<Player> {
public:
	WireShotState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* task);

};