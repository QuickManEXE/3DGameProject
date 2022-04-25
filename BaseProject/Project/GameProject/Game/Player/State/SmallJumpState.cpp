#include "SmallJumpState.h"
#include"../../CameraManager.h"
#include"../../Geometry.h"

SmallJumpState::SmallJumpState(Player* owner) : State(owner)
{
}

void SmallJumpState::Enter()
{
	printf("大ジャンプ開始！\n");
	//ベクトルにジャンプの力を加える
	owner->transform.m_vec.y += owner->m_param.GetParam("jump_power");
	//ジャンプのアニメーションに切り替える
	owner->m_model_a3m.ChangeAnimation(4, false);
}

void SmallJumpState::Execute()
{
	//キャラクターの向きに
	CVector3D dir(sin(owner->transform.rotation.y), 0, cos(owner->transform.rotation.y));

	//前方向に進む
	float speed = owner->m_param.GetParam("move_speed") * 10.0f;

	//移動ベクトルを加算
	owner->transform.m_vec += dir * speed * DELTA;

	//地面についたら
	if (owner->m_IsGround) {
		owner->state_AI.ChangeState(PlayerState::IdleState);
	}

	owner->AddGravity();

	owner->m_flag.SetParam("is_ground", false);
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();
}

void SmallJumpState::Exit()
{
}

void SmallJumpState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void SmallJumpState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionCheckToField(task);
}
