#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class PlayerDeathState : public State<Player> {
private:
	//�^�C�g���܂ł̎���
	float m_time;

public:
	PlayerDeathState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void CollisionCheck(CollisionTask* _task);
};