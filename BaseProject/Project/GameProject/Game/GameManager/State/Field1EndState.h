#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"

class Field1EndState : public State<GameManager> ,public ICamera{
private:
	//イベントの時間
	float t;

	bool event_flag;

	//イベント用の円
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