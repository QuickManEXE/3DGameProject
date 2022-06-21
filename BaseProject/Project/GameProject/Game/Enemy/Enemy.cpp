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
{3,6,0.5f},		//����
{49,50,0.4f},		//��
{8,9,0.2f},		//���r
{9,10,0.2f},		//����
{29,30,0.2f},		//�E�r
{30,31,0.2f},		//�E��
{51,52,0.2f},		//������
{52,53,0.2f},		//����
{57,58,0.2f},		//�E����
{58,59,0.2f},		//�E��
};



Enemy::Enemy(const Transform& _transform, const char* _modelName):
	CharacterBase(UpdatePriority::eUp_Enemy,"Enemy",_modelName, RenderPriority::eRd_Enemy)
{
	
	m_Model = COPY_RESOURCE(_modelName, CModelA3M);

	//���W�ݒ�
	this->m_Transform = _transform;

	m_PopPos = _transform.position;

	m_rad = 0.2f;

	m_HitPoint = 1; rand() % 3 + 5;

	// �`��^�X�N�ɓo�^
	//m_Render.RegistRender(this, (RenderFunc)&Enemy::Render, RenderPriority::eRd_Enemy, "Enemy");

	// �Փ˔���^�X�N�ɓo�^
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
	{//�Փ˗p�J�v�Z���R���C�_�[
		m_Capsule.SetRad(0.8f);
		m_Capsule.Set(m_Transform.position + CVector3D(0.0f, m_Capsule.m_rad, 0.0f), m_Transform.position + CVector3D(0.0f, 2.0f - m_Capsule.m_rad, 0.0f));
	}

	{//�ǂ̂ڂ�p���R���C�_�[(���E2��)

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

	{//��̎h����J�v�Z���̃R���C�_�[

		for (int i = 0; i < m_cupsule_max; i++) {
			//�J�v�Z����ݒ�
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
	
	//���E�ɉ����������Ă�����
	CVector3D view_pos = m_Transform.position + CVector3D(0.0f, 1.8, 0.0f);
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

					if (StaticMeshObject* g = dynamic_cast<StaticMeshObject*> (c->GetTask())) {

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

void Enemy::AttackCollide(CollisionTask* _task)
{
	//�U�����ł͂Ȃ���Δ��肵�Ȃ�
	if (!m_IsAttack)return;
	//�����v���C���[�Ȃ画�肷��
	if (_task->GetPriority() != CollisionPriority::eCol_Player)return;
		//�v���C���[�ɕϊ�
		if (Player* p = dynamic_cast<Player*> (_task->GetTask())){

			CVector3D enemy_cup_s = m_Model.GetFrameMatrix(m_culsule_idx[5].s).GetPosition();
			CVector3D enemy_cup_e = m_Model.GetFrameMatrix(m_culsule_idx[5].e).GetPosition();

			//�G�l�~�[�̘r�ƃv���C���[�̑̂̃J�v�Z�����m�Ŕ���
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
			//���̃L�����N�^�[�O���̂Q�̐��ƃI�u�W�F�N�g�Ƃ̓����蔻��(�ǂ̂ڂ�p)

			//�ڐG�_�Ɩ@��
			CVector3D cross1, normal1;
			//�v���C���[�����̐��Ɣ���
			if (b->GetModel().CollisionRay(&cross1, &normal1, m_LineCol[0].m_start, m_LineCol[0].m_end)) {

				
				CVector3D cross2, normal2;
				//�v���C���[�E���̐��Ɣ���
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

			//�ڐG�_�Ɩ@��
			CVector3D cross, normal;
			//�v���C���[�O�������Ɣ���
			if (b->GetModel().CollisionRay(&cross, &normal, line_posion, line_posion - CVector3D(0, 10.0f, 0))) {

				float y = m_Transform.position.y - cross.y;

				//printf("%f::%f\n", transform.position.y,cross.y);
				//�����ڐG�_�����_��������Ă���Ȃ�
				if (y >= 1.8f) {

					//state_AI.ChangeState(EnemyState::FallState);
					//printf("%f::�������Ă܂�\n", y);

				}

				
			}
		}
	}
}



void Enemy::CollisionArrow(CollisionTask* _task)
{
	if (_task->GetPriority()!=CollisionPriority::eCol_Player_Attack)return;

	Arrow* a = dynamic_cast<Arrow*>(_task->GetTask());
	//���Ɏh�����Ă���ꍇ
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



