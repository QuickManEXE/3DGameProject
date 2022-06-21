#include "Enemy.h"
#include"EnemyState/SearchState.h"
#include"EnemyState/BattleIdleState.h"
#include"EnemyState/IntervalState.h"
#include"EnemyState/EnemyHangWallState.h"
#include"EnemyState/AvoidState.h"
#include"EnemyState/ChaseState.h"
#include"EnemyState/EnemyAttackState.h"
#include"EnemyState/DamageState.h"
#include"EnemyState/DeathState.h"
#include"../Player/Player.h"
#include"../Arrow.h"
#include"../../Effect/EffectCollection.h"
#include"../../SoundResource.h"

Enemy::Index Enemy::m_culsule_idx[m_cupsule_max] = {
{3,6,0.5f},		//胴体
{49,50,0.4f},		//頭
{8,9,0.2f},		//左腕
{9,10,0.2f},		//左手
{29,30,0.2f},		//右腕
{30,31,0.2f},		//右手
{51,52,0.2f},		//左もも
{52,53,0.2f},		//左足
{57,58,0.2f},		//右もも
{58,59,0.2f},		//右足
};



Enemy::Enemy(const Transform& _transform, const char* _modelName):
	CharacterBase(UpdatePriority::eUp_Enemy,"Enemy",_modelName, RenderPriority::eRd_Enemy)
{
	
	m_Model = COPY_RESOURCE(_modelName, CModelA3M);

	//座標設定
	this->m_Transform = _transform;

	m_PopPos = _transform.position;

	m_rad = 0.2f;

	m_HitPoint = 1; rand() % 3 + 5;

	// 描画タスクに登録
	//m_Render.RegistRender(this, (RenderFunc)&Enemy::Render, RenderPriority::eRd_Enemy, "Enemy");

	// 衝突判定タスクに登録
	m_Col.RegistCollision(CollisionTask::eLayer_Game,this, (CollisionFunc)&Enemy::CollisionCheck, CollisionPriority::eCol_Enemy, "Enemy");
	m_AttackCol.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Enemy::AttackCollide, CollisionPriority::eCol_Enemy_Attack, "Enemy");


	m_StateAI.state_vector.push_back(new SearchState(this));
	m_StateAI.state_vector.push_back(new BattleIdleState(this));
	m_StateAI.state_vector.push_back(new IntervalState(this));
	m_StateAI.state_vector.push_back(new EnemyHangWallState(this));
	m_StateAI.state_vector.push_back(new AvoidState(this));
	m_StateAI.state_vector.push_back(new ChaseState(this));
	m_StateAI.state_vector.push_back(new EnemyAttackState(this));
	m_StateAI.state_vector.push_back(new DamageState(this));
	m_StateAI.state_vector.push_back(new DeathState(this));

	m_StateAI.stateMachine = new StateMachine<Enemy>;

	m_StateAI.ChangeState(EnemyState::SearchState);

	m_IsAttack = false;

	m_IsGround = false;
}

Enemy::~Enemy()
{
	for (Arrow* arrow : m_ArrowVector) {

		arrow->SetKill();

	}
}

void Enemy::Update()
{
	m_StateAI.StateUpdate();
	m_IsGround = false;
	UpdateCollider();
	
}

void Enemy::Render()
{
	StandardRender();

	DebugInfo();
}

void Enemy::CollisionCheck(CollisionTask* _task)
{
	CollisionArrow(_task);

	CollisionField(_task);

	CollisionCharacter(_task);

	HangCheck(_task);
}

void Enemy::UpdateCollider()
{
	{//衝突用カプセルコライダー
		m_Capsule.SetRad(0.8f);
		m_Capsule.Set(m_Transform.position + CVector3D(0.0f, m_Capsule.m_rad, 0.0f), m_Transform.position + CVector3D(0.0f, 2.0f - m_Capsule.m_rad, 0.0f));
	}

	{//壁のぼり用線コライダー(左右2つ)

		CVector3D rot = CVector3D(0, m_Transform.rotation.y, 0);
		float front_len = 0.5f;
		float side_len = 0.25f;

		CVector3D lineS = m_Model.GetFrameMatrix("head").GetPosition()
			+ CVector3D(0, 2.0f, 0)
			+ CMatrix::MRotation(rot).GetFront() * front_len
			+ CMatrix::MRotation(rot).GetLeft() * side_len;

		CVector3D lineE = m_Model.GetFrameMatrix("head").GetPosition()
			+ CMatrix::MRotation(rot).GetFront() * front_len
			+ CMatrix::MRotation(rot).GetLeft() * side_len;

		m_LineCol[0].Set(lineS, lineE);


		lineS = m_Model.GetFrameMatrix("head").GetPosition()
			+ CVector3D(0, 2.0f, 0)
			+ CMatrix::MRotation(rot).GetFront() * front_len
			- CMatrix::MRotation(rot).GetLeft() * side_len;

		lineE = m_Model.GetFrameMatrix("head").GetPosition()
			+ CMatrix::MRotation(rot).GetFront() * front_len
			- CMatrix::MRotation(rot).GetLeft() * side_len;

		m_LineCol[1].Set(lineS, lineE);
	}

	{//矢の刺さるカプセルのコライダー

		for (int i = 0; i < m_cupsule_max; i++) {
			//カプセルを設定
			m_cupsule[i].m_start = m_Model.GetFrameMatrix(m_culsule_idx[i].s).GetPosition();
			m_cupsule[i].m_end = m_Model.GetFrameMatrix(m_culsule_idx[i].e).GetPosition();
			m_cupsule[i].m_rad = m_culsule_idx[i].r;
		}

	}
}

void Enemy::DebugInfo()
{
#ifdef _DEBUG
	m_Dijkstra.Draw();

	Utility::DrawCapsule(m_Capsule.m_start, m_Capsule.m_end, m_Capsule.m_rad, CColorRGBA(1.0f, 0.0f, 0.0f, 0.5f));

	for (int i = 0; i < 2; i++) {
		Utility::DrawLine(m_LineCol[i].m_start, m_LineCol[i].m_end, CColorRGBA(0.5f, 0.5, 0, 1.0f));
	}

	if (!m_IsAttack)return;
	for (int i = 0; i < m_cupsule_max; i++) {
		Utility::DrawCapsule(m_cupsule[i].m_start, m_cupsule[i].m_end, m_cupsule[i].m_rad, CVector4D(0.0f, 1.0f, 0.0f,0.5f));
	}
		
#endif // _DEBUG
}

bool Enemy::SearchPlayer()
{
	
	//視界に何かが入っていたら
	CVector3D view_pos = m_Transform.position + CVector3D(0.0f, 1.8, 0.0f);
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

					if (StaticMeshObject* g = dynamic_cast<StaticMeshObject*> (c->GetTask())) {

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

void Enemy::AttackCollide(CollisionTask* _task)
{
	//攻撃中ではなければ判定しない
	if (!m_IsAttack)return;
	//もしプレイヤーなら判定する
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;
		//プレイヤーに変換
		if (Player* p = dynamic_cast<Player*> (_task->GetTask())){

			CVector3D enemy_cup_s = m_Model.GetFrameMatrix(m_culsule_idx[5].s).GetPosition();
			CVector3D enemy_cup_e = m_Model.GetFrameMatrix(m_culsule_idx[5].e).GetPosition();

			//エネミーの腕とプレイヤーの体のカプセル同士で判定
			if (!p->m_StateAI.IsCurrentState(PlayerState::DeathState)&&!p->m_StateAI.IsCurrentState(PlayerState::DamageState)&&CCollision::CollisionCapsule(enemy_cup_s, enemy_cup_e, m_culsule_idx[5].r,
				p->m_Capsule.m_start, p->m_Capsule.m_end, p->m_Capsule.m_rad)) {

				p->m_HitPoint -= 10;
				if (p->m_HitPoint <= 0) {
					p->m_StateAI.ChangeState(PlayerState::DeathState);
				}
				else {
					p->m_StateAI.ChangeState(PlayerState::DamageState);
				}
			}

		}

}

void Enemy::HangCheck(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Field) return;

	if (StaticMeshObject* b = dynamic_cast<StaticMeshObject*> (_task->GetTask())) {

		CVector3D d = b->m_Transform.position - m_Transform.position;

		if (d.Length() <= b->GetModel().GetMax().Length()) {
			//このキャラクター前方の２つの線とオブジェクトとの当たり判定(壁のぼり用)

			//接触点と法線
			CVector3D cross1, normal1;
			//プレイヤー左側の線と判定
			if (b->GetModel().CollisionRay(&cross1, &normal1, m_LineCol[0].m_start, m_LineCol[0].m_end)) {

				
				CVector3D cross2, normal2;
				//プレイヤー右側の線と判定
				if (b->GetModel().CollisionRay(&cross2, &normal2, m_LineCol[1].m_start, m_LineCol[1].m_end)) {

					m_Transform.m_pos_vec.y = 0;

					m_ClimePoint = (cross1 + cross2) / 2.0f;

					m_StateAI.ChangeState(EnemyState::EnemyHangWallState);

				}
			}
		}
	}
}


void Enemy::CanFallCheck(CollisionTask* _task)
{
	if (StaticMeshObject* b = dynamic_cast<StaticMeshObject*> (_task->GetTask())) {

		CVector3D d = b->m_Transform.position - m_Transform.position;

		if (d.Length() <= b->GetModel().GetMax().Length()) {

			
			//CVector3D line_posion = transform.position + CMatrix::MRotation(transform.rotation).GetFront() * 0.2f + CVector3D(0, 0.9f, 0);
			CVector3D line_posion = CVector3D(0, 5.0f, 0);

			//接触点と法線
			CVector3D cross, normal;
			//プレイヤー前方下線と判定
			if (b->GetModel().CollisionRay(&cross, &normal, line_posion, line_posion - CVector3D(0, 10.0f, 0))) {

				float y = m_Transform.position.y - cross.y;

				//printf("%f::%f\n", transform.position.y,cross.y);
				//もし接触点が原点よりも離れているなら
				if (y >= 1.8f) {

					//state_AI.ChangeState(EnemyState::FallState);
					//printf("%f::当たってます\n", y);

				}

				
			}
		}
	}
}



void Enemy::CollisionArrow(CollisionTask* _task)
{
	if (_task->GetPriority()!=CollisionPriority::eCol_Player_Attack)return;

	Arrow* a = dynamic_cast<Arrow*>(_task->GetTask());
	//既に刺さっている場合
	if (a->mp_parent_model) return;
	for (int i = 0; i < m_cupsule_max; i++) {
		CVector4D color(0, 1, 0, 1);
		if (CCollision::CollisionCapsule(a->m_s, a->m_e, a->m_rad, m_cupsule[i].m_start, m_cupsule[i].m_end, m_cupsule[i].m_rad)) {
			a->SetParent(&m_Model, m_culsule_idx[i].s);

			EffectCollection::BloodSplash3D(a->m_Transform.position);

			SOUND(SoundResource::SE_ArrowHit.c_str())->Play3D(a->m_Transform.position, a->m_Transform.m_pos_vec);
			m_ArrowVector.push_back(a);
			if (--m_HitPoint <= 0) {
				m_StateAI.ChangeState(EnemyState::DeathState);
			}
			else {
				m_StateAI.ChangeState(EnemyState::DamageState);
			}
			break;
		}
	}

}



