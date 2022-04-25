#include "PlayerDamageState.h"

PlayerDamageState::PlayerDamageState(Player* owner) : State(owner)
{
}

void PlayerDamageState::Enter()
{
	if(owner->mp_Arrow)owner->mp_Arrow->SetKill();
	owner->mp_Arrow = false;
	

	int rand_idx = rand() % 2 + Player::PlayerAnimJam::Damage;

	owner->GetModel()->ChangeAnimation(rand_idx, false);
}

void PlayerDamageState::Execute()
{
	if (owner->GetModel()->isAnimationEnd()) {

		owner->m_StateAI.ChangeState(PlayerState::IdleState);

	}

	owner->AddGravity();
	owner->AddMoveForce();

	//モーションの更新
	owner->GetModel()->UpdateAnimation();

}

void PlayerDamageState::Exit()
{
}

void PlayerDamageState::CollisionCheck(CollisionTask* _task)
{
	owner->CollisionObject(_task);
}
