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
	//�d�͂�������
	m_Transform.m_pos_vec.y -= GRAVITY * DELTA;
}

void CharacterBase::AddMoveForce() {

	//�܂��ړ��̍X�V���������Ƃ�
	m_Transform.position += m_Transform.m_pos_vec * DELTA;
	//�ړ��x�N�g���Ɍ����W����������
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
	//�L�����N�^�[�\��
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

		//���̃L�����N�^�[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
		{
			//�Փ˃f�[�^�i�[�p
			CCollTriangle out[256];
			//�����߂���
			CVector3D v(0, 0, 0);

			//�J�v�Z���ƃ��f���Ƃ̏Փ�
			int cnt = b->GetModel().CollisionCupsel(out, m_Capsule.m_start,
				m_Capsule.m_end, m_Capsule.m_rad, 250);

			//�ڐG�����ʂ̐��J��Ԃ�
			for (int i = 0; i < cnt; i++) {

				//�ڐG�����ʂ��΂߂���������Ă�����n��
				if (out[i].m_normal.y > 0.5f) {
					//�d�͗������x��0�ɖ߂�
					m_Transform.m_pos_vec.y = 0;

					m_IsGround = true;
				}

				float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
				//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
				CVector3D nv = out[i].m_normal * (m_Capsule.m_rad - out[i].m_dist);
				//�ł��傫�Ȉړ��ʂ����߂�
				v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
				//�G���܂ł͏��z����
				//�G��ȏ�̕ǂ̂݉����߂����
				if (max_y > m_Transform.position.y + 0.5f) {
					v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
					v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
				}
			}
			//�����߂�
			m_Transform.position += v;
		}


	}
}

