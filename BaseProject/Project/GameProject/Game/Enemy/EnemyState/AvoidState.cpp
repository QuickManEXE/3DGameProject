#include "AvoidState.h"
#include"../../Player/Player.h"

AvoidState::AvoidState(Enemy* owner) : State(owner)
{
}

void AvoidState::Enter()
{
	owner->GetModel()->ChangeAnimation(27, false);

	is_right = Utility::IsPlusMinus();
}

void AvoidState::Execute()
{
	CVector3D dir = Player::GetInstance()->m_Transform.position - owner->m_Transform.position;
	float ang = atan2(dir.x, dir.z);
	owner->m_Transform.rotation = (CVector3D(0, ang, 0));

	if (owner->GetModel()->isAnimationEnd()) {

		owner->m_StateAI.ChangeState(EnemyState::ChaseState);

	}
	else {

		float speed = 50.0f;

		if (is_right == 1) {

			owner->m_Transform.m_pos_vec -= CMatrix::MRotation(owner->m_Transform.rotation).GetLeft() * speed * DELTA;

		}
		else {

			owner->m_Transform.m_pos_vec += CMatrix::MRotation(owner->m_Transform.rotation).GetLeft() * speed * DELTA;

		}


	}

	owner->AddGravity();
	owner->AddMoveForce();

	owner->GetModel()->UpdateAnimation();
}

void AvoidState::Exit()
{
}
