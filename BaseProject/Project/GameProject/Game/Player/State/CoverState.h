#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class CoverState : public State<Player> {
	//‰E‚ğŒü‚¢‚Ä‚¢‚é‚©¶‚ğŒü‚¢‚Ä‚¢‚é‚©
	bool is_right;
public:
	CoverState(Player* owner);

	void Enter();

	void Execute();

	void Move();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);
};