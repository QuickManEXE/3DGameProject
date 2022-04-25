#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

//
//敵を追いかけるステート
//

class ChaseState : public State<Enemy> {
private:
	float m_search_lost_time;


public:
	ChaseState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};