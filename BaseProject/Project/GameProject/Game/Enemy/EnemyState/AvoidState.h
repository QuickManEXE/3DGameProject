#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

//
//��������X�e�[�g
//

class AvoidState : public State<Enemy> {
private:
	//�E�����ɉ�����邽�߂̕ϐ�
	int is_right;

public:
	AvoidState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};