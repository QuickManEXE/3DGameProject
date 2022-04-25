#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../FireElemental.h"

class EnemyMoveState : public State<FireElemental> {
private:
	//�U���͈̔�
	float attack_range;

	//��]�̖ڕW�l
	CVector3D rot_target;
public:
	EnemyMoveState(FireElemental* owner);

	void Enter();

	void Execute();

	void Exit();

	void Move();

};