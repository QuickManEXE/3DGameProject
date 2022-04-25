#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class WireActionState : public State<Player> {
private:
	float t;
public:
	WireActionState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* task);

};