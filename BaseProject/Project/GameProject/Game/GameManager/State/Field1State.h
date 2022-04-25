#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"

class Field1State : public State<GameManager>{
private:
	float t;
	//イベント用の円
	CSphere m_event;
public:
	Field1State(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();
};