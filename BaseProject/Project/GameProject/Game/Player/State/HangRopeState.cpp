#include "HangRopeState.h"
#include"../../CameraManager.h"

HangRopeState::HangRopeState(Player* owner) : State(owner)
{

}

void HangRopeState::Enter()
{
	owner->m_model_a3m.ChangeAnimation(10);
}

void HangRopeState::Execute()
{
	//���[�v������Ȃ�
	if (owner->rope) {

		RopeIdle();
		
		if (CInput::GetState(0, CInput::ePush, CInput::eButton5)) {
			//�v���C���[�����[�v�𗣂��Ƃ��̏���
			ReleaseRope();
		}

	}

	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//���[�V�����̍X�V
	owner->m_model_a3m.UpdateAnimation();
}

void HangRopeState::Exit()
{
}

void HangRopeState::RopeIdle()
{
	//���[�v�̉�]�ƃv���C���[�̉�]�����킹��
	owner->rope->transform.rotation.y = owner->transform.rotation.y;

	//����
	if (CInput::GetState(0, CInput::eHold, CInput::eUp)) owner->rope->transform.m_vec.x -= DtoR(1) * DELTA;
	if (CInput::GetState(0, CInput::eHold, CInput::eDown)) owner->rope->transform.m_vec.x += DtoR(0.5) * DELTA;
	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) owner->transform.rotation.y += DtoR(1.0);
	if (CInput::GetState(0, CInput::eHold, CInput::eRight)) owner->transform.rotation.y -= DtoR(1.0);

	//�d�͂�0�ɂ���
	owner->transform.m_vec.y = 0;

	//�v���C���[�����܂����n�_���X�V����
	owner->transform.position = owner->rope->GetConectPoint();

	owner->transform.rotation.x = owner->rope->transform.rotation.x;

}

void HangRopeState::ReleaseRope()
{
	//�v���C���[�̍�����
	CVector3D player_left = CMatrix::MRotation(owner->transform.rotation).GetLeft();
	//���[�v�̎n�_
	CVector3D ropeS = owner->rope->GetLineS();

	//�v���C���[���烍�[�v�̎n�_�ւ̃x�N�g��
	CVector3D up = ropeS - owner->rope->GetConectPoint();

	//�O�ς��g���O���������߂�
	CVector3D front = CMatrix::MRotation(owner->transform.rotation).GetFront();

	CVector3D::Cross(player_left, up);

	//�␳
	float correction = 20.0f; fabs(Utility::NormalizeAngle(owner->rope->transform.rotation.x));
	//�v���C���[�̑O�����ɔ�΂�
	owner->transform.m_vec += front;// *correction;//�␳��������

	owner->rope = nullptr;

	owner->transform.rotation.x = 0;

	owner->state_AI.ChangeState(PlayerState::BigJumpState);
}

void HangRopeState::Render()
{
	//�L�����N�^�[�\��
	owner->StandardRender();
}
