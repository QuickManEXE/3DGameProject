#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class IdleState : public State<Player> {
private:
	//走っているかどうか
	bool is_run;
	//平行移動しているかどうか
	bool is_hold;
	//キャラクターの回転の方向
	CVector3D rot_target;
public:
	IdleState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Move();

	void Render();

	void CollisionCheck(CollisionTask* _task);

	
};
