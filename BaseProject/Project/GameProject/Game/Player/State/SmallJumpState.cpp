#include "SmallJumpState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"

SmallJumpState::SmallJumpState(Player* owner) : State(owner)
{
}

void SmallJumpState::Enter()
{
	printf("��W�����v�J�n�I\n");
	//�x�N�g���ɃW�����v�̗͂�������
	owner->transform.m_vec.y += owner->m_param.GetParam("jump_power");
	//�W�����v�̃A�j���[�V�����ɐ؂�ւ���
	owner->m_model_a3m.ChangeAnimation(4, false);
}

void SmallJumpState::Execute()
{
	//�L�����N�^�[�̌�����
	CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

	//�O�����ɐi��
	float speed = owner->m_param.GetParam("move_speed") * 10.0f;

	//�ړ��x�N�g�������Z
	owner->transform.m_vec += dir * speed * DELTA;

	//�n�ʂɂ�����
	if (owner->m_IsGround) {
		owner->state_AI.ChangeState(PlayerState::IdleState);
	}

	owner->AddGravity();

	owner->m_flag.SetParam("is_ground", false);
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();
}

void SmallJumpState::Exit()
{
}

void SmallJumpState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void SmallJumpState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
