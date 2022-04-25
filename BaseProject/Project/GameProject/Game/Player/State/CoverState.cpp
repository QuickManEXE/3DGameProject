#include "CoverState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"

CoverState::CoverState(Player* owner) : State(owner),is_right(true)
{
}

void CoverState::Enter()
{
	is_right = true;
}

void CoverState::Execute()
{
	Move();

	//���������ƃJ�o�[���[�h������
	if (CInput::GetState(0, CInput::ePush, CInput::eDown)) {

		owner->state_AI.ChangeState(PlayerState::CoverEndState);

	}

	owner->AddGravity();
	owner->AddForce();


	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();

	//�J�����̐ݒ�
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);
}

void CoverState::Move()
{

	float speed = owner->m_param.GetParam("move_speed")/3;

	//�ǂɕ��s�ړ����邽�߂̕ǂ̖@��
	CVector3D normal = owner->cover_wall_normal;

	//�@���̊p�x���o��
	float ang = atan2(normal.x, normal.z);

	//�E�����x�N�g��
	CVector3D right_dir = CVector3D(sin(ang + DtoR(90)), 0, cos(ang + DtoR(90)));

	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) {
		
		owner->transform.m_vec -= right_dir* speed * DELTA;
		owner->m_model_a3m.ChangeAnimation(21);
		is_right = false;
	}
	else if (CInput::GetState(0, CInput::eHold, CInput::eRight)) {

		owner->transform.m_vec += right_dir * speed * DELTA;
		owner->m_model_a3m.ChangeAnimation(17);
		is_right = true;
	}
	else {
		if(is_right)
		owner->m_model_a3m.ChangeAnimation(16);
		else owner->m_model_a3m.ChangeAnimation(20);
	}
	
}

void CoverState::Exit()
{
}

void CoverState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void CoverState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}