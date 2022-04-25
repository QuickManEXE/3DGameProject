#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

//壁を登るステート
class ClimeState : public State<Player> {
public:
	ClimeState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();
};