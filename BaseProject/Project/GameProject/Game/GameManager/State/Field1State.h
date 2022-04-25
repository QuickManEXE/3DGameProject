#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"

class Field1State : public State<GameManager>{
private:
	float t;
	//�C�x���g�p�̉~
	CSphere m_event;
public:
	Field1State(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();
};