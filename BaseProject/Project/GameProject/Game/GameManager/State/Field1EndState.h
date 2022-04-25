#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"

class Field1EndState : public State<GameManager> ,public ICamera{
private:
	//�C�x���g�̎���
	float t;

	bool event_flag;

	//�C�x���g�p�̉~
	CSphere m_event;
public:
	Field1EndState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void Field1Update();

	void Render()override;

	void CollisionCheck(CollisionTask* task);

	void BreakRock();

	void UpdateCamera()override;
};