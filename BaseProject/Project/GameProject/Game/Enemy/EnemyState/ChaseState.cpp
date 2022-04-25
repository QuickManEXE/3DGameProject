#include "ChaseState.h"
#include"../../Player/Player.h"

ChaseState::ChaseState(Enemy* owner) : State(owner)
{
}

void ChaseState::Enter()
{
	m_search_lost_time = 5.0f;
}

void ChaseState::Execute()
{
	CVector3D dir = Player::GetInstance()->m_Transform.position - owner->m_Transform.position;

	if (dir.Length() >= 1.5f) {

		float ang = atan2(dir.x, dir.z);

		owner->m_Transform.rotation = (CVector3D(0, ang, 0));

		owner->m_Transform.m_pos_vec += dir.GetNormalize() * DELTA * 30.0f;

		owner->GetModel()->ChangeAnimation(18);
	}
	else {

		//‹ß‚Ã‚¢‚½‚çUŒ‚‚ÉˆÚ‚é
		owner->m_StateAI.ChangeState(EnemyState::AttackState);

	}

	if (owner->SearchPlayer()) {

		m_search_lost_time = 5.0f;
	}
	else {

		m_search_lost_time -= DELTA;
		if (m_search_lost_time <= 0) {
			printf("Œ©Ž¸‚Á‚½\n");
			owner->m_StateAI.ChangeState(EnemyState::IntervalState);
		}
	}


	owner->AddGravity();
	owner->AddMoveForce();

	owner->GetModel()->UpdateAnimation();
}

void ChaseState::Exit()
{
}
