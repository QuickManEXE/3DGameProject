#include "EnemyMoveState.h"
#include"../../Player/Player.h"
#include"../../../SoundResource.h"

EnemyMoveState::EnemyMoveState(FireElemental* owner) : State(owner)
{
}

void EnemyMoveState::Enter()
{
	rot_target = CVector3D(0, 0, 0);

	SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Play(true);
}

void EnemyMoveState::Execute()
{
	//�U���͈͂ɓ���܂ŋ߂Â�
	Move();
}

void EnemyMoveState::Exit()
{
	SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Pause();
}

void EnemyMoveState::Move()
{
	//�v���C���[�ւ̍U���̎�ނɂ���čU�����@���ς��

	CVector3D target_pos = Player::GetInstance()->m_Transform.position;
	target_pos.y = 0;
	CVector3D owner_pos = owner->m_Transform.position;
	owner_pos.y = 0;

	CVector3D dir = target_pos - owner_pos;

	if(Utility::IsViewInside(owner_pos, owner->m_Transform.rotation, target_pos, DtoR(10), owner->m_attack_range)) {

		SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Stop();
		printf("���E���ɓ����Ă��܂�%.0f\n", dir.Length());
		owner->m_stateAI.ChangeState(FireElementalState::AttackState);
	}
	else {

		float ang = atan2(dir.x, dir.z);

		rot_target = (CVector3D(0, ang, 0));
		//owner->transform.rotation = (CVector3D(0, ang, 0));
		//�ړ�����
		float speed = 2.0f;

		owner->m_Transform.m_pos_vec += (dir.GetNormalize() * speed);

		owner->m_model.ChangeAnimation(2);

	}

	//���p�x�̕��
	//�p�x���������߂�i�ڕW�l-���ݒl�j
	//�p�x������-��(-180)�`��(180)�͈̔͂ɐ�����
	float a = Utility::NormalizeAngle(rot_target.y - owner->m_Transform.rotation.y);

	//�����ɂ����
	//rot.y += a*0.1f;

	//���ʉ�]�ɂ��p�x���
	float rot_speed = DtoR(30);
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

}





