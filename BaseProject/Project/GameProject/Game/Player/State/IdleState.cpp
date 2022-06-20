#include "IdleState.h"
#include"../../NonAnimObject.h"
#include"../../RopeConnectPoint.h"
#include"../../Rope/Rope.h"

IdleState::IdleState(Player* owner) : State(owner),is_run(false),is_hold(false){}

void IdleState::Enter()
{
	Camera::GetInstance()->SetCamera(owner);
	if (owner->mp_WireRope) owner->mp_WireRope->SetKill();
}

void IdleState::Execute()
{
	is_run = false;

	Move();

	//���N���b�N�������ƍU���Ɉڍs
	if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {

		owner->m_StateAI.ChangeState(PlayerState::ArrowAimState);

	}
	//�E�N���b�N�𒷉����Ń��C���[�G�C�����[�h�Ɉڍs
	if (CInput::GetState(0, CInput::eHold, CInput::eMouseR)&&owner->m_HookNum>0) {

		owner->m_StateAI.ChangeState(PlayerState::WireAimState);
	}

	//�����n�ʂɐڂ��Ă��Ȃ��̂������΁i�x�N�g���̑傫�������ƂȂ�΃W�����v����)
	//�����Ă���Ȃ�
	/*if (is_run) {
		if(owner->m_IsJump)owner->m_StateAI.ChangeState(PlayerState::JumpState);
	}*/

	//�t�b�N���[�h�؂�ւ�
	if (CInput::GetState(0, CInput::ePush, CInput::eButton3))
		owner->m_IsHook = (owner->m_IsHook)? false : true;

	//�d�͂�������
	owner->AddGravity();
	//���W�ɕω��ʂ�������
	owner->AddMoveForce();

	//���[�V�����̍X�V
	owner->GetModel()->UpdateAnimation();
}

void IdleState::Exit()
{
}

void IdleState::Move()
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
	//����SHIFT���������Ȃ瑖��
	if (HOLD(CInput::eButton8)) {
		is_run = true;
		speed *= 2.0f;
	}
	int _anim_dir = 0;
	//�����L�[������͕����x�N�g����ݒ�
	if (CInput::GetState(0, CInput::eHold, CInput::eUp)) {
		key_dir.z = 1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunForward : Player::PlayerAnimJam::WalkForward;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eDown)) {
		key_dir.z = -1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunBack : Player::PlayerAnimJam::WalkBack;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) {
		key_dir.x = 1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunLeft : Player::PlayerAnimJam::WalkLeft;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eRight)) {
		key_dir.x = -1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunRight : Player::PlayerAnimJam::WalkRight;
	}

	CMatrix rotMtx = CMatrix::MRotationY(owner->m_Transform.rotation.y);

	//�L�[���͂������
	if (key_dir.LengthSq() > 0) {
		//�ړ����� ��]�s��~�ړ�����
		CVector3D dir = rotMtx * key_dir;
		owner->m_Transform.m_pos_vec += dir * speed * DELTA;;
		owner->GetModel()->ChangeAnimation(_anim_dir);
	}
	else {
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Idle);
	}
}

void IdleState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}

void IdleState::CollisionCheck(CollisionTask* task)
{

	owner->CollisionObject(task);

	owner->HangCheck(task);

	owner->CollisionCharacter(task);

	owner->CollisionIsJump(task);
}
