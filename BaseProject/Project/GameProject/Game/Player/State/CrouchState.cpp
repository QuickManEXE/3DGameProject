#include "CrouchState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"


CrouchState::CrouchState(Player* owner) : State(owner),is_crouch(false)
{

}

void CrouchState::Enter()
{
	is_crouch = false;

	owner->m_model_a3m.ChangeAnimation(11, false);
}

void CrouchState::Execute()
{
	if (is_crouch) {

		Move();

		//コントロールを押すとしゃがみモードを解除
		if (CInput::GetState(0, CInput::ePush, CInput::eButton9)) {

			owner->m_model_a3m.ChangeAnimation(14, false);
			owner->state_AI.ChangeState(PlayerState::IdleState);

		}

	}
	else {

		if (owner->m_model_a3m.GetAnimationFrame() >= 19) 
			is_crouch = true;

	}

	owner->AddGravity();
	owner->AddForce();

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();

	//カメラの設定
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);
}

void CrouchState::Move()
{
	//キャラクター操作
	//方向キーの入力方向ベクトル
	CVector3D key_dir(0, 0, 0);
	//カメラの方向ベクトル
	CVector3D cam_dir = CCamera::GetCurrent()->GetDir();
	//入力回転値
	float key_ang = 0;
	//カメラの回転値
	float cam_ang = atan2(cam_dir.x, cam_dir.z);

	key_dir = Utility::GetInputKeyDir();

	float speed = owner->m_param.GetParam("move_speed");

	//キー入力があれば
	if (key_dir.LengthSq() > 0) {

		//入力方向から入力回転値を計算
		key_ang = atan2(key_dir.x, key_dir.z);

		//カメラの回転値と入力回転値からキャラクターの回転値を決定
		//即座にrot.yに設定せず一旦目標値を設定する
		owner->transform.rotation.y = cam_ang + key_ang;

		//移動処理
		CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

		//移動ベクトルを加算する
		owner->transform.m_vec += dir * speed * DELTA;

		float run_speed = 10.0f;

	    owner->m_model_a3m.ChangeAnimation(13);
	}
	else {
		owner->m_model_a3m.ChangeAnimation(12);
	}
}

void CrouchState::Exit()
{
}

void CrouchState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void CrouchState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);	
}
