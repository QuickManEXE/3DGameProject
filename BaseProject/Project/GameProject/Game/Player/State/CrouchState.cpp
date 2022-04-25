#include "CrouchState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"


CrouchState::CrouchState(Player* owner) : State(owner),is_crouch(false)
{

}

void CrouchState::Enter()
{
	is_crouch = false;

	owner->m_model_a3m.ChangeAnimation(11, false);
}

void CrouchState::Execute()
{
	if (is_crouch) {

		Move();

		//�R���g���[���������Ƃ��Ⴊ�݃��[�h������
		if (CInput::GetState(0, CInput::ePush, CInput::eButton9)) {

			owner->m_model_a3m.ChangeAnimation(14, false);
			owner->state_AI.ChangeState(PlayerState::IdleState);

		}

	}
	else {

		if (owner->m_model_a3m.GetAnimationFrame() >= 19) 
			is_crouch = true;

	}

	owner->AddGravity();
	owner->AddForce();

	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();

	//�J�����̐ݒ�
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);
}

void CrouchState::Move()
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

	    owner->m_model_a3m.ChangeAnimation(13);
	}
	else {
		owner->m_model_a3m.ChangeAnimation(12);
	}
}

void CrouchState::Exit()
{
}

void CrouchState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void CrouchState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);	
}
