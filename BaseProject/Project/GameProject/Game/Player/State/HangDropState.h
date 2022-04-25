#pragma once
#include"../../../StateMachine/State.h"
#include"../Player.h"

//•Ç‚ð’Í‚Þ
class HangDropState : public State<Player> {
public:
	HangDropState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);

};