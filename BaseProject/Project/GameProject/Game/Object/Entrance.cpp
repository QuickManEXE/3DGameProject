#include "Entrance.h"
#include"../../StarterAsset.h"
#include"../CharacterBase/CharacterBase.h"

std::vector<Entrance*> Entrance::m_entrances;

Entrance::Entrance(int _entrance_num) : entrance_num(_entrance_num),
	StaticMeshObject(UpdatePriority::eUp_Field,"Entrance", StarterAsset::Cylinder,RenderPriority::eRd_Field)
{
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Entrance::CollisionCheck, CollisionPriority::eCol_Field, "Entrance");

	m_rad = 5.0f;

	m_entrances.push_back(this);

	m_door = COPY_RESOURCE(StarterAsset::Square , CModelObj);
	
	m_door_transform[left_door].position = CVector3D(-1.5, 0, 0);
	m_door_transform[right_door].position = CVector3D(1.5, 0, 0);
}

void Entrance::Update()
{
	
	CMatrix parent_matrix =
		CMatrix::MTranselate(m_Transform.position) *
		CMatrix::MRotation(m_Transform.rotation);


	m_door_matrix[left_door] =
		parent_matrix * CMatrix::MTranselate(m_door_transform[left_door].position) *
		CMatrix::MRotation(m_door_transform[left_door].rotation);// *CMatrix::MScale(CVector3D(2, 0.5, 4));
	m_door_matrix[right_door] =
		parent_matrix * CMatrix::MTranselate(m_door_transform[right_door].position) *
		CMatrix::MRotation(m_door_transform[right_door].rotation);// *CMatrix::MScale(CVector3D(2, 0.5, 4));

	m_door_transform[left_door].rotation.y += DtoR(0.1);
}

void Entrance::Render()
{
	for (auto m : m_door_matrix) {
		m_door.Render(m);
	}
	
}

void Entrance::CollisionCheck(CollisionTask* _task)
{
	if (CharacterBase* c = dynamic_cast<CharacterBase*> (_task->GetTask())) {

		CVector3D d = c->m_Transform.position - m_Transform.position;
		if (d.Length() > m_door.GetMax().Length())return;

		//���̃L�����N�^�[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
		{
			for (int j = 0; j < max_count; j++) {
				//�Փ˃f�[�^�i�[�p
				CCollTriangle out[256];
				//�����߂���
				CVector3D v(0, 0, 0);

				//m_door.UpdateMatrix(m_door_matrix[j]);
				//�J�v�Z���ƃ��f���Ƃ̏Փ�
				

				int cnt = m_door.CollisionCupsel(out, c->m_Capsule.m_start,
					c->m_Capsule.m_end, c->m_Capsule.m_rad, 250);

				//�ڐG�����ʂ̐��J��Ԃ�
				for (int i = 0; i < cnt; i++) {

					//�ڐG�����ʂ��΂߂���������Ă�����n��
					if (out[i].m_normal.y > 0.5f) {
						//�d�͗������x��0�ɖ߂�
						c->m_Transform.m_pos_vec.y = 0;

						c->m_IsGround = true;
					}

					float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
					//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
					CVector3D nv = out[i].m_normal * (c->m_Capsule.m_rad - out[i].m_dist);
					//�ł��傫�Ȉړ��ʂ����߂�
					v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
					//�G���܂ł͏��z����
					//�G��ȏ�̕ǂ̂݉����߂����
					if (max_y > c->m_Transform.position.y + 0.5f) {
						v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
						v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
					}
				}
				//�����߂�
				c->m_Transform.position -= v;
			}
		}


	}
}
