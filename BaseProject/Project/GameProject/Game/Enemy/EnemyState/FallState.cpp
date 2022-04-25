#include "FallState.h"

FallState::FallState(Enemy* owner) : State(owner),is_fall(false)
{
}

void FallState::Enter()
{
	
	is_fall = false;

	owner->m_model_a3m.ChangeAnimation(7);

}

void FallState::Execute()
{
	const float frame_check_point = 32;
	
	const float frame_max = 58;

	const float anim_frame = owner->m_model_a3m.GetAnimationFrame();


	owner->AddGravity();
	owner->AddMoveForce();

	if (is_fall) {

		if(anim_frame>=frame_max)
		//owner->state_AI.ChangeState(EnemyState::WanderState);
		owner->m_model_a3m.UpdateAnimation();

	}
	else if(anim_frame<=frame_check_point){

		owner->m_model_a3m.UpdateAnimation();

	}
}

void FallState::Exit()
{
}

void FallState::Render()
{
	owner->StandardRender();
}

void FallState::CollisionCheck(CollisionTask* task)
{
	//通常状態
	switch (task->GetPriority())
	{
		//静的なオブジェクトとの判定
	case  CollisionPriority::eCol_Field:
		if (GameObject* b = dynamic_cast<GameObject*> (task->GetTask())) {

			CVector3D d = b->transform.position - owner->transform.position;

			if (d.Length() <= b->GetModel().GetMax().Length()) {
				//このキャラクターとオブジェクトとの当たり判定
				{

					//衝突データ格納用
					CCollTriangle out[256];
					//押し戻し量
					CVector3D v(0, 0, 0);

					//カプセルとモデルとの衝突
					int cnt = b->GetModel().CollisionCupsel(out, owner->transform.position + CVector3D(0, owner->m_rad, 0),
						owner->transform.position + CVector3D(0, 1.8 - owner->m_rad, 0), owner->m_rad, 250);
					//接触した面の数繰り返す
					for (int i = 0; i < cnt; i++) {

						//接触した面が斜めより上を向いていたら地面
						if (out[i].m_normal.y > 0.5f) {
							//重力落下速度を0に戻す
							owner->m_move_vec.y = 0;

							is_fall = true;
						}
						float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
						//接触した面の方向へ、めり込んだ分押し戻す
						CVector3D nv = out[i].m_normal * (owner->m_rad - out[i].m_dist);
						//最も大きな移動量を求める
						v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
						//膝下までは乗り越える
						//膝上以上の壁のみ押し戻される
						if (max_y > owner->transform.position.y + 0.5f) {
							v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
							v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
						}

					}
					//押し戻す
					owner->transform.position += v;
				}
			}

			//owner->HangCheck(task);

		}
	}
}
