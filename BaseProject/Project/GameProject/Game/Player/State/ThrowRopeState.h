#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class ThrowRopeState : public State<Player> {
private:
	CVector3D rope_pos;

	float t;
public:
	ThrowRopeState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* task);

};