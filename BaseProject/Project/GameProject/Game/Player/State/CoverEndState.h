#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CoverEndState : public State<Player> {
public:
	CoverEndState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);
};