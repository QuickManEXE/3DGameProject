#include "HangRopeState.h"
#include"../../CameraManager.h"

HangRopeState::HangRopeState(Player* owner) : State(owner)
{

}

void HangRopeState::Enter()
{
	owner->m_model_a3m.ChangeAnimation(10);
}

void HangRopeState::Execute()
{
	//ロープがあるなら
	if (owner->rope) {

		RopeIdle();
		
		if (CInput::GetState(0, CInput::ePush, CInput::eButton5)) {
			//プレイヤーがロープを離すときの処理
			ReleaseRope();
		}

	}

	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();
}

void HangRopeState::Exit()
{
}

void HangRopeState::RopeIdle()
{
	//ロープの回転とプレイヤーの回転を合わせる
	owner->rope->transform.rotation.y = owner->transform.rotation.y;

	//入力
	if (CInput::GetState(0, CInput::eHold, CInput::eUp)) owner->rope->transform.m_vec.x -= DtoR(1) * DELTA;
	if (CInput::GetState(0, CInput::eHold, CInput::eDown)) owner->rope->transform.m_vec.x += DtoR(0.5) * DELTA;
	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) owner->transform.rotation.y += DtoR(1.0);
	if (CInput::GetState(0, CInput::eHold, CInput::eRight)) owner->transform.rotation.y -= DtoR(1.0);

	//重力を0にして
	owner->transform.m_vec.y = 0;

	//プレイヤーがつかまった地点を更新する
	owner->transform.position = owner->rope->GetConectPoint();

	owner->transform.rotation.x = owner->rope->transform.rotation.x;

}

void HangRopeState::ReleaseRope()
{
	//プレイヤーの左向き
	CVector3D player_left = CMatrix::MRotation(owner->transform.rotation).GetLeft();
	//ロープの始点
	CVector3D ropeS = owner->rope->GetLineS();

	//プレイヤーからロープの始点へのベクトル
	CVector3D up = ropeS - owner->rope->GetConectPoint();

	//外積を使い前方向を求める
	CVector3D front = CMatrix::MRotation(owner->transform.rotation).GetFront();

	CVector3D::Cross(player_left, up);

	//補正
	float correction = 20.0f; fabs(Utility::NormalizeAngle(owner->rope->transform.rotation.x));
	//プレイヤーの前方向に飛ばす
	owner->transform.m_vec += front;// *correction;//補正をかける

	owner->rope = nullptr;

	owner->transform.rotation.x = 0;

	owner->state_AI.ChangeState(PlayerState::BigJumpState);
}

void HangRopeState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}
