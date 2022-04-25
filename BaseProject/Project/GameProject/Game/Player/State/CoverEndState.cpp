#include "CoverEndState.h"
#include"../../CameraManager.h"

CoverEndState::CoverEndState(Player* owner) : State(owner)
{
}

void CoverEndState::Enter()
{
	owner->m_model_a3m.SetAnimationSpeed(2.0f);

	owner->m_model_a3m.ChangeAnimation(18, false);
}

void CoverEndState::Execute()
{
	const float frame_max = 42;

	if (owner->m_model_a3m.GetAnimationFrame() >= frame_max) {

		owner->state_AI.ChangeState(PlayerState::IdleState);

	}

	owner->m_flag.SetParam("is_ground", false);
	//カメラの設定
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();
}

void CoverEndState::Exit()
{
	owner->m_model_a3m.SetAnimationSpeed(1.0f);
}

void CoverEndState::Render()
{
	owner->StandardRender();
}

void CoverEndState::CollisionCheck(CollisionTask* _task)
{
}
