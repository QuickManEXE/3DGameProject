#include "JumpAttackState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"

JumpAttackState::JumpAttackState(Player* owner) : State(owner)
{
}

void JumpAttackState::Enter()
{
	owner->m_model_a3m.SetAnimationSpeed(1.5f);
	//ジャンプのアニメーションに切り替える
	owner->m_model_a3m.ChangeAnimation(31, false);
}

void JumpAttackState::Execute()
{
	const float frame_max = 45;

	if (owner->m_model_a3m.GetAnimationFrame() >= frame_max) {

		owner->state_AI.ChangeState(PlayerState::IdleState);

	}

	Move();

	//キャラクターの向きに
	CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

	//前方向に進む
	float speed = owner->m_param.GetParam("move_speed");

	//移動ベクトルを加算
	owner->transform.m_vec += dir * speed * DELTA;

	owner->AddGravity();
	owner->AddForce();

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();

	CameraManager::GetInstance()->ControllCamera(owner->transform.position);
}

void JumpAttackState::Move()
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

	//キー入力があれば
	if (key_dir.LengthSq() > 0) {

		//入力方向から入力回転値を計算
		key_ang = atan2(key_dir.x, key_dir.z);

		//カメラの回転値と入力回転値からキャラクターの回転値を決定
		//即座にrot.yに設定せず一旦目標値を設定する
		//owner->transform.rotation.y = cam_ang + key_ang;
		rot_target.y = cam_ang + key_ang;
	}

	//------------------------------------------------------
		//★角度の補間
		//角度差分を求める（目標値-現在値）
		//角度差分を-π(-180)～π(180)の範囲に整える
	float a = Utility::NormalizeAngle(rot_target.y - owner->transform.rotation.y);

	//割合による補間
	//rot.y += a*0.1f;

	//一定量回転による角度補間
	float rot_speed = DtoR(360);
	if (a > rot_speed) {
		owner->transform.rotation.y += rot_speed * DELTA;
	}
	else
		if (a < -rot_speed) {
			owner->transform.rotation.y -= rot_speed * DELTA;
		}
		else {
			owner->transform.rotation.y += a * DELTA;
		}

	//補間後は-π(-180)～π(180)の範囲に角度を整える
	owner->transform.rotation.y = Utility::NormalizeAngle(owner->transform.rotation.y);
	//--------------------------------------------------------


}

void JumpAttackState::Exit()
{
	owner->m_model_a3m.SetAnimationSpeed(1.0f);
}

void JumpAttackState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void JumpAttackState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
