#include "JumpState.h"
#include"../../Geometry.h"
#include "IdleState.h"

JumpState::JumpState(Player* owner) : State(owner)
{
}

void JumpState::Enter()
{
	float jump_power = 10.0f;
	//ベクトルにジャンプの力を加える
	owner->m_Transform.m_pos_vec.y += jump_power;// owner->m_Param.GetParam("jump_power");

	//ジャンプのアニメーションに切り替える
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Jump, false);

	rot_target = CVector3D::zero;
}

void JumpState::Execute()
{
	//キャラクターの向きに
	CVector3D dir(sin(owner->m_Transform.rotation.y), 0, cos(owner->m_Transform.rotation.y));

	//前方向に進む
	float speed = 10.0f;// owner->m_param.GetParam("move_speed");

	//移動ベクトルを加算
	owner->m_Transform.m_pos_vec += dir * speed * DELTA;

	AimMove();

	//owner->RopeSearch();

	//地面についたら
	if (owner->m_IsGround) {
		owner->m_StateAI.ChangeState(PlayerState::IdleState);
	}

	owner->AddGravity();
	owner->AddMoveForce();

	//モーションの更新
	owner->GetModel()->UpdateAnimation();
}

void JumpState::Move()
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
	float a = Utility::NormalizeAngle(rot_target.y - owner->m_Transform.rotation.y);

	//割合による補間
	//rot.y += a*0.1f;

	//一定量回転による角度補間
	float rot_speed = DtoR(360);
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

	//補間後は-π(-180)～π(180)の範囲に角度を整える
	owner->m_Transform.rotation.y = Utility::NormalizeAngle(owner->m_Transform.rotation.y);
	//--------------------------------------------------------


}

void JumpState::Exit()
{
}

void JumpState::AimMove()
{
	//マウス操作でキャラクターを回転----
	CVector2D mouse_vec = CInput::GetMouseVec();
	owner->m_Transform.rotation.y += mouse_vec.x * -0.01f;
	owner->m_Transform.rotation.x += mouse_vec.y * 0.01f;
	owner->m_Transform.rotation.x = min(DtoR(45), max(DtoR(-45), owner->m_Transform.rotation.x));


	//方向キーの入力方向ベクトル
	CVector3D key_dir(0, 0, 0);
	//キャラクターの移動量
	float speed = owner->m_Param.GetParam(Player::move_speed);
	int _anim_dir = 0;
	//方向キーから入力方向ベクトルを設定
	if (CInput::GetState(0, CInput::eHold, CInput::eUp)) {
		key_dir.z = 1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunForward : Player::PlayerAnimJam::WalkForward;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eDown)) {
		key_dir.z = -1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunBack : Player::PlayerAnimJam::WalkBack;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) {
		key_dir.x = 1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunLeft : Player::PlayerAnimJam::WalkLeft;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eRight)) {
		key_dir.x = -1;
		//_anim_dir = (is_run) ? Player::PlayerAnimJam::RunRight : Player::PlayerAnimJam::WalkRight;
	}

	CMatrix rotMtx = CMatrix::MRotationY(owner->m_Transform.rotation.y);

	//キー入力があれば
	if (key_dir.LengthSq() > 0) {
		//移動処理 回転行列×移動方向
		CVector3D dir = rotMtx * key_dir;
		owner->m_Transform.m_pos_vec += dir * speed * DELTA;;
		//owner->m_model_a3m.ChangeAnimation(_anim_dir);
	}
}

void JumpState::CollisionCheck(CollisionTask* task)
{
	
	owner->CollisionCheckToField(task);

}


