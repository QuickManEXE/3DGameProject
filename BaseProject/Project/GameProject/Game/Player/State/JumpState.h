#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class JumpState : public State<Player> {
private:

	CVector3D rot_target;

public:
	JumpState(Player* owner);

	void Enter();

	void Execute();

	void Move();

	void Exit();

	void AimMove();

	void CollisionCheck(CollisionTask* _task);

};