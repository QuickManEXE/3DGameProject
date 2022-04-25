#include "GodState.h"
#include"../../CameraManager.h"

GodState::GodState(Player* owner) : State(owner)
{
}

void GodState::Enter()
{
}

void GodState::Execute()
{
	Move();

	
	owner->AddGravity();
	//座標に変化量を加える
	owner->AddForce();


	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();

	//カメラの設定
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);
}

void GodState::Exit()
{
}

void GodState::Move()
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

		owner->m_model_a3m.ChangeAnimation(2);
	}
	else {
		owner->m_model_a3m.ChangeAnimation(1);
	}

	//左クリックを押すと攻撃に移行
	if (CInput::GetState(0, CInput::eHold, CInput::eButton5)) {

		owner->transform.m_vec.y += owner->m_param.GetParam("jump_power")*5.0f * DELTA;
	}
}

void GodState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
