#include "DamageState.h"


DamageState::DamageState(Enemy* owner) : State(owner)
{
}

void DamageState::Enter()
{
	owner->GetModel()->ChangeAnimation(15, false);
}

void DamageState::Execute()
{

	if (owner->GetModel()->isAnimationEnd()) {

		owner->m_StateAI.ChangeState(EnemyState::BattleIdleState);

	}
	

	owner->AddGravity();
	owner->AddMoveForce();

	owner->GetModel()->UpdateAnimation();
}

void DamageState::Exit()
{
}

