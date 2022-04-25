#include "HangWallState.h"


HangWallState::HangWallState(Player* owner) : State(owner)
{
}

void HangWallState::Enter()
{
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::HangWall, false);

	owner->GetModel()->SetAnimationSpeed(3.0f);

	owner->m_Transform.m_pos_vec = CVector3D::zero;
}

void HangWallState::Execute()
{
	
	//壁つかまりのアニメーションの総フレーム数
	int frame_max = owner->GetModel()->GetAnimationSize();

	//アニメ―ションが終わるまで
	
	if (owner->GetModel()->isAnimationEnd()) {

		//上を押すと登る
		if (CInput::GetState(0, CInput::ePush, CInput::eUp)) {
			owner->m_StateAI.ChangeState(PlayerState::ClimeState);
		}

		//下を押すと降りる
		if (CInput::GetState(0, CInput::ePush, CInput::eDown)) {
			owner->m_StateAI.ChangeState(PlayerState::HangDropState);
		}

	}
	else {

		//アニメーションの進行度
		float t = min(0.5,owner->GetModel()->GetAnimationFrame() / frame_max);

		CVector3D clime_pos = owner->m_ClimePoint; 
		clime_pos.y -= owner->m_Param.GetParam("height");

		owner->m_Transform.position = owner->m_Transform.position*(1-t) + clime_pos *t;
	}

	//モーションの更新
	owner->GetModel()->UpdateAnimation();

}

void HangWallState::Exit()
{
	owner->GetModel()->SetAnimationSpeed(1.0f);
}

void HangWallState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}

void HangWallState::CollisionCheck(CollisionTask* task)
{

	owner->CollisionObject(task);
}


