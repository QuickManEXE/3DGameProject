#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

//弓矢を射るステート
class ArrowAimState : public State<Player> {
public:
	ArrowAimState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* _task);

	void Aim();
};