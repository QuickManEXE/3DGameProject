#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class PlayerDamageState : public State<Player> {
public:
	PlayerDamageState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* _task);
};