#include "BattleIdleState.h"
#include"../../Player/Player.h"

BattleIdleState::BattleIdleState(Enemy* owner) : State(owner)
{
}

void BattleIdleState::Enter()
{
	m_active_time = rand() % 1 +0.5f;

	owner->GetModel()->ChangeAnimation(17);
}

void BattleIdleState::Execute()
{
	CVector3D dir = Player::GetInstance()->m_Transform.position - owner->m_Transform.position;

	float ang = atan2(dir.x, dir.z);

	owner->m_Transform.rotation = (CVector3D(0, ang, 0));

	m_active_time -= DELTA;

	if (m_active_time <= 0) {

		int i = rand() % 100;

		if (i < 50) {
			//ƒ‰ƒ“ƒ_ƒ€‚É“G‚É‹ß‚Ã‚¢‚½‚è
			owner->m_StateAI.ChangeState(EnemyState::ChaseState);
		}
		else if (i < 75) {
			//Œ„‚ðŒ©‚¹‚½‚è‚·‚é

			owner->m_StateAI.ChangeState(EnemyState::AvoidState);

		}
		else if (i < 100) {
			//‰ñ”ð‚µ‚½‚è

			owner->m_StateAI.ChangeState(EnemyState::AvoidState);


		}

	}

	owner->AddGravity();
	owner->AddMoveForce();

	owner->GetModel()->UpdateAnimation();
}

void BattleIdleState::Exit()
{
}

