#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class WireAimState : public State<Player> {

public:
	WireAimState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* task);

	void Move();

	void AimWire();
};