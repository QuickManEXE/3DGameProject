#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

//
//�G�ƌ�킵�Ċԍ�����U���Ɉڍs���钆�ԃX�e�[�g
//

class BattleIdleState : public State<Enemy> {
private:

	float m_active_time;

public:
	BattleIdleState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};