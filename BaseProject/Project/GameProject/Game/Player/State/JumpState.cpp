#include "JumpState.h"
#include"../../Geometry.h"
#include "IdleState.h"

JumpState::JumpState(Player* owner) : State(owner)
{
}

void JumpState::Enter()
{
	float jump_power = 10.0f;
	//�x�N�g���ɃW�����v�̗͂�������
	owner->m_Transform.m_pos_vec.y += jump_power;// owner->m_Param.GetParam("jump_power");

	//�W�����v�̃A�j���[�V�����ɐ؂�ւ���
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Jump, false);

	rot_target = CVector3D::zero;
}

void JumpState::Execute()
{
	//�L�����N�^�[�̌�����
	CVector3D dir(sin(owner->m_Transform.rotation.y), 0, cos(owner->m_Transform.rotation.y));

	//�O�����ɐi��
	float speed = 10.0f;// owner->m_param.GetParam("move_speed");

	//�ړ��x�N�g�������Z
	owner->m_Transform.m_pos_vec += dir * speed * DELTA;

	AimMove();

	//owner->RopeSearch();

	//�n�ʂɂ�����
	if (owner->m_IsGround) {
		owner->m_StateAI.ChangeState(PlayerState::IdleState);
	}

	owner->AddGravity();
	owner->AddMoveForce();

	//���[�V�����̍X�V
	owner->GetModel()->UpdateAnimation();
}

void JumpState::Move()
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
	float a = Utility::NormalizeAngle(rot_target.y - owner->m_Transform.rotation.y);

	//�����ɂ����
	//rot.y += a*0.1f;

	//���ʉ�]�ɂ��p�x���
	float rot_speed = DtoR(360);
	if (a > rot_speed) {
		owner->m_Transform.rotation.y += rot_speed * DELTA;
	}
	else
		if (a < -rot_speed) {
			owner->m_Transform.rotation.y -= rot_speed * DELTA;
		}
		else {
			owner->m_Transform.rotation.y += a * DELTA;
		}

	//��Ԍ��-��(-180)�`��(180)�͈̔͂Ɋp�x�𐮂���
	owner->m_Transform.rotation.y = Utility::NormalizeAngle(owner->m_Transform.rotation.y);
	//--------------------------------------------------------


}

void JumpState::Exit()
{
}

void JumpState::AimMove()
{
	//�}�E�X����ŃL�����N�^�[����]----
	CVector2D mouse_vec = CInput::GetMouseVec();
	owner->m_Transform.rotation.y += mouse_vec.x * -0.01f;
	owner->m_Transform.rotation.x += mouse_vec.y * 0.01f;
	owner->m_Transform.rotation.x = min(DtoR(45), max(DtoR(-45), owner->m_Transform.rotation.x));


	//�����L�[�̓��͕����x�N�g��
	CVector3D key_dir(0, 0, 0);
	//�L�����N�^�[�̈ړ���
	float speed = owner->m_Param.GetParam(Player::move_speed);
	int _anim_dir = 0;
	//�����L�[������͕����x�N�g����ݒ�
	if (CInput::GetState(0, CInput::eHold, CInput::eUp)) {
		key_dir.z = 1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunForward : Player::PlayerAnimJam::WalkForward;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eDown)) {
		key_dir.z = -1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunBack : Player::PlayerAnimJam::WalkBack;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) {
		key_dir.x = 1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunLeft : Player::PlayerAnimJam::WalkLeft;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eRight)) {
		key_dir.x = -1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunRight : Player::PlayerAnimJam::WalkRight;
	}

	CMatrix rotMtx = CMatrix::MRotationY(owner->m_Transform.rotation.y);

	//�L�[���͂������
	if (key_dir.LengthSq() > 0) {
		//�ړ����� ��]�s��~�ړ�����
		CVector3D dir = rotMtx * key_dir;
		owner->m_Transform.m_pos_vec += dir * speed * DELTA;;
		//owner->m_model_a3m.ChangeAnimation(_anim_dir);
	}
}

void JumpState::CollisionCheck(CollisionTask* task)
{
	
	owner->CollisionCheckToField(task);

}


