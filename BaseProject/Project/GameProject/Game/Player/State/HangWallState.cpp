#include "HangWallState.h"


HangWallState::HangWallState(Player* owner) : State(owner)
{
}

void HangWallState::Enter()
{
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::HangWall, false);

	owner->GetModel()->SetAnimationSpeed(3.0f);

	owner->m_Transform.m_pos_vec = CVector3D::zero;
}

void HangWallState::Execute()
{
	
	//�ǂ��܂�̃A�j���[�V�����̑��t���[����
	int frame_max = owner->GetModel()->GetAnimationSize();

	//�A�j���\�V�������I���܂�
	
	if (owner->GetModel()->isAnimationEnd()) {

		//��������Ɠo��
		if (CInput::GetState(0, CInput::ePush, CInput::eUp)) {
			owner->m_StateAI.ChangeState(PlayerState::ClimeState);
		}

		//���������ƍ~���
		if (CInput::GetState(0, CInput::ePush, CInput::eDown)) {
			owner->m_StateAI.ChangeState(PlayerState::HangDropState);
		}

	}
	else {

		//�A�j���[�V�����̐i�s�x
		float t = min(0.5,owner->GetModel()->GetAnimationFrame() / frame_max);

		CVector3D clime_pos = owner->m_ClimePoint; 
		clime_pos.y -= owner->m_Param.GetParam("height");

		owner->m_Transform.position = owner->m_Transform.position*(1-t) + clime_pos *t;
	}

	//���[�V�����̍X�V
	owner->GetModel()->UpdateAnimation();

}

void HangWallState::Exit()
{
	owner->GetModel()->SetAnimationSpeed(1.0f);
}

void HangWallState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void HangWallState::CollisionCheck(CollisionTask* task)
{

	owner->CollisionObject(task);
}


