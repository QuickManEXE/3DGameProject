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
{4,5,2.0f},		//����
{6,7,2.9f},		//��
{8,10,1.6f},	//���r
{10,22,1.4f},	//����
{32,34,1.6f},	//�E�r
{34,46,1.4f},	//�E��
{56,57,1.4f},	//������
{57,58,1.4f},	//����
{62,63,1.4f},	//�E����
{63,64,1.4f},	//�E��
};

FireElemental::FireElemental(const Transform& transform, const char* model_name) 
	: Base(UpdatePriority::eUp_Enemy,"FireElemental")
{
	m_model = COPY_RESOURCE(model_name, CModelA3M);

	//���W�ݒ�
	this->m_Transform = transform;

	m_rad = 0.2f;

	m_hp = 10;

	// �`��^�X�N�ɓo�^
	m_render.RegistRender(this, (RenderFunc)&FireElemental::Render, RenderPriority::eRd_Enemy, "FireElemental");

	// �Փ˔���^�X�N�ɓo�^
	m_col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&FireElemental::CollisionCheck, CollisionPriority::eCol_Enemy, "FireElemental");

	// �Փ˔���^�X�N�ɓo�^
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
	
	//FONT_T()->Draw(100, 100, 0, 0, 0, "����%.2f", (Player::GetInstance()->transform.position - transform.position).Length());
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
	//�d�͂�������
	m_Transform.m_pos_vec.y -= GRAVITY * DELTA;

}

void FireElemental::StandardRender()
{
	//�L�����N�^�[�\��
	m_model.SetPos(m_Transform.position);
	m_model.SetRot(m_Transform.rotation);
	m_model.SetScale(m_Transform.scale);
	m_model.Render();

	
}

bool FireElemental::SearchPlayer()
{
	//���E�ɉ����������Ă�����
	CVector3D view_pos = m_Transform.position + CVector3D(0, 1.8, 0);
	CVector3D view_rot = m_Transform.rotation;
	CVector3D target_pos = Player::GetInstance()->GetCenterPoint();
	if (Utility::IsViewInside(view_pos, view_rot, target_pos, DtoR(60.0f), 5.0f)) {

		//printf("���E���ɓ����Ă��܂�\n");
		Utility::DrawLine(view_pos, target_pos, CVector4D(0, 1, 0, 1));
		//�^�[�Q�b�g�ƃv���C���[�̎����̐�ɏ�Q�����Ȃ������ׂ�

		CVector3D dir = target_pos - view_pos;
		float length = 5.0f;
		std::vector<CollisionTask*> buff = CollisionTaskManager::GetInstance()->GetCollisionList();

		if (buff.size() > 0) {

			bool is_view = true;

			//buff�̒��g�œ����蔻��
			for (CollisionTask* c : buff) {


				if (c->GetPriority() == CollisionPriority::eCol_Field) {

					if (NonAnimObject* g = dynamic_cast<NonAnimObject*> (c->GetTask())) {

						if (CCollision::CollisionShpere(g->m_Transform.position, 0,
							m_Transform.position, 10.0f)) {
							//����Ƃ����Ŏ��������������Ă��Ȃ�������
							//�ڐG�_�Ɩ@��
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

	//�U�����ł͂Ȃ���Δ��肵�Ȃ�
	if (!m_is_attack)return;
	//�����v���C���[�Ȃ画�肷��
	if (task->GetPriority() != CollisionPriority::eCol_Player)return;
	//�v���C���[�ɕϊ�
	if (Player* p = dynamic_cast<Player*> (task->GetTask())) {

		CVector3D enemy_cup_s = m_model.GetFrameMatrix(m_culsule_idx[5].s).GetPosition();
		CVector3D enemy_cup_e = m_model.GetFrameMatrix(m_culsule_idx[5].e).GetPosition();

		//�G�l�~�[�̘r�ƃv���C���[�̑̂̃J�v�Z�����m�Ŕ���
		if (!p->m_StateAI.IsCurrentState(PlayerState::DeathState) && !p->m_StateAI.IsCurrentState(PlayerState::DamageState) && CCollision::CollisionCapsule(enemy_cup_s, enemy_cup_e, m_culsule_idx[5].r,
			p->m_Capsule.m_start, p->m_Capsule.m_end, p->m_Capsule.m_rad)) {

			//�������Ă��
			printf("�������Ă��\n");
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
	//�v���C���[�Ƃ̋��������߂�
	float range = (Player::GetInstance()->m_Transform.position - m_Transform.position).Length();
	printf("�{�X�Ƃ̋���%.2f\n", range);
	//���ߋ���
	float close_range = 20.0f;
	//������
	float medium_range = 40.0f;
	
	//�����ɉ������U�����@��I�ʂ���
	if (range <= close_range) {
		//2���̂P�ōU�������߂�
		return  AttackType::SlamGround;//��������
		//(Utility::IsPlusMinus() == 1) ? AttackType::Strike :
	}
	else return AttackType::ThrowStone;//�Ⓤ��

}

void FireElemental::AddMoveForce()
{
	//�܂��ړ��̍X�V���������Ƃ�
	m_Transform.position += m_Transform.m_pos_vec * DELTA;
	//�ړ��x�N�g���Ɍ����W����������
	m_Transform.m_pos_vec.x *= FRICTION;
	m_Transform.m_pos_vec.z *= FRICTION;
}


void FireElemental::GameObjectCollisionCheck(CollisionTask* task)
{
	//�ʏ���
	switch (task->GetPriority())
	{
		//�ÓI�ȃI�u�W�F�N�g�Ƃ̔���
	case  CollisionPriority::eCol_Field:
		if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (task->GetTask())) {

			CVector3D d = b->m_Transform.position - m_Transform.position;

			if (d.Length() <= b->GetModel().GetMax().Length()) {
				//���̃L�����N�^�[�ƃI�u�W�F�N�g�Ƃ̓����蔻��
				{

					//�Փ˃f�[�^�i�[�p
					CCollTriangle out[256];
					//�����߂���
					CVector3D v(0, 0, 0);

					//�J�v�Z���ƃ��f���Ƃ̏Փ�
					int cnt = b->GetModel().CollisionCupsel(out, m_Transform.position + CVector3D(0, m_rad, 0),
						m_Transform.position + CVector3D(0, 1.8 - m_rad, 0), m_rad, 250);
					//�ڐG�����ʂ̐��J��Ԃ�
					for (int i = 0; i < cnt; i++) {

						//�ڐG�����ʂ��΂߂���������Ă�����n��
						if (out[i].m_normal.y > 0.5f) {
							//�d�͗������x��0�ɖ߂�
							m_Transform.m_pos_vec.y = 0;

						}
						float max_y = max(out[i].m_vertex[0].y, max(out[i].m_vertex[1].y, out[i].m_vertex[2].y));
						//�ڐG�����ʂ̕����ցA�߂荞�񂾕������߂�
						CVector3D nv = out[i].m_normal * (m_rad - out[i].m_dist);
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
	}
}

void FireElemental::UpdateCollider()
{
	for (int i = 0; i < m_cupsule_max; i++) {
		//�J�v�Z����ݒ�
		m_cupsule[i].m_start = m_model.GetFrameMatrix(m_culsule_idx[i].s).GetPosition();
		m_cupsule[i].m_end = m_model.GetFrameMatrix(m_culsule_idx[i].e).GetPosition();
		m_cupsule[i].m_rad = m_culsule_idx[i].r;
	}
}

void FireElemental::CollisionArrow(CollisionTask* task)
{
	Arrow* a = dynamic_cast<Arrow*>(task->GetTask());
	//���Ɏh�����Ă���ꍇ
	if (a->mp_parent_model) return;
	for (int i = 0; i < m_cupsule_max; i++) {
		CVector4D color(0, 1, 0, 1);
		if (CCollision::CollisionCapsule(a->m_s, a->m_e, a->m_rad, m_cupsule[i].m_start, m_cupsule[i].m_end, m_cupsule[i].m_rad)) {
			a->SetParent(&m_model, m_culsule_idx[i].s);
			
			EffectCollection::BloodSplash3D(a->m_Transform.position);

			SOUND(SoundResource::SE_ArrowHit.c_str())->Play3D(a->m_Transform.position, a->m_Transform.m_pos_vec);
			if (--m_hp <= 0) {
				m_stateAI.ChangeState(FireElementalState::DeathState);
				printf("���ɂ܂���\n");
			}
			break;
		}
	}

}
