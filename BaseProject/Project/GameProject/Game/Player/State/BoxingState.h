#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Player.h"

class BoxingState : public State<Player> {
private:
	int attack_cnt;

	int attack_max;

	const float frame_max[4]{
		55,
		55,
		39,
		39,
	};

	const float born_to_be_born[4][2]{
		{17,19},
		{46,48},
		{74,76},
		{80,82},
	};


	CCapsule attack_capsule;
public:
	BoxingState(Player* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* _task);

};