#include "SwingJumpState.h"
#include"../../Geometry.h"
#include"../../CameraManager.h"

SwingJumpState::SwingJumpState(Player* owner) : State(owner)
{
}

void SwingJumpState::Enter()
{
	printf("スィングジャンプ開始！\n");
	//ジャンプのアニメーションに切り替える
	owner->m_model_a3m.ChangeAnimation(4, false);
}

void SwingJumpState::Execute()
{
	//キャラクターの向きに
	CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

	//前方向に進む
	float speed = owner->m_param.GetParam("move_speed") * 2.0f;

	//移動ベクトルを加算
	owner->transform.m_vec += dir * speed * DELTA;

	owner->RopeSearch();

	//地面についたら
	if (owner->m_IsGround) {

		owner->state_AI.ChangeState(PlayerState::IdleState);

	}

	//マウス左ボタンをクリックすると
	if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {
		//ジャンプアタックステートに移行
		//owner->state_AI.ChangeState(PlayerState::JumpAttackState);

	}

	
	owner->AddGravity();
	owner->AddForce();

	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();
}

void SwingJumpState::Exit()
{
}

void SwingJumpState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void SwingJumpState::CollisionCheck(CollisionTask* task)
{
	
	owner->CollisionCheckToField(task);

	//owner->CollisionRope(task);	
}
