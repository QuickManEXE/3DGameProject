#include "BoxingState.h"
#include"../../CameraManager.h"

BoxingState::BoxingState(Player* owner) : State(owner)
{
}

void BoxingState::Enter()
{
	attack_cnt=0;

	attack_max = 1;

	owner->m_model_a3m.SetAnimationSpeed(2.5f);

	owner->m_model_a3m.ChangeAnimation(27,false);

	owner->m_Attack_Col.SetPause(false);

	owner->m_rand_id = rand();
}

void BoxingState::Execute()
{
	//const float frame_max = 55;
	
	if (owner->m_model_a3m.GetAnimationFrame() >= frame_max[attack_cnt]) {

		attack_cnt++;
		if (attack_cnt >= attack_max) {
			owner->m_stateAI.ChangeState(PlayerState::IdleState);
		}
		else {

			owner->m_model_a3m.ChangeAnimation(27 + attack_cnt, false);

		}

	}

	//左マウスをクリックするとコンボする
	if (CInput::GetState(0, CInput::ePush, CInput::eMouseL)) {
		
		attack_max = min(2,attack_max++);

	}


	CameraManager::GetInstance()->ControllCamera(owner->transform.position);

	//モーションの更新
	owner->m_model_a3m.UpdateAnimation();

}

void BoxingState::Exit()
{
	owner->m_Attack_Col.SetPause(true);

	owner->m_model_a3m.SetAnimationSpeed(1.0f);
}

void BoxingState::Render()
{
	owner->StandardRender();

	CVector3D s = owner->m_model_a3m.GetFrameMatrix(born_to_be_born[attack_cnt][0]).GetPosition();
	CVector3D e = owner->m_model_a3m.GetFrameMatrix(born_to_be_born[attack_cnt][1]).GetPosition();
	
	Utility::DrawCapsule(s, e, 0.1f,CVector4D(0.5,0.2,0.7,0.4));
}

void BoxingState::CollisionCheck(CollisionTask* _task)
{
}
