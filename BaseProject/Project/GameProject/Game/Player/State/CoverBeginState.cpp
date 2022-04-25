#include "CoverBeginState.h"
#include"../../CameraManager.h"

CoverBeginState::CoverBeginState(Player* owner) : State(owner)
{
}

void CoverBeginState::Enter()
{
	owner->m_model_a3m.SetAnimationSpeed(2.0f);

	owner->m_model_a3m.ChangeAnimation(15, false);
}

void CoverBeginState::Execute()
{
	const float frame_max = owner->m_model_a3m.GetAnimationSize();

	if (owner->m_model_a3m.isAnimationEnd()) {

		owner->state_AI.ChangeState(PlayerState::CoverState);

	}

	//カメラの設定
	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();
}

void CoverBeginState::Exit()
{
	owner->m_model_a3m.SetAnimationSpeed(1.0f);
}

void CoverBeginState::Render()
{
	owner->StandardRender();
}

void CoverBeginState::CollisionCheck(CollisionTask* _task)
{
}
