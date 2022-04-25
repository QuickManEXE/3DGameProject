#include "EnemyHangWallState.h"

EnemyHangWallState::EnemyHangWallState(Enemy* owner) : State(owner)
{
}

void EnemyHangWallState::Enter()
{
	t = 0;

	owner->GetModel()->ChangeAnimation(23, false);

	
}

void EnemyHangWallState::Execute()
{
	t += DELTA;

	if (owner->GetModel()->isAnimationEnd()) {

		//•Ç‚Ì‚Ú‚è‚ð‚â‚ß‚é
		owner->m_StateAI.ChangeState(EnemyState::IntervalState);

	}else {

		owner->m_Transform.position = owner->m_Transform.position * (1 - t) + owner->m_ClimePoint * t;

	}

	owner->GetModel()->UpdateAnimation();
}

void EnemyHangWallState::Exit()
{
}

