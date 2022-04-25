#include "JumpAttackState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"

JumpAttackState::JumpAttackState(Player* owner) : State(owner)
{
}

void JumpAttackState::Enter()
{
	owner->m_model_a3m.SetAnimationSpeed(1.5f);
	//�W�����v�̃A�j���[�V�����ɐ؂�ւ���
	owner->m_model_a3m.ChangeAnimation(31, false);
}

void JumpAttackState::Execute()
{
	const float frame_max = 45;

	if (owner->m_model_a3m.GetAnimationFrame() >= frame_max) {

		owner->state_AI.ChangeState(PlayerState::IdleState);

	}

	Move();

	//�L�����N�^�[�̌�����
	CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

	//�O�����ɐi��
	float speed = owner->m_param.GetParam("move_speed");

	//�ړ��x�N�g�������Z
	owner->transform.m_vec += dir * speed * DELTA;

	owner->AddGravity();
	owner->AddForce();

	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();

	CameraManager::GetInstance()->ControllCamera(owner->transform.position);
}

void JumpAttackState::Move()
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

	//�L�[���͂������
	if (key_dir.LengthSq() > 0) {

		//���͕���������͉�]�l���v�Z
		key_ang = atan2(key_dir.x, key_dir.z);

		//�J�����̉�]�l�Ɠ��͉�]�l����L�����N�^�[�̉�]�l������
		//������rot.y�ɐݒ肹����U�ڕW�l��ݒ肷��
		//owner->transform.rotation.y = cam_ang + key_ang;
		rot_target.y = cam_ang + key_ang;
	}

	//------------------------------------------------------
		//���p�x�̕��
		//�p�x���������߂�i�ڕW�l-���ݒl�j
		//�p�x������-��(-180)�`��(180)�͈̔͂ɐ�����
	float a = Utility::NormalizeAngle(rot_target.y - owner->transform.rotation.y);

	//�����ɂ����
	//rot.y += a*0.1f;

	//���ʉ�]�ɂ��p�x���
	float rot_speed = DtoR(360);
	if (a > rot_speed) {
		owner->transform.rotation.y += rot_speed * DELTA;
	}
	else
		if (a < -rot_speed) {
			owner->transform.rotation.y -= rot_speed * DELTA;
		}
		else {
			owner->transform.rotation.y += a * DELTA;
		}

	//��Ԍ��-��(-180)�`��(180)�͈̔͂Ɋp�x�𐮂���
	owner->transform.rotation.y = Utility::NormalizeAngle(owner->transform.rotation.y);
	//--------------------------------------------------------


}

void JumpAttackState::Exit()
{
	owner->m_model_a3m.SetAnimationSpeed(1.0f);
}

void JumpAttackState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void JumpAttackState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
