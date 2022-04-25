#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../Enemy.h"

class DeathState : public State<Enemy> {
private:

	struct DeathAnim {
		const int anim_jam;
		const int frame_max;
	};


	DeathAnim damage_anim[3]{
		{14,91},
		{15,111},
		{16,99},
	};

	int rand_jam;

public:
	DeathState(Enemy* owner);

	void Enter();

	void Execute();

	void Exit();

	void Render();

	void CollisionCheck(CollisionTask* task);
};