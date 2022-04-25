#include "GodState.h"
#include"../../CameraManager.h"

GodState::GodState(Player* owner) : State(owner)
{
}

void GodState::Enter()
{
}

void GodState::Execute()
{
	Move();

	
	owner->AddGravity();
	//���W�ɕω��ʂ�������
	owner->AddForce();


	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();

	//�J�����̐ݒ�
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);
}

void GodState::Exit()
{
}

void GodState::Move()
{
	//�L�����N�^�[����
	//�����L�[�̓��͕����x�N�g��
	CVector3D key_dir(0, 0, 0);
	//�J�����̕����x�N�g��
	CVector3D cam_dir = CCamera::GetCurrent()->GetDir();
	//���͉�]�l
	float key_ang = 0;
	//�J�����̉�]�l
	float cam_ang = atan2(cam_dir.x, cam_dir.z);

	key_dir = Utility::GetInputKeyDir();

	float speed = owner->m_param.GetParam("move_speed");

	
	//�L�[���͂������
	if (key_dir.LengthSq() > 0) {

		//���͕���������͉�]�l���v�Z
		key_ang = atan2(key_dir.x, key_dir.z);

		//�J�����̉�]�l�Ɠ��͉�]�l����L�����N�^�[�̉�]�l������
		//������rot.y�ɐݒ肹����U�ڕW�l��ݒ肷��
		owner->transform.rotation.y = cam_ang + key_ang;

		//�ړ�����
		CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

		//�ړ��x�N�g�������Z����
		owner->transform.m_vec += dir * speed * DELTA;

		float run_speed = 10.0f;

		owner->m_model_a3m.ChangeAnimation(2);
	}
	else {
		owner->m_model_a3m.ChangeAnimation(1);
	}

	//���N���b�N�������ƍU���Ɉڍs
	if (CInput::GetState(0, CInput::eHold, CInput::eButton5)) {

		owner->transform.m_vec.y += owner->m_param.GetParam("jump_power")*5.0f * DELTA;
	}
}

void GodState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
