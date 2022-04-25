#pragma once
#include"../../../StateMachine/State.h"
#include"../Player.h"

//�ǂ�͂�
class HangDropState : public State<Player> {
public:
	HangDropState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);

};