#include "StoneAttack.h"
#include"../Player/Player.h"
#include"../../Effect/EffectCollection.h"

StoneAttack::StoneAttack(UpdatePriority priority, const char* model_name, const Transform& trans, const CVector3D& target_pos,float rad)
	: Base(priority, "StoneAttack"), mp_parent_model(nullptr), m_shot(false)
{

	m_model = COPY_RESOURCE(model_name, CModelObj);
	// 描画タスクに登録
	m_Render.RegistRender(this, (RenderFunc)&StoneAttack::Render, RenderPriority::eRd_Enemy_Attack, "EnemyAttack");

	// 衝突判定タスクに登録
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&StoneAttack::CollisionCheck, CollisionPriority::eCol_Enemy_Attack, "EnemyAttack");

	this->m_Transform = trans;

	t = 0;

	m_target_point = target_pos;

	m_sphere.Set(m_Transform.position + CVector3D(0, rad, 0), rad);

}

StoneAttack::~StoneAttack()
{
}

void StoneAttack::Update()
{
	if (m_shot) {

		t += DELTA;


		m_Transform.position = m_Transform.position * (1 - t) + m_target_point * t;

		if (t >= 1) {

			SetKill();
			EffectCollection::RockSplash3D(m_Transform.position);
		}

		m_sphere.Set(m_Transform.position + CVector3D(0, m_sphere.m_rad, 0), m_sphere.m_rad);
	}

}

void StoneAttack::Render()
{
	if (m_shot) {

		m_model.SetPos(m_Transform.position);
		m_model.SetRot(m_Transform.rotation);
		m_model.SetScale(m_Transform.scale);
		m_model.Render();

#ifdef _DEBUG

		//Utility::DrawSphere(m_sphere.m_center, m_sphere.m_rad, CColorRGBA(1, 0, 1, 0.5f));

#endif // _DEBUG
	}
	else if(mp_parent_model) {

		//構え中
		//位置のみボーンから取得
		m_Transform.position = mp_parent_model->GetFrameMatrix(m_parent_idx).GetPosition();
		//回転行列は方向から
		CMatrix M;
		M.LookAt(CVector3D::zero, m_Transform.m_pos_vec, CVector3D::up);
		m_world_matrix = CMatrix::MTranselate(m_Transform.position) *CMatrix::MTranselate(-0.91f, -0.40f, 0.45f);
		m_model.Render(m_world_matrix * CMatrix::MScale(m_Transform.scale));
	}
	
}

void StoneAttack::CollisionCheck(CollisionTask* task)
{
	if (!m_shot)return;
	if (task->GetPriority() != CollisionPriority::eCol_Player)return;
	

	if (Player* p = dynamic_cast<Player*> (task->GetTask())) {

		if (p->m_StateAI.IsCurrentState(PlayerState::DeathState) ||
			p->m_StateAI.IsCurrentState(PlayerState::DamageState))return;


		if (CCollision::CollisionCapsuleShpere(p->m_Capsule.m_start, p->m_Capsule.m_end, p->m_Capsule.m_rad,
			m_sphere.m_center,m_sphere.m_rad)) {

			p->m_HitPoint -= 20;
			if (p->m_HitPoint <= 0) {
				p->m_StateAI.ChangeState(PlayerState::DeathState);
			}
			else {
				p->m_StateAI.ChangeState(PlayerState::DamageState);
			}

		}

	}
}

void StoneAttack::SetParent(CModel* p, int idx)
{

	mp_parent_model = p;
	m_parent_idx = idx;

	CMatrix P = mp_parent_model->GetFrameMatrix(m_parent_idx);
	//親のスケールに影響され、矢の大きさが変わらないよう
	//親のスケールを打ち消し
	float s = P.GetFront().Length();
	P = P * CMatrix::MScale(s, s, s).GetInverse();
	//親の逆行列からローカル行列を逆算
	//矢が刺さるときに使用
	m_local_matrix = P.GetInverse() * m_world_matrix;

}
