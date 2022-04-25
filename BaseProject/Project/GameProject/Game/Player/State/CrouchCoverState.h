#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CrouchCoverState : public State<Player> {
private:
	//�J�o�[�̎n�߂̃A�N�V�������I��������ǂ���
	bool is_cover;
	//�E�������Ă��邩���������Ă��邩
	bool is_right;
public:
	CrouchCoverState(Player* owner);

	void Enter();

	void Execute();

	void Move();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* task);
};