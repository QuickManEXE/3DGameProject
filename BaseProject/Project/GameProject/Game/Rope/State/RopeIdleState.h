#pragma once
#include"../../../StateMachine/State.h"
#include"../Rope.h"

class RopeIdleState : public State<Rope> {
private:

	CVector3D vec;

public:
	RopeIdleState(Rope* owner);

	void Enter();

	void Execute();

	void Exit();

};