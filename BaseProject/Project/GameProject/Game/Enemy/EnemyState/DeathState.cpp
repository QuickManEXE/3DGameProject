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
	//�ʏ���
	switch (task->GetPriority())
	{
		//�ÓI�ȃI�u�W�F�N�g�Ƃ̔���
	case  CollisionPriority::eCol_Field:
		if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (task->GetTask())) {

			CVector3D d = b->m_Transform.position - owner->m_Transform.position;

			if (d.Length() <= b->GetModel().GetMax().Length()) {
				//���̃L�����N�^�[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
				{

					//�Փ˃f�[�^�i�[�p
					CCollTriangle out[256];
					//�����߂���
					CVector3D v(0, 0, 0);

					//�J�v�Z���ƃ��f���Ƃ̏Փ�
					int cnt = b->GetModel().CollisionCupsel(out, owner->m_Transform.position + CVector3D(0, owner->m_rad, 0),
						owner->m_Transform.position + CVector3D(0, 1.8 - owner->m_rad, 0), owner->m_rad, 250);
					//�ڐG�����ʂ̐��J��Ԃ�
					for (int i = 0; i < cnt; i++) {

						//�ڐG�����ʂ��΂߂���������Ă�����n��
						if (out[i].m_normal.y > 0.5f) {
							//�d�͗������x��0�ɖ߂�
							owner->m_Transform.m_pos_vec.y = 0;

						}
						float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
						//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
						CVector3D nv = out[i].m_normal * (owner->m_rad - out[i].m_dist);
						//�ł��傫�Ȉړ��ʂ����߂�
						v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
						//�G���܂ł͏��z����
						//�G��ȏ�̕ǂ̂݉����߂����
						if (max_y > owner->m_Transform.position.y + 0.5f) {
							v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
							v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
						}

					}
					//�����߂�
					owner->m_Transform.position += v;
				}
			}
		}
	}
}
