#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CrouchState : public State<Player> {

	CVector3D vec;

	bool is_crouch;
public:
	CrouchState(Player* owner);

	void Enter();

	void Execute();

	void Move();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* task);
};