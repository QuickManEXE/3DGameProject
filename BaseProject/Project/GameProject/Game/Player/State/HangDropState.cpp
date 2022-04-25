#include "HangDropState.h"
#include"../../Geometry.h"

HangDropState::HangDropState(Player* owner) : State(owner)
{
}

void HangDropState::Enter()
{
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::HangDrop, false);

	owner->GetModel()->SetAnimationSpeed(4.0f);

}

void HangDropState::Execute()
{
	
	//アニメ―ションが終わるまで
	if (owner->GetModel()->isAnimationEnd()) {

		owner->m_StateAI.ChangeState(PlayerState::IdleState);

	}
	else {
		owner->m_Transform.position -= CMatrix::MRotation(owner->m_Transform.rotation).GetFront()*3.0f*DELTA;
	}

	owner->AddGravity();
	owner->AddMoveForce();

	//モーションの更新
	owner->GetModel()->UpdateAnimation();
}

void HangDropState::Exit()
{
	owner->GetModel()->SetAnimationSpeed(1.0f);
}

void HangDropState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void HangDropState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
