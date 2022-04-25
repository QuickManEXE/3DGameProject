#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

//�|����˂�X�e�[�g
class ArrowAimState : public State<Player> {
public:
	ArrowAimState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* _task);

	void Aim();
};