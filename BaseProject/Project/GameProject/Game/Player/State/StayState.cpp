#include "StayState.h"
#include"../../Camera.h"

StayState::StayState(Player* owner) : State(owner)
{
}


void StayState::Enter()
{
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Idle);
}

void StayState::Execute()
{
	owner->AddGravity();
	owner->AddMoveForce();

	//���[�V�����̍X�V
	owner->GetModel()->UpdateAnimation();
}

void StayState::Exit()
{
	
}

void StayState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionObject(task);
}
