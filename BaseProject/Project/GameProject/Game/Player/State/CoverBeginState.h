#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CoverBeginState : public State<Player> {
public:
	CoverBeginState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);
};