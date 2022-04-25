#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

//
//敵と交戦して間合いや攻撃に移行する中間ステート
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