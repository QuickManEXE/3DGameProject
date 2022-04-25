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
	
	//�A�j���\�V�������I���܂�
	if (owner->GetModel()->isAnimationEnd()) {

		owner->m_StateAI.ChangeState(PlayerState::IdleState);

	}
	else {
		owner->m_Transform.position -= CMatrix::MRotation(owner->m_Transform.rotation).GetFront()*3.0f*DELTA;
	}

	owner->AddGravity();
	owner->AddMoveForce();

	//���[�V�����̍X�V
	owner->GetModel()->UpdateAnimation();
}

void HangDropState::Exit()
{
	owner->GetModel()->SetAnimationSpeed(1.0f);
}

void HangDropState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void HangDropState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
