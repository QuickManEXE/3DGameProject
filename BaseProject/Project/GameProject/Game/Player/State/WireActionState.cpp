#include "WireActionState.h"
#include"../../Rope/Rope.h"

WireActionState::WireActionState(Player* owner) : State(owner)
{
	
}

void WireActionState::Enter()
{
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::WireAction);

	t = 0;
}

void WireActionState::Execute()
{
	float length = (owner->m_RopeConectPoint - owner->m_Transform.position).Length();
	if (length >= 1) {
		t = min(1, t + 0.1f/60.0f);

		owner->m_Transform.position = owner->m_Transform.position * (1 - t) + owner->m_RopeConectPoint * t;
	}
	else {

		owner->m_StateAI.ChangeState(PlayerState::IdleState);
		owner->mp_WireRope->SetKill();
	}
	
	owner->GetModel()->UpdateAnimation();
}

void WireActionState::Exit()
{
}

void WireActionState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionObject(task);
}
