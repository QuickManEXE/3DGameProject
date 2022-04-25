#include "PlayerDeathState.h"
#include"../../GameManager/GameManager.h"

PlayerDeathState::PlayerDeathState(Player* owner) : State(owner)
{
}

void PlayerDeathState::Enter()
{


	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Death, false);

	m_time = 5.0f;
}

void PlayerDeathState::Execute()
{
	if (owner->GetModel()->isAnimationEnd())m_time -= DELTA;

	if (m_time <= 0) {

		GameManager::GetInstance()->m_StateAI.ChangeState(GameManager::GameManagerState::GameOverState);

	}

	owner->AddGravity();
	owner->AddMoveForce();

	//モーションの更新
	owner->GetModel()->UpdateAnimation();
}

void PlayerDeathState::Exit()
{

}

void PlayerDeathState::CollisionCheck(CollisionTask* _task)
{
	owner->CollisionObject(_task);
}
