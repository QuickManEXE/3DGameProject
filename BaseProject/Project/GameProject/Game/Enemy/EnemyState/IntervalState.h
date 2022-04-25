#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

//
//�v���C���[�̋߂��m�[�h�܂ňړ�����X�e�[�g
//

class IntervalState : public State<Enemy> {
private:
	std::list<Node*> m_route;

	CVector3D m_next_pos;
public:
	IntervalState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};