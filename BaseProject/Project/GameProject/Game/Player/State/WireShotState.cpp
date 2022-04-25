#include "WireShotState.h"
#include"../../Arrow.h"


WireShotState::WireShotState(Player* owner) : State(owner)
{
}

void WireShotState::Enter()
{

	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Shot);

}

void WireShotState::Execute()
{
	

	//アニメ―ションが終わるまで
	if (owner->GetModel()->isAnimationEnd()) {
		
		/*
		CVector3D dir =CameraManager::GetInstance()->GetAt() - CameraManager::GetInstance()->GetPos();
		new Arrow(Transform(owner->m_model_a3m.GetFrameMatrix("LeftHand").GetPosition(),
			CameraManager::GetInstance()->GetRot(),
			CVector3D(1.5, 1.5, 1.5)), dir.GetNormalize(), 
			100.0f);
			*/
		owner->m_StateAI.ChangeState(PlayerState::WireActionState);

	}

	owner->AddGravity();
	owner->AddMoveForce();


	//モーションの更新
	owner->GetModel()->UpdateAnimation();

	
}

void WireShotState::Exit()
{
}

void WireShotState::CollisionCheck(CollisionTask* task)
{
	owner->CollisionObject(task);
}