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
	//UŒ‚”ÍˆÍ‚É“ü‚é‚Ü‚Å‹ß‚Ã‚­
	Move();
}

void EnemyMoveState::Exit()
{
	SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Pause();
}

void EnemyMoveState::Move()
{
	//ƒvƒŒƒCƒ„[‚Ö‚ÌUŒ‚‚ÌŽí—Þ‚É‚æ‚Á‚ÄUŒ‚•û–@‚ª•Ï‚í‚é

	CVector3D target_pos = Player::GetInstance()->m_Transform.position;
	target_pos.y = 0;
	CVector3D owner_pos = owner->m_Transform.position;
	owner_pos.y = 0;

	CVector3D dir = target_pos - owner_pos;

	if(Utility::IsViewInside(owner_pos, owner->m_Transform.rotation, target_pos, DtoR(10), owner->m_attack_range)) {

		SOUND(SoundResource::SE_MonsterFootSteps.c_str())->Stop();
		printf("Ž‹ŠE“à‚É“ü‚Á‚Ä‚¢‚Ü‚·%.0f\n", dir.Length());
		owner->m_stateAI.ChangeState(FireElementalState::AttackState);
	}
	else {

		float ang = atan2(dir.x, dir.z);

		rot_target = (CVector3D(0, ang, 0));
		//owner->transform.rotation = (CVector3D(0, ang, 0));
		//ˆÚ“®‚·‚é
		float speed = 2.0f;

		owner->m_Transform.m_pos_vec += (dir.GetNormalize() * speed);

		owner->m_model.ChangeAnimation(2);

	}

	//šŠp“x‚Ì•âŠÔ
	//Šp“x·•ª‚ð‹‚ß‚éi–Ú•W’l-Œ»Ý’lj
	//Šp“x·•ª‚ð-ƒÎ(-180)`ƒÎ(180)‚Ì”ÍˆÍ‚É®‚¦‚é
	float a = Utility::NormalizeAngle(rot_target.y - owner->m_Transform.rotation.y);

	//Š„‡‚É‚æ‚é•âŠÔ
	//rot.y += a*0.1f;

	//ˆê’è—Ê‰ñ“]‚É‚æ‚éŠp“x•âŠÔ
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

	//•âŠÔŒã‚Í-ƒÎ(-180)`ƒÎ(180)‚Ì”ÍˆÍ‚ÉŠp“x‚ð®‚¦‚é
	owner->m_Transform.rotation.y = Utility::NormalizeAngle(owner->m_Transform.rotation.y);

}





