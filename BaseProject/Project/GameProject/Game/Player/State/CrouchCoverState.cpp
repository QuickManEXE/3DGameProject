#include "CrouchCoverState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"

CrouchCoverState::CrouchCoverState(Player* owner) : State(owner),is_cover(false),is_right(true)
{
}

void CrouchCoverState::Enter()
{
	is_right = true;

	is_cover = false;

	owner->m_model_a3m.ChangeAnimation(23, false);
}

void CrouchCoverState::Execute()
{
	if (is_cover) {

		Move();

		//���������ƃJ�o�[���[�h������
		if (CInput::GetState(0, CInput::ePush, CInput::eDown)) {

			owner->m_model_a3m.ChangeAnimation(14, false);
			owner->state_AI.ChangeState(PlayerState::CrouchState);

		}


	}
	else {
		if (owner->m_model_a3m.GetAnimationFrame() >= 19) is_cover = true;
	}

	owner->AddGravity();

	owner->m_flag.SetParam("is_ground", false);
	//�J�����̐ݒ�
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();
}

void CrouchCoverState::Move()
{
	float speed = owner->m_param.GetParam("move_speed") / 4;

	//�ǂɕ��s�ړ����邽�߂̕ǂ̖@��
	CVector3D normal = owner->cover_wall_normal;

	//�@���̊p�x���o��
	float ang = atan2(normal.x, normal.z);

	//�E�����x�N�g��
	CVector3D right_dir = CVector3D(sin(ang + DtoR(90)), 0, cos(ang + DtoR(90)));

	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) {

		owner->transform.m_vec -= right_dir * speed;
		owner->m_model_a3m.ChangeAnimation(26);
		is_right = false;
	}
	else if (CInput::GetState(0, CInput::eHold, CInput::eRight)) {

		owner->transform.m_vec += right_dir * speed;
		owner->m_model_a3m.ChangeAnimation(24);
		is_right = true;
	}
	else {
		if (is_right)
			owner->m_model_a3m.ChangeAnimation(23);
		else owner->m_model_a3m.ChangeAnimation(25);
	}
}

void CrouchCoverState::Exit()
{
}

void CrouchCoverState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void CrouchCoverState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
