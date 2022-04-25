#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

//�ǂ�o��X�e�[�g
class ClimeState : public State<Player> {
public:
	ClimeState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();
};