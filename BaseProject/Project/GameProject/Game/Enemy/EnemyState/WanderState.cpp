#include "WanderState.h"
#include"../../Player/Player.h"



//徘徊ステート::ダイクストラで見つけたルートを徘徊する
WanderState::WanderState(Enemy* owner) : State(owner)
{
}

void WanderState::Enter()
{
	
	
}

void WanderState::Execute()
{
	
	if (owner->SearchPlayer()) {
		
		owner->state_AI.ChangeState(EnemyState::AttackState);

	}
	//もしルートが空なら新しい経路を作る
	if (route.empty()) {

		auto next = Dijkstra::center_pos + CVector3D(Utility::IsPlusMinus() * Utility::Rand(0, 10), 0, Utility::IsPlusMinus() * Utility::Rand(0, 10));
		route = owner->dijkstra.FindShortestPath(owner->transform.position, next);
		auto itr = route.end();
		itr--;
		next_pos = (*itr)->m_pos;

	}
	else {
		//ルートがあるなら移動する

		CVector3D dir = next_pos - owner->transform.position;

		if (dir.Length() >= 0.5f) {

			//距離があるなら
			//移動する
			float ang = atan2(dir.x, dir.z);
			owner->transform.rotation = (CVector3D(0, ang, 0));
			//owner->transform.Translate(dir.GetNormalize() * 1.0f * DELTA);
			owner->m_move_vec += dir.GetNormalize() * 30.0f * DELTA;

			owner->m_model_a3m.ChangeAnimation(18);
		}
		else {

			//距離が短くなったら

			//次のノードに切り替える
			//最初のノードを消す
			route.pop_back();

			if (!route.empty()) {

				auto itr = route.end();
				itr--;
				next_pos = (*itr)->m_pos;

			}
			
		}

	}
	


	
	owner->AddGravity();
	owner->AddMoveForce();

	owner->m_model_a3m.UpdateAnimation();
}

void WanderState::Exit()
{
}

void WanderState::Move()
{
}

void WanderState::Render()
{
	owner->StandardRender();

#ifdef _DEBUG


	owner->dijkstra.Draw();
	
	Utility::DrawViewPoint(owner->transform.position + CVector3D(0, 1.8, 0), owner->transform.rotation, DtoR(60), 5.0f);
		
	Utility::DrawLine(owner->transform.position + CVector3D(0, 1.8, 0)
		, Player::GetInstance()->GetCenterPoint()
		, CVector4D(0, 1, 0, 1));

	CVector3D line_posion = owner->transform.position + CMatrix::MRotation(owner->transform.rotation).GetFront() * 0.2f + CVector3D(0, 0.9f, 0);
	
	Utility::DrawLine(line_posion
		, line_posion - CVector3D(0, 5.0f, 0)
		, CVector4D(0, 1, 1, 1));


	Utility::DrawSphere(owner->clime_point,0.2f,CVector4D(0, 1, 0, 1));

#endif // _DEBUG
}

void WanderState::CollisionCheck(CollisionTask* task)
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
				
				//owner->HangCheck(task);

				//owner->CanFallCheck(task);

			}

			

			
		}
	}
}
