#include "ThrowRopeState.h"

ThrowRopeState::ThrowRopeState(Player* owner) : State(owner)
{
}

void ThrowRopeState::Enter()
{
	//アニメーションのスピードを変更する
	owner->m_model_a3m.SetAnimationSpeed(6.0f);
	//モデルのアニメーションを変える
	owner->m_model_a3m.ChangeAnimation(32, false);
	//ロープの最初の位置を設定
	rope_pos = owner->transform.position+CVector3D(0,owner->m_param.GetParam("height")/2,0);
	//変化量
	t = 0;

}

void ThrowRopeState::Execute()
{
	const float frame_max = 167;

	if(owner->m_model_a3m.GetAnimationFrame() >= 167){

		owner->HookRope(owner->rope_conect_point);

	}
	else if(owner->m_model_a3m.GetAnimationFrame() >= 64){

		
		t = min(1, t + 0.1f);

		rope_pos = rope_pos * (1-t)+ owner->rope_conect_point * t;

	}

	owner->m_model_a3m.UpdateAnimation();
}

void ThrowRopeState::Exit()
{
	owner->m_model_a3m.SetAnimationSpeed(1.0f);
}

void ThrowRopeState::Render()
{
	owner->StandardRender();
}

void ThrowRopeState::CollisionCheck(CollisionTask* task)
{
}
