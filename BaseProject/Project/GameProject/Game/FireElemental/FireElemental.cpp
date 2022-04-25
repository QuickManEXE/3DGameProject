#include "FireElemental.h"
#include"../Player/Player.h"
#include"FireElementalState/EnemyIdleState.h"
#include"FireElementalState/EnemyMoveState.h"
#include"FireElementalState/FireElementalAttackState.h"
#include"FireElementalState/FireElementalDeathState.h"
#include"../Arrow.h"
#include"../../Effect/EffectCollection.h"
#include"../../SoundResource.h"


FireElemental::Index FireElemental::m_culsule_idx[m_cupsule_max] = {
{4,5,2.0f},		//胴体
{6,7,2.9f},		//頭
{8,10,1.6f},	//左腕
{10,22,1.4f},	//左手
{32,34,1.6f},	//右腕
{34,46,1.4f},	//右手
{56,57,1.4f},	//左もも
{57,58,1.4f},	//左足
{62,63,1.4f},	//右もも
{63,64,1.4f},	//右足
};

FireElemental::FireElemental(const Transform& transform, const char* model_name) 
	: Base(UpdatePriority::eUp_Enemy,"FireElemental")
{
	m_model = COPY_RESOURCE(model_name, CModelA3M);

	//座標設定
	this->m_Transform = transform;

	m_rad = 0.2f;

	m_hp = 10;

	// 描画タスクに登録
	m_render.RegistRender(this, (RenderFunc)&FireElemental::Render, RenderPriority::eRd_Enemy, "FireElemental");

	// 衝突判定タスクに登録
	m_col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&FireElemental::CollisionCheck, CollisionPriority::eCol_Enemy, "FireElemental");

	// 衝突判定タスクに登録
	m_attackCol.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&FireElemental::AttackCollide, CollisionPriority::eCol_Enemy, "FireElemental");


	m_stateAI.state_vector.push_back(new EnemyIdleState(this));
	m_stateAI.state_vector.push_back(new EnemyMoveState(this));
	m_stateAI.state_vector.push_back(new FireElementalAttackState(this));
	m_stateAI.state_vector.push_back(new FireElementalDeathState(this));

	m_stateAI.stateMachine = new StateMachine<FireElemental>;

	m_stateAI.ChangeState(FireElementalState::IdleState);

	

	m_is_attack = false;
}

FireElemental::~FireElemental()
{
}

void FireElemental::Update()
{
	m_stateAI.StateUpdate();

	AddGravity();
	AddMoveForce();

	m_model.UpdateAnimation();

	UpdateCollider();
}

void FireElemental::Render()
{
	StandardRender();

	/*
	if (!m_is_attack)return;
	for (int i = 0; i < m_cupsule_max; i++) {
		Utility::DrawCapsule(m_cupsule[i].s, m_cupsule[i].e, m_cupsule[i].r, CVector4D(0, 1, 0, 1));
	}
	*/
	
	//FONT_T()->Draw(100, 100, 0, 0, 0, "距離%.2f", (Player::GetInstance()->transform.position - transform.position).Length());
}

void FireElemental::CollisionCheck(CollisionTask* _task)
{
	GameObjectCollisionCheck(_task);

	if(_task->GetPriority()==CollisionPriority::eCol_Player_Attack)
	CollisionArrow(_task);
	//state_AI.StateCollisionCheck(_task);
}

void FireElemental::AddGravity()
{
	//重力をかける
	m_Transform.m_pos_vec.y -= GRAVITY * DELTA;

}

void FireElemental::StandardRender()
{
	//キャラクター表示
	m_model.SetPos(m_Transform.position);
	m_model.SetRot(m_Transform.rotation);
	m_model.SetScale(m_Transform.scale);
	m_model.Render();

	
}

bool FireElemental::SearchPlayer()
{
	//視界に何かが入っていたら
	CVector3D view_pos = m_Transform.position + CVector3D(0, 1.8, 0);
	CVector3D view_rot = m_Transform.rotation;
	CVector3D target_pos = Player::GetInstance()->GetCenterPoint();
	if (Utility::IsViewInside(view_pos, view_rot, target_pos, DtoR(60.0f), 5.0f)) {

		//printf("視界内に入っています\n");
		Utility::DrawLine(view_pos, target_pos, CVector4D(0, 1, 0, 1));
		//ターゲットとプレイヤーの視線の先に障害物がないか調べる

		CVector3D dir = target_pos - view_pos;
		float length = 5.0f;
		std::vector<CollisionTask*> buff = CollisionTaskManager::GetInstance()->GetCollisionList();

		if (buff.size() > 0) {

			bool is_view = true;

			//buffの中身で当たり判定
			for (CollisionTask* c : buff) {


				if (c->GetPriority() == CollisionPriority::eCol_Field) {

					if (NonAnimObject* g = dynamic_cast<NonAnimObject*> (c->GetTask())) {

						if (CCollision::CollisionShpere(g->m_Transform.position, 0,
							m_Transform.position, 10.0f)) {
							//やっとここで視線がさえぎられていないか判定
							//接触点と法線
							CVector3D cross, normal;
							if (g->GetModel().CollisionRay(&cross, &normal, view_pos, target_pos)) {

								is_view = false;

							}
						}

					}


				}



			}

			if (is_view) return true;
			else return false;

		}


	}
}

void FireElemental::AttackCollide(CollisionTask* task)
{

	//攻撃中ではなければ判定しない
	if (!m_is_attack)return;
	//もしプレイヤーなら判定する
	if (task->GetPriority() != CollisionPriority::eCol_Player)return;
	//プレイヤーに変換
	if (Player* p = dynamic_cast<Player*> (task->GetTask())) {

		CVector3D enemy_cup_s = m_model.GetFrameMatrix(m_culsule_idx[5].s).GetPosition();
		CVector3D enemy_cup_e = m_model.GetFrameMatrix(m_culsule_idx[5].e).GetPosition();

		//エネミーの腕とプレイヤーの体のカプセル同士で判定
		if (!p->m_StateAI.IsCurrentState(PlayerState::DeathState) && !p->m_StateAI.IsCurrentState(PlayerState::DamageState) && CCollision::CollisionCapsule(enemy_cup_s, enemy_cup_e, m_culsule_idx[5].r,
			p->m_Capsule.m_start, p->m_Capsule.m_end, p->m_Capsule.m_rad)) {

			//当たってるよ
			printf("当たってるよ\n");
			p->m_HitPoint -= 30;
				
			if (p->m_HitPoint <= 0) {
				p->m_StateAI.ChangeState(PlayerState::DeathState);
			}
			else {
				p->m_StateAI.ChangeState(PlayerState::DamageState);
			}
			
		}

	}

}

int FireElemental::SelectAttackType()
{
	//プレイヤーとの距離を求める
	float range = (Player::GetInstance()->m_Transform.position - m_Transform.position).Length();
	printf("ボスとの距離%.2f\n", range);
	//至近距離
	float close_range = 20.0f;
	//中距離
	float medium_range = 40.0f;
	
	//距離に応じた攻撃方法を選別する
	if (range <= close_range) {
		//2分の１で攻撃を決める
		return  AttackType::SlamGround;//たたきつけ
		//(Utility::IsPlusMinus() == 1) ? AttackType::Strike :
	}
	else return AttackType::ThrowStone;//岩投げ

}

void FireElemental::AddMoveForce()
{
	//まず移動の更新をしたあとに
	m_Transform.position += m_Transform.m_pos_vec * DELTA;
	//移動ベクトルに減数係数をかける
	m_Transform.m_pos_vec.x *= FRICTION;
	m_Transform.m_pos_vec.z *= FRICTION;
}


void FireElemental::GameObjectCollisionCheck(CollisionTask* task)
{
	//通常状態
	switch (task->GetPriority())
	{
		//静的なオブジェクトとの判定
	case  CollisionPriority::eCol_Field:
		if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (task->GetTask())) {

			CVector3D d = b->m_Transform.position - m_Transform.position;

			if (d.Length() <= b->GetModel().GetMax().Length()) {
				//このキャラクターとオブジェクトとの当たり判定
				{

					//衝突データ格納用
					CCollTriangle out[256];
					//押し戻し量
					CVector3D v(0, 0, 0);

					//カプセルとモデルとの衝突
					int cnt = b->GetModel().CollisionCupsel(out, m_Transform.position + CVector3D(0, m_rad, 0),
						m_Transform.position + CVector3D(0, 1.8 - m_rad, 0), m_rad, 250);
					//接触した面の数繰り返す
					for (int i = 0; i < cnt; i++) {

						//接触した面が斜めより上を向いていたら地面
						if (out[i].m_normal.y > 0.5f) {
							//重力落下速度を0に戻す
							m_Transform.m_pos_vec.y = 0;

						}
						float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
						//接触した面の方向へ、めり込んだ分押し戻す
						CVector3D nv = out[i].m_normal * (m_rad - out[i].m_dist);
						//最も大きな移動量を求める
						v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
						//膝下までは乗り越える
						//膝上以上の壁のみ押し戻される
						if (max_y > m_Transform.position.y + 0.5f) {
							v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
							v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
						}

					}
					//押し戻す
					m_Transform.position += v;
				}
			}

		}
	}
}

void FireElemental::UpdateCollider()
{
	for (int i = 0; i < m_cupsule_max; i++) {
		//カプセルを設定
		m_cupsule[i].m_start = m_model.GetFrameMatrix(m_culsule_idx[i].s).GetPosition();
		m_cupsule[i].m_end = m_model.GetFrameMatrix(m_culsule_idx[i].e).GetPosition();
		m_cupsule[i].m_rad = m_culsule_idx[i].r;
	}
}

void FireElemental::CollisionArrow(CollisionTask* task)
{
	Arrow* a = dynamic_cast<Arrow*>(task->GetTask());
	//既に刺さっている場合
	if (a->mp_parent_model) return;
	for (int i = 0; i < m_cupsule_max; i++) {
		CVector4D color(0, 1, 0, 1);
		if (CCollision::CollisionCapsule(a->m_s, a->m_e, a->m_rad, m_cupsule[i].m_start, m_cupsule[i].m_end, m_cupsule[i].m_rad)) {
			a->SetParent(&m_model, m_culsule_idx[i].s);
			
			EffectCollection::BloodSplash3D(a->m_Transform.position);

			SOUND(SoundResource::SE_ArrowHit.c_str())->Play3D(a->m_Transform.position, a->m_Transform.m_pos_vec);
			if (--m_hp <= 0) {
				m_stateAI.ChangeState(FireElementalState::DeathState);
				printf("死にました\n");
			}
			break;
		}
	}

}
