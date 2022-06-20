#include "DeathState.h"
#include"../../../SoundResource.h"
#include"../../GameManager/GameManager.h"

DeathState::DeathState(Enemy* owner) : State(owner),rand_jam(0)
{
}

void DeathState::Enter()
{

	owner->GetModel()->ChangeAnimation(16, false);

	SOUND(SoundResource::SE_MonsterCry2.c_str())->Play3D(owner->m_Transform.position, owner->m_Transform.m_pos_vec);

	for (Arrow* a : owner->m_ArrowVector) {

		a->SetKill();

	}

	owner->m_ArrowVector.clear();

	GameManager::Instance().m_EnemyCount--;
}

void DeathState::Execute()
{
	const float frame_max = damage_anim[rand_jam].frame_max;

	const float anim_frame = owner->GetModel()->GetAnimationFrame();

	if (owner->GetModel()->isAnimationEnd()) {

		owner->GetModel()->GetMaterial(0)->m_alpha -= DELTA;
		
		if (owner->GetModel()->GetMaterial(0)->m_alpha <= 0) {

			
			owner->SetKill();
			
		}

	}


	owner->AddGravity();
	owner->AddMoveForce();

	owner->GetModel()->UpdateAnimation();
}

void DeathState::Exit()
{
}

void DeathState::Render()
{
	owner->StandardRender();
}

void DeathState::CollisionCheck(CollisionTask* task)
{
	//通常状態
	switch (task->GetPriority())
	{
		//静的なオブジェクトとの判定
	case  CollisionPriority::eCol_Field:
		if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (task->GetTask())) {

			CVector3D d = b->m_Transform.position - owner->m_Transform.position;

			if (d.Length() <= b->GetModel().GetMax().Length()) {
				//このキャラクターとオブジェクトとの当たり判定
				{

					//衝突データ格納用
					CCollTriangle out[256];
					//押し戻し量
					CVector3D v(0, 0, 0);

					//カプセルとモデルとの衝突
					int cnt = b->GetModel().CollisionCupsel(out, owner->m_Transform.position + CVector3D(0, owner->m_rad, 0),
						owner->m_Transform.position + CVector3D(0, 1.8 - owner->m_rad, 0), owner->m_rad, 250);
					//接触した面の数繰り返す
					for (int i = 0; i < cnt; i++) {

						//接触した面が斜めより上を向いていたら地面
						if (out[i].m_normal.y > 0.5f) {
							//重力落下速度を0に戻す
							owner->m_Transform.m_pos_vec.y = 0;

						}
						float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
						//接触した面の方向へ、めり込んだ分押し戻す
						CVector3D nv = out[i].m_normal * (owner->m_rad - out[i].m_dist);
						//最も大きな移動量を求める
						v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
						//膝下までは乗り越える
						//膝上以上の壁のみ押し戻される
						if (max_y > owner->m_Transform.position.y + 0.5f) {
							v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
							v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
						}

					}
					//押し戻す
					owner->m_Transform.position += v;
				}
			}
		}
	}
}
