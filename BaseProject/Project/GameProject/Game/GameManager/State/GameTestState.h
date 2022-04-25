#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"

class GameTestState : public State<GameManager> {
public:
	GameTestState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

};