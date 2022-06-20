#include "IdleState.h"
#include"../../NonAnimObject.h"
#include"../../RopeConnectPoint.h"
#include"../../Rope/Rope.h"

IdleState::IdleState(Player* owner) : State(owner),is_run(false),is_hold(false){}

void IdleState::Enter()
{
	Camera::GetInstance()->SetCamera(owner);
	if (owner->mp_WireRope) owner->mp_WireRope->SetKill();
}

void IdleState::Execute()
{
	is_run = false;

	Move();

	//左クリックを押すと攻撃に移行
	if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {

		owner->m_StateAI.ChangeState(PlayerState::ArrowAimState);

	}
	//右クリックを長押しでワイヤーエイムモードに移行
	if (CInput::GetState(0, CInput::eHold, CInput::eMouseR)&&owner->m_HookNum>0) {

		owner->m_StateAI.ChangeState(PlayerState::WireAimState);
	}

	//もし地面に接していないのが続けば（ベクトルの大きさが一定となればジャンプする)
	//走っているなら
	/*if (is_run) {
		if(owner->m_IsJump)owner->m_StateAI.ChangeState(PlayerState::JumpState);
	}*/

	//フックモード切り替え
	if (CInput::GetState(0, CInput::ePush, CInput::eButton3))
		owner->m_IsHook = (owner->m_IsHook)? false : true;

	//重力をかける
	owner->AddGravity();
	//座標に変化量を加える
	owner->AddMoveForce();

	//モーションの更新
	owner->GetModel()->UpdateAnimation();
}

void IdleState::Exit()
{
}

void IdleState::Move()
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
	//もしSHIFTを押したなら走る
	if (HOLD(CInput::eButton8)) {
		is_run = true;
		speed *= 2.0f;
	}
	int _anim_dir = 0;
	//方向キーから入力方向ベクトルを設定
	if (CInput::GetState(0, CInput::eHold, CInput::eUp)) {
		key_dir.z = 1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunForward : Player::PlayerAnimJam::WalkForward;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eDown)) {
		key_dir.z = -1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunBack : Player::PlayerAnimJam::WalkBack;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) {
		key_dir.x = 1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunLeft : Player::PlayerAnimJam::WalkLeft;
	}
	if (CInput::GetState(0, CInput::eHold, CInput::eRight)) {
		key_dir.x = -1;
		_anim_dir = (is_run)? Player::PlayerAnimJam::RunRight : Player::PlayerAnimJam::WalkRight;
	}

	CMatrix rotMtx = CMatrix::MRotationY(owner->m_Transform.rotation.y);

	//キー入力があれば
	if (key_dir.LengthSq() > 0) {
		//移動処理 回転行列×移動方向
		CVector3D dir = rotMtx * key_dir;
		owner->m_Transform.m_pos_vec += dir * speed * DELTA;;
		owner->GetModel()->ChangeAnimation(_anim_dir);
	}
	else {
		owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Idle);
	}
}

void IdleState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void IdleState::CollisionCheck(CollisionTask* task)
{

	owner->CollisionObject(task);

	owner->HangCheck(task);

	owner->CollisionCharacter(task);

	owner->CollisionIsJump(task);
}
