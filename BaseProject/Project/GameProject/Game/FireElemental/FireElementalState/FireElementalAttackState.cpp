#include "FireElementalAttackState.h"
#include"../SlamGroundCollider.h"
#include"../../Player/Player.h"
#include"../../../Effect/EffectCollection.h"

FireElementalAttackState::FireElementalAttackState(FireElemental* owner) : State(owner)
{
}

void FireElementalAttackState::Enter()
{
	owner->m_model.SetAnimationSpeed(0.9f);

	owner->m_model.ChangeAnimation(owner->m_attack_type,false);

	owner->m_is_attack = true;
}

void FireElementalAttackState::Execute()
{
	//���r�̍��W���擾����
	mp_lefthand_pos = owner->m_model.GetFrameMatrix(28).GetPosition();

	//�n�ʒ@���t���U���@30.0f�ڂ�
	if (owner->m_model.GetAnimationFrame() == 30.0f&&owner->m_attack_type == FireElemental::AttackType::SlamGround) {
		//�n�ʒ@���t���̓����蔻����o��
		new SlamGroundCollider(UpdatePriority::eUp_Enemy_Attack,(owner->m_model.GetFrameMatrix("LeftHand").GetPosition() 
			+ owner->m_model.GetFrameMatrix("RightHand").GetPosition()) / 2.0f, 10.0f, 0.5f);
	}

	//�Ⓤ�����U��
	if (owner->m_attack_type == FireElemental::AttackType::ThrowStone) {
		//�ŏ��̃t���[����
		if (owner->m_model.GetAnimationFrame() == 0.0f) {
			//�v���C���[�̃|�W�V�������擾
			CVector3D target_pos = Player::GetInstance()->m_Transform.position;
			//��̓����蔻����o��������
			if(!mp_stone)mp_stone = new StoneAttack(UpdatePriority::eUp_Enemy_Attack, "Rock2",
				Transform(owner->m_model.GetFrameMatrix("LeftHand").GetPosition(), CVector3D::zero, CVector3D(6, 6, 6)),
				target_pos, 2.0f);
			//�|�C���^�[�ɐݒ肷��
			if(mp_stone)mp_stone->SetParent(&owner->m_model, 28);
			//�v���C���[�ɍU��������ƕ�����₷�����邽�߂Ɏ茳�����点��
			EffectCollection::LightSplash3D(owner->m_model.GetFrameMatrix("LeftHand").GetPosition(),&mp_lefthand_pos);
		}

		//45.0f�ڂ�
		if (owner->m_model.GetAnimationFrame() == 45.0f) {
			//��̑ł��o���֐����Ă�
			if(mp_stone)mp_stone->SetShot();
			mp_stone = nullptr;
		}
	}


	if (owner->m_model.isAnimationEnd()) {

		owner->m_stateAI.ChangeState(FireElementalState::IdleState);

	}
}

void FireElementalAttackState::Exit()
{
	owner->m_is_attack = false;

	owner->m_model.SetAnimationSpeed(1.0f);
}