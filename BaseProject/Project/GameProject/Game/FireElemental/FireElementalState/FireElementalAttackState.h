#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../FireElemental.h"
#include"../StoneAttack.h"

class FireElementalAttackState : public State<FireElemental> {
private:
	StoneAttack* mp_stone;
	CVector3D mp_lefthand_pos;
public:
	FireElementalAttackState(FireElemental* owner);

	void Enter();

	void Execute();

	void Exit();
};