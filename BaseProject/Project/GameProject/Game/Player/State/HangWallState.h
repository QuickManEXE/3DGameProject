#pragma once
#include"../../../StateMachine/State.h"
#include"../Player.h"

//�ǂ�͂�
class HangWallState : public State<Player> {
public:
	HangWallState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* task);
};