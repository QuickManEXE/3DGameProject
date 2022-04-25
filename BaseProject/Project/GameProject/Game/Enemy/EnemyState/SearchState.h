#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

class SearchState : public State<Enemy> {
private:

	CVector3D m_rot_target;

	float m_time;
public:
	SearchState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();

	void Search();

	void Turn();
};