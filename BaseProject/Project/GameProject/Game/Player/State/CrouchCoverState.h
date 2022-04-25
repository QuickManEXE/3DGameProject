#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CrouchCoverState : public State<Player> {
private:
	//カバーの始めのアクションが終わったかどうか
	bool is_cover;
	//右を向いているか左を向いているか
	bool is_right;
public:
	CrouchCoverState(Player* owner);

	void Enter();

	void Execute();

	void Move();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* task);
};