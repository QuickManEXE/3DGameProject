#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class IdleState : public State<Player> {
private:
	//�����Ă��邩�ǂ���
	bool is_run;
	//���s�ړ����Ă��邩�ǂ���
	bool is_hold;
	//�L�����N�^�[�̉�]�̕���
	CVector3D rot_target;
public:
	IdleState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Move();

	void Render();

	void CollisionCheck(CollisionTask* _task);

	
};
