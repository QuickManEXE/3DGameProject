#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../GameManager.h"
#include"../../Camera.h"
#include"../../FireElemental/FireElemental.h"

class BossState : public State<GameManager> {
private:
	FireElemental* mp_fire;
	Base* mp_field;
public:
	BossState(GameManager* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render()override;

	void CollisionCheck(CollisionTask* task)override;

	
};