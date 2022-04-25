#include"EnemyIdleState.h"
#include"../../../SoundResource.h"

EnemyIdleState::EnemyIdleState(FireElemental* owner) : State(owner)
{
}

void EnemyIdleState::Enter()
{
	owner->m_model.ChangeAnimation(1, false);

	owner->m_model.SetAnimationSpeed(2.0f);

	Thinking();
}

void EnemyIdleState::Execute()
{

	if (owner->m_model.isAnimationEnd()) {
		//敵が近くにいると思考モードに入る
		owner->m_stateAI.ChangeState(FireElementalState::MoveState);

		SOUND(SoundResource::SE_MonsterCry3.c_str())->Play3D(owner->m_Transform.position, owner->m_Transform.m_pos_vec);
	}
}

void EnemyIdleState::Exit()
{
	owner->m_model.SetAnimationSpeed(1.0f);
}

void EnemyIdleState::Thinking()
{
	//もしプレイヤーとの距離が一定ならそれぞれの攻撃に入る
	owner->m_attack_type = owner->SelectAttackType();
	switch (owner->m_attack_type)
	{
	case FireElemental::AttackType::Strike:
	case FireElemental::AttackType::SlamGround:
		owner->m_attack_range = 20;
		break;
	case FireElemental::AttackType::ThrowStone:
		owner->m_attack_range = 40;
		break;
	default:
		break;
	}


}
