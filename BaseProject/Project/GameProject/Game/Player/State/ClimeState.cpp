#include "ClimeState.h"


ClimeState::ClimeState(Player* owner) : State(owner)
{
}

void ClimeState::Enter()
{
	//アニメーションのスピードを変更
	owner->GetModel()->SetAnimationSpeed(2.0f);
	//壁のぼりアニメーションに移行
	owner->GetModel()->ChangeAnimation(Player::PlayerAnimJam::Clime, false);
}

void ClimeState::Execute()
{

	//壁のぼりのアニメーションの総フレーム数
	float frame_max = 116;
	//壁を登りだす始まりのフレーム数
	float frame_clime_begin = 9;
	//壁のぼりの高さが合うフレーム数
	float frame_clime = 70;
	//アニメーションがおわったら
	if (owner->GetModel()->isAnimationEnd()) {

		//上った地点に合わせる
		owner->m_Transform.position = owner->m_ClimePoint;
		//アイドルステートにもどる
		owner->m_StateAI.ChangeState(PlayerState::IdleState);

	}
	else {
		//アニメーションが終わってなければ

		//登り始めるフレームまでは何もしない
		if (owner->GetModel()->GetAnimationFrame() <= frame_clime_begin) {
			


		}else //登り始めたら最終の登る地点まで座標を遷移していく
		if (owner->GetModel()->GetAnimationFrame() <= frame_clime) {
			//アニメーションの進行度
			float t = min(1, (owner->GetModel()->GetAnimationFrame() - frame_clime_begin) / frame_clime);

			owner->m_Transform.position.y = 
				owner->m_Transform.position.y * (1 - t) + ((owner->m_ClimePoint.y - owner->m_Param.GetParam("height")/2) * t);
		}
		else {

			//登り切ったアニメーションのフレームになったらXとYの座標も合わせていく
			//アニメーションの進行度
			float t = min(1, (owner->GetModel()->GetAnimationFrame()-frame_clime) / frame_max);

			owner->m_Transform.position =
				owner->m_Transform.position * (1 - t) + ((owner->m_ClimePoint) * t);
		}
	}
	
	//モーションの更新
	owner->GetModel()->UpdateAnimation();
}

void ClimeState::Exit()
{
	//アニメーションの速度を戻しておく
	owner->GetModel()->SetAnimationSpeed(1.0f);
}

void ClimeState::Render()
{
	//キャラクター表示
	owner->StandardRender();
}
