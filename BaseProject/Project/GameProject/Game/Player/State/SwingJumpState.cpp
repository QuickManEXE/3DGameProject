#include "SwingJumpState.h"
#include"../../Geometry.h"
#include"../../CameraManager.h"

SwingJumpState::SwingJumpState(Player* owner) : State(owner)
{
}

void SwingJumpState::Enter()
{
	printf("�X�B���O�W�����v�J�n�I\n");
	//�W�����v�̃A�j���[�V�����ɐ؂�ւ���
	owner->m_model_a3m.ChangeAnimation(4, false);
}

void SwingJumpState::Execute()
{
	//�L�����N�^�[�̌�����
	CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

	//�O�����ɐi��
	float speed = owner->m_param.GetParam("move_speed") * 2.0f;

	//�ړ��x�N�g�������Z
	owner->transform.m_vec += dir * speed * DELTA;

	owner->RopeSearch();

	//�n�ʂɂ�����
	if (owner->m_IsGround) {

		owner->state_AI.ChangeState(PlayerState::IdleState);

	}

	//�}�E�X���{�^�����N���b�N�����
	if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {
		//�W�����v�A�^�b�N�X�e�[�g�Ɉڍs
		//owner->state_AI.ChangeState(PlayerState::JumpAttackState);

	}

	
	owner->AddGravity();
	owner->AddForce();

	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();
}

void SwingJumpState::Exit()
{
}

void SwingJumpState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void SwingJumpState::CollisionCheck(CollisionTask* task)
{
	
	owner->CollisionCheckToField(task);

	//owner->CollisionRope(task);	
}
