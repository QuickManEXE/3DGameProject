#include "RopeIdleState.h"



RopeIdleState::RopeIdleState(Rope* owner) : State(owner)
{
}

void RopeIdleState::Enter()
{
}

void RopeIdleState::Execute()
{
	//���[�v�̊p�x����s������߂�
	CMatrix m = CMatrix::MRotation(owner->m_Transform.rotation);

	//���[�v�̏I�[�̈ʒu������
	//owner->lineE = owner->lineS + -m.GetUp() * owner->rope_length;

	//�v���C���[���߂܂��Ă���ʒu���X�V
	//owner->m_conect_point = owner->lineS + -m.GetUp() * owner->conect_dir;

	
}

void RopeIdleState::Exit()
{
}

