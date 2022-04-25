#include "SlamGroundCollider.h"
#include"../Player/Player.h"
#include"../../Effect/EffectCollection.h"
#include"../../SoundResource.h"

SlamGroundCollider::SlamGroundCollider(UpdatePriority priority, const CVector3D& pos,float rad, float time)
	: Base(priority,"SlamGroundCollider")
{
	// 描画タスクに登録
	m_Render.RegistRender(this, (RenderFunc)&SlamGroundCollider::Render, RenderPriority::eRd_Enemy_Attack, "EnemyAttack");

	// 衝突判定タスクに登録
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&SlamGroundCollider::CollisionCheck, CollisionPriority::eCol_Enemy_Attack, "EnemyAttack");

	m_limit_time = time;

	m_Transform.position = pos;

	m_rad = rad;

	EffectCollection::RockSplash3D(m_Transform.position + CVector3D(0, -1.5f, 0));

	EffectCollection::ShockWave3D(m_Transform.position + CVector3D(0,-1.5f,0));

	SOUND(SoundResource::SE_SlamGround.c_str())->Play3D(m_Transform.position, CVector3D(100.0f,100.0f,100.0f));
	
}

SlamGroundCollider::~SlamGroundCollider()
{
}

void SlamGroundCollider::Update()
{
	m_limit_time -= DELTA;
	//もし
	if (m_limit_time <= 0)SetKill();
}

void SlamGroundCollider::Render()
{
	//Utility::DrawSphere(transform.position, m_rad, CVector4D(1, 0, 0, 0.3));
}

void SlamGroundCollider::CollisionCheck(CollisionTask* task)
{
	//もしプレイヤーなら判定する
	if (task->GetPriority() != CollisionPriority::eCol_Player)return;
	//プレイヤーに変換
	if (Player* p = dynamic_cast<Player*> (task->GetTask())) {


		if (p->m_StateAI.IsCurrentState(PlayerState::DeathState) || 
			p->m_StateAI.IsCurrentState(PlayerState::DamageState))return;

		//エネミーの腕とプレイヤーの体のカプセル同士で判定
		if (CCollision::CollisionCapsuleShpere(p->m_Capsule.m_start, p->m_Capsule.m_end, p->m_Capsule.m_rad,m_Transform.position, m_rad)) {

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
