#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"


class InitGameState : public State<GameManager>{
public:
	InitGameState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();
};