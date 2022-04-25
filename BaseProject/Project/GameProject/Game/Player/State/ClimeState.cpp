#include "ClimeState.h"


ClimeState::ClimeState(Player* owner) : State(owner)
{
}

void ClimeState::Enter()
{
	//�A�j���[�V�����̃X�s�[�h��ύX
	owner->GetModel()->SetAnimationSpeed(2.0f);
	//�ǂ̂ڂ�A�j���[�V�����Ɉڍs
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Clime, false);
}

void ClimeState::Execute()
{

	//�ǂ̂ڂ�̃A�j���[�V�����̑��t���[����
	float frame_max = 116;
	//�ǂ�o�肾���n�܂�̃t���[����
	float frame_clime_begin = 9;
	//�ǂ̂ڂ�̍����������t���[����
	float frame_clime = 70;
	//�A�j���[�V���������������
	if (owner->GetModel()->isAnimationEnd()) {

		//������n�_�ɍ��킹��
		owner->m_Transform.position = owner->m_ClimePoint;
		//�A�C�h���X�e�[�g�ɂ��ǂ�
		owner->m_StateAI.ChangeState(PlayerState::IdleState);

	}
	else {
		//�A�j���[�V�������I����ĂȂ����

		//�o��n�߂�t���[���܂ł͉������Ȃ�
		if (owner->GetModel()->GetAnimationFrame() <= frame_clime_begin) {
			


		}else //�o��n�߂���ŏI�̓o��n�_�܂ō��W��J�ڂ��Ă���
		if (owner->GetModel()->GetAnimationFrame() <= frame_clime) {
			//�A�j���[�V�����̐i�s�x
			float t = min(1, (owner->GetModel()->GetAnimationFrame() - frame_clime_begin) / frame_clime);

			owner->m_Transform.position.y = 
				owner->m_Transform.position.y * (1 - t) + ((owner->m_ClimePoint.y - owner->m_Param.GetParam("height")/2) * t);
		}
		else {

			//�o��؂����A�j���[�V�����̃t���[���ɂȂ�����X��Y�̍��W�����킹�Ă���
			//�A�j���[�V�����̐i�s�x
			float t = min(1, (owner->GetModel()->GetAnimationFrame()-frame_clime) / frame_max);

			owner->m_Transform.position =
				owner->m_Transform.position * (1 - t) + ((owner->m_ClimePoint) * t);
		}
	}
	
	//���[�V�����̍X�V
	owner->GetModel()->UpdateAnimation();
}

void ClimeState::Exit()
{
	//�A�j���[�V�����̑��x��߂��Ă���
	owner->GetModel()->SetAnimationSpeed(1.0f);
}

void ClimeState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}
