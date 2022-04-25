#include "EnemyAttackState.h"
#include"../../Player/Player.h"
#include"../../../SoundResource.h"

EnemyAttackState::EnemyAttackState(Enemy* owner) : State(owner),attack_cooldown(0)
{
}


void EnemyAttackState::Enter()
{
	owner->GetModel()->ChangeAnimation(22, false);

	owner->m_IsAttack = true;

	SOUND(SoundResource::SE_Punch1.c_str())->Play3D(owner->m_Transform.position, owner->m_Transform.position);
}

void EnemyAttackState::Execute()
{
	if (owner->GetModel()->isAnimationEnd()) {
		
		owner->m_StateAI.ChangeState(EnemyState::BattleIdleState);

	}

	owner->AddGravity();
	owner->AddMoveForce();

	owner->GetModel()->UpdateAnimation();

}

void EnemyAttackState::Exit()
{
	owner->m_IsAttack = false;
}

