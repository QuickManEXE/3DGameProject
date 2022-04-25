#pragma once
#include"../../../StateMachine/StateAI.h"
#include"../FireElemental.h"
#include"../../Camera.h"

class FireElementalDeathState : public State<FireElemental>,public ICamera {
private:

	float t;

	struct CameraPosData {
		CVector3D pos;
		CVector3D at;
	};

	static const int data_max=3;
	static CameraPosData m_posData[data_max];
	int m_idx;

	bool is_effect;

public:
	FireElementalDeathState(FireElemental* owner);

	void Enter();

	void Execute();

	void Exit();

	void Death();

	void UpdateCamera()override;
};