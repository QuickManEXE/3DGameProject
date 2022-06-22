#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"


class EndGameState : public State<GameManager>{
public:
	EndGameState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();
};