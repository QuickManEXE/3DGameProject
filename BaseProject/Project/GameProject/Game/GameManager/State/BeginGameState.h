#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"

class BeginGameState : public State<GameManager> {
public:
	BeginGameState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);

};