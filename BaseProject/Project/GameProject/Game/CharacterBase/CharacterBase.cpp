#include"CharacterBase.h"
#include"../NonAnimObject.h"

CharacterBase::CharacterBase(UpdatePriority _updatePriority,const char* model_name, RenderPriority _renderPriority):
	AnimObject(_updatePriority,model_name,_renderPriority)
{

}

void CharacterBase::Update()
{
}

void CharacterBase::AddGravity()
{
	//重力をかける
	m_Transform.m_pos_vec.y -= GRAVITY * DELTA;
}

void CharacterBase::AddMoveForce() {

	//まず移動の更新をしたあとに
	m_Transform.position += m_Transform.m_pos_vec * DELTA;
	//移動ベクトルに減数係数をかける
	if (m_IsGround) {
		m_Transform.m_pos_vec.x *= 1 - FRICTION;
		m_Transform.m_pos_vec.z *= 1 - FRICTION;
	}
	else {
		m_Transform.m_pos_vec.x *= 1 - 0.05f;
		m_Transform.m_pos_vec.z *= 1 - 0.05f;
	}
}

void CharacterBase::StandardRender()
{
	//キャラクター表示
	m_Model.SetPos(m_Transform.position);
	m_Model.SetRot(m_Transform.rotation);
	m_Model.SetScale(m_Transform.scale);
	m_Model.Render();
}

void CharacterBase::CollisionCharacter(CollisionTask* _task)
{

	if (_task->GetPriority() != CollisionPriority::eCol_Enemy&&
		_task->GetPriority() != CollisionPriority::eCol_Player)return;

	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {


		CVector3D dir;
		float dist;

		if(CCollision::CollisionCapsule(m_Capsule.m_start, m_Capsule.m_end, m_Capsule.m_rad,
			c->m_Capsule.m_start, c->m_Capsule.m_end, c->m_Capsule.m_rad, &dist, &dir)) {

			float s = m_Capsule.m_rad + c->m_Capsule.m_rad - dist;

			m_Transform.m_pos_vec -= dir * s;

		}

	}

}

void CharacterBase::CollisionField(CollisionTask* _task)
{
	if (_task->GetPriority() != CollisionPriority::eCol_Field)return;

	if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (_task->GetTask())) {

		CVector3D d = b->m_Transform.position - m_Transform.position;
		if (d.Length() > b->GetModel().GetMax().Length())return;

		//このキャラクターとオブジェクトとの当たり判定
		{
			//衝突データ格納用
			CCollTriangle out[256];
			//押し戻し量
			CVector3D v(0, 0, 0);

			//カプセルとモデルとの衝突
			int cnt = b->GetModel().CollisionCupsel(out, m_Capsule.m_start,
				m_Capsule.m_end, m_Capsule.m_rad, 250);

			//接触した面の数繰り返す
			for (int i = 0; i < cnt; i++) {

				//接触した面が斜めより上を向いていたら地面
				if (out[i].m_normal.y > 0.5f) {
					//重力落下速度を0に戻す
					m_Transform.m_pos_vec.y = 0;

					m_IsGround = true;
				}

				float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
				//接触した面の方向へ、めり込んだ分押し戻す
				CVector3D nv = out[i].m_normal * (m_Capsule.m_rad - out[i].m_dist);
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

