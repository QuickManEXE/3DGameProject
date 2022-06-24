#include "ArrowAimState.h"
#include"../../Arrow.h"
#include"../../../SoundResource.h"


ArrowAimState::ArrowAimState(Player* owner) : State(owner)
{

}

void ArrowAimState::Enter()
{
	//弓矢を引き絞る音を鳴らす
	SOUND(SoundResource::SE_ShotArrowStart.c_str())->Play3D(owner->m_Transform.position,CVector3D::zero);

	owner->m_AimState = 0;

	float speed = owner->GetAttackSpeed();
	//アニメーションのスピードを変更
	owner->GetModel()->SetAnimationSpeed(speed);
}

void ArrowAimState::Execute()
{

	Aim();

	owner->AddGravity();
	owner->AddMoveForce();

	//モーションの更新
	owner->GetModel()->UpdateAnimation();
}

void ArrowAimState::Exit()
{
	owner->GetModel()->SetAnimationSpeed(1.0f);
}

void ArrowAimState::CollisionCheck(CollisionTask* _task)
{
	owner->CollisionObject(_task);
}

void ArrowAimState::Aim()
{
	//マウス操作でキャラクターを回転----
	CVector2D mouse_vec = CInput::GetMouseVec();
	owner->m_Transform.rotation.y += mouse_vec.x * -0.01f;
	owner->m_Transform.rotation.x += mouse_vec.y * 0.01f;
	owner->m_Transform.rotation.x = min(DtoR(60), max(DtoR(-60), owner->m_Transform.rotation.x));

	//矢の向きを変える
	if (Arrow* a = dynamic_cast<Arrow*>(owner->mp_Arrow)) {
		a->SetDir(CMatrix::MRotation(owner->m_Transform.rotation).GetFront());
	}
	switch (owner->m_AimState) {
	case 0:
		//弓を弾くアニメーションに変える
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::AimReady, false);
		//矢がなければ生成する
		if (!owner->mp_Arrow && owner->GetModel()->GetAnimationFrame() > 10) {
			Arrow* a = new Arrow();
			owner->mp_Arrow = a;
			static CVector3D v;
			a->SetParent(owner->GetModel(), 50);
		}
		if (!owner->GetModel()->isAnimationEnd()) break;
		owner->m_AimState++;
		break;
	case 1:
		//弓矢を構えるアニメーションに移行する
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::AimIdle);
		//左クリックを離すと発射
		if (HOLD(CInput::eMouseL)) break;
		owner->m_AimState++;
		break;
	case 2:
		if (Arrow* a = dynamic_cast<Arrow*>(owner->mp_Arrow)) {
			a->Shot(1.0f);
			owner->mp_Arrow = false;
			SOUND(SoundResource::SE_ShotArrowEnd.c_str())->Play3D(a->m_Transform.position,a->m_Transform.m_pos_vec);

		}
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Shot, false);
		if (!owner->GetModel()->isAnimationEnd()) break;
		owner->m_AimState = 0;

		if (!HOLD(CInput::eMouseL))
			owner->m_StateAI.ChangeState(PlayerState::IdleState);
		break;

	}

}
