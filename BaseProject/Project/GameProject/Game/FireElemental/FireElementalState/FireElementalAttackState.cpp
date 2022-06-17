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
	//左腕の座標を取得する
	mp_lefthand_pos = owner->m_model.GetFrameMatrix(28).GetPosition();

	//地面叩き付け攻撃　30.0f目で
	if (owner->m_model.GetAnimationFrame() == 30.0f&&owner->m_attack_type == FireElemental::AttackType::SlamGround) {
		//地面叩き付けの当たり判定を出す
		new SlamGroundCollider(UpdatePriority::eUp_Enemy_Attack,(owner->m_model.GetFrameMatrix("LeftHand").GetPosition() 
			+ owner->m_model.GetFrameMatrix("RightHand").GetPosition()) / 2.0f, 10.0f, 0.5f);
	}

	//岩投げつけ攻撃
	if (owner->m_attack_type == FireElemental::AttackType::ThrowStone) {
		//最初のフレームで
		if (owner->m_model.GetAnimationFrame() == 0.0f) {
			//プレイヤーのポジションを取得
			CVector3D target_pos = Player::GetInstance()->m_Transform.position;
			//岩の当たり判定を出現させる
			if(!mp_stone)mp_stone = new StoneAttack(UpdatePriority::eUp_Enemy_Attack, "Rock2",
				Transform(owner->m_model.GetFrameMatrix("LeftHand").GetPosition(), CVector3D::zero, CVector3D(6, 6, 6)),
				target_pos, 2.0f);
			//ポインターに設定する
			if(mp_stone)mp_stone->SetParent(&owner->m_model, 28);
			//プレイヤーに攻撃が来ると分かりやすくするために手元を光らせる
			EffectCollection::LightSplash3D(owner->m_model.GetFrameMatrix("LeftHand").GetPosition(),&mp_lefthand_pos);
		}

		//45.0f目で
		if (owner->m_model.GetAnimationFrame() == 45.0f) {
			//岩の打ち出し関数を呼ぶ
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