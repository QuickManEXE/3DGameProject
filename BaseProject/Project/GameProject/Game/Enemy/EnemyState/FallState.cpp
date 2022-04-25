#include "FallState.h"

FallState::FallState(Enemy* owner) : State(owner),is_fall(false)
{
}

void FallState::Enter()
{
	
	is_fall = false;

	owner->m_model_a3m.ChangeAnimation(7);

}

void FallState::Execute()
{
	const float frame_check_point = 32;
	
	const float frame_max = 58;

	const float anim_frame = owner->m_model_a3m.GetAnimationFrame();


	owner->AddGravity();
	owner->AddMoveForce();

	if (is_fall) {

		if(anim_frame>=frame_max)
		//owner->state_AI.ChangeState(EnemyState::WanderState);
		owner->m_model_a3m.UpdateAnimation();

	}
	else if(anim_frame<=frame_check_point){

		owner->m_model_a3m.UpdateAnimation();

	}
}

void FallState::Exit()
{
}

void FallState::Render()
{
	owner->StandardRender();
}

void FallState::CollisionCheck(CollisionTask* task)
{
	//�ʏ���
	switch (task->GetPriority())
	{
		//�ÓI�ȃI�u�W�F�N�g�Ƃ̔���
	case  CollisionPriority::eCol_Field:
		if (GameObject* b = dynamic_cast<GameObject*> (task->GetTask())) {

			CVector3D d = b->transform.position - owner->transform.position;

			if (d.Length() <= b->GetModel().GetMax().Length()) {
				//���̃L�����N�^�[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
				{

					//�Փ˃f�[�^�i�[�p
					CCollTriangle out[256];
					//�����߂���
					CVector3D v(0, 0, 0);

					//�J�v�Z���ƃ��f���Ƃ̏Փ�
					int cnt = b->GetModel().CollisionCupsel(out, owner->transform.position + CVector3D(0, owner->m_rad, 0),
						owner->transform.position + CVector3D(0, 1.8 - owner->m_rad, 0), owner->m_rad, 250);
					//�ڐG�����ʂ̐��J��Ԃ�
					for (int i = 0; i < cnt; i++) {

						//�ڐG�����ʂ��΂߂���������Ă�����n��
						if (out[i].m_normal.y > 0.5f) {
							//�d�͗������x��0�ɖ߂�
							owner->m_move_vec.y = 0;

							is_fall = true;
						}
						float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
						//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
						CVector3D nv = out[i].m_normal * (owner->m_rad - out[i].m_dist);
						//�ł��傫�Ȉړ��ʂ����߂�
						v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
						//�G���܂ł͏��z����
						//�G��ȏ�̕ǂ̂݉����߂����
						if (max_y > owner->transform.position.y + 0.5f) {
							v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
							v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
						}

					}
					//�����߂�
					owner->transform.position += v;
				}
			}

			//owner->HangCheck(task);

		}
	}
}
