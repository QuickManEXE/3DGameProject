#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

//
//回避を取るステート
//

class AvoidState : public State<Enemy> {
private:
	//右か左に回避するための変数
	int is_right;

public:
	AvoidState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();
};