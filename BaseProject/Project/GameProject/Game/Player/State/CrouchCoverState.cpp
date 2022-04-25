#include "CrouchCoverState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"

CrouchCoverState::CrouchCoverState(Player* owner) : State(owner),is_cover(false),is_right(true)
{
}

void CrouchCoverState::Enter()
{
	is_right = true;

	is_cover = false;

	owner->m_model_a3m.ChangeAnimation(23, false);
}

void CrouchCoverState::Execute()
{
	if (is_cover) {

		Move();

		//下を押すとカバーモードを解除
		if (CInput::GetState(0, CInput::ePush, CInput::eDown)) {

			owner->m_model_a3m.ChangeAnimation(14, false);
			owner->state_AI.ChangeState(PlayerState::CrouchState);

		}


	}
	else {
		if (owner->m_model_a3m.GetAnimationFrame() >= 19) is_cover = true;
	}

	owner->AddGravity();

	owner->m_flag.SetParam("is_ground", false);
	//カメラの設定
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();
}

void CrouchCoverState::Move()
{
	float speed = owner->m_param.GetParam("move_speed") / 4;

	//壁に平行移動するための壁の法線
	CVector3D normal = owner->cover_wall_normal;

	//法線の角度を出す
	float ang = atan2(normal.x, normal.z);

	//右方向ベクトル
	CVector3D right_dir = CVector3D(sin(ang + DtoR(90)), 0, cos(ang + DtoR(90)));

	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) {

		owner->transform.m_vec -= right_dir * speed;
		owner->m_model_a3m.ChangeAnimation(26);
		is_right = false;
	}
	else if (CInput::GetState(0, CInput::eHold, CInput::eRight)) {

		owner->transform.m_vec += right_dir * speed;
		owner->m_model_a3m.ChangeAnimation(24);
		is_right = true;
	}
	else {
		if (is_right)
			owner->m_model_a3m.ChangeAnimation(23);
		else owner->m_model_a3m.ChangeAnimation(25);
	}
}

void CrouchCoverState::Exit()
{
}

void CrouchCoverState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void CrouchCoverState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
