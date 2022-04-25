#pragma once
#include"../../../StateMachine/State.h"
#include"../Player.h"

//•Ç‚ð’Í‚Þ
class GodState : public State<Player> {
public:
	GodState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Move();

	//void Render();

	void CollisionCheck(CollisionTask* task);

};