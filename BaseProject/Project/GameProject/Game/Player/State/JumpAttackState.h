#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class JumpAttackState : public State<Player> {
private:

	CVector3D rot_target;

public:
	JumpAttackState(Player* owner);

	void Enter();

	void Execute();

	void Move();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);
};