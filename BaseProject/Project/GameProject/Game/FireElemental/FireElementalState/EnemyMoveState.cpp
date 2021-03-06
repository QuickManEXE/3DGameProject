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
	//攻撃範囲に入るまで近づく
	Move();
}

void EnemyMoveState::Exit()
{
	SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Pause();
}

void EnemyMoveState::Move()
{
	//プレイヤーへの攻撃の種類によって攻撃方法が変わる

	CVector3D target_pos = Player::GetInstance()->m_Transform.position;
	target_pos.y = 0;
	CVector3D owner_pos = owner->m_Transform.position;
	owner_pos.y = 0;

	CVector3D dir = target_pos - owner_pos;

	if(Utility::IsViewInside(owner_pos, owner->m_Transform.rotation, target_pos, DtoR(10), owner->m_attack_range)) {

		SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Stop();
		printf("視界内に入っています%.0f\n", dir.Length());
		owner->m_stateAI.ChangeState(FireElementalState::AttackState);
	}
	else {

		float ang = atan2(dir.x, dir.z);

		rot_target = (CVector3D(0, ang, 0));
		//owner->transform.rotation = (CVector3D(0, ang, 0));
		//移動する
		float speed = 2.0f;

		owner->m_Transform.m_pos_vec += (dir.GetNormalize() * speed);

		owner->m_model.ChangeAnimation(2);

	}

	//★角度の補間
	//角度差分を求める（目標値-現在値）
	//角度差分を-π(-180)〜π(180)の範囲に整える
	float a = Utility::NormalizeAngle(rot_target.y - owner->m_Transform.rotation.y);

	//割合による補間
	//rot.y += a*0.1f;

	//一定量回転による角度補間
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

	//補間後は-π(-180)〜π(180)の範囲に角度を整える
	owner->m_Transform.rotation.y = Utility::NormalizeAngle(owner->m_Transform.rotation.y);

}





