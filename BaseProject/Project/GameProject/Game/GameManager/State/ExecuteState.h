#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"


class ExecuteState : public State<GameManager>{
public:
	ExecuteState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void SetEntrance();
	void CollisionCheck(CollisionTask* _task);
};