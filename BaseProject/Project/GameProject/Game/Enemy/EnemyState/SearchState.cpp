#include "SearchState.h"
#include"../../Player/Player.h"
#include"../../NonAnimObject.h"
#include"../../../SoundResource.h"

SearchState::SearchState(Enemy* owner) : State(owner)
{
}

void SearchState::Enter()
{

	m_time = 10.0f;

	m_rot_target = owner->m_Transform.rotation;
}

void SearchState::Execute()
{
	//�������E�̒��ɓ�������Ȃ�U���X�e�[�g�Ɉڍs����
	Search();

	m_time -= DELTA;

	if (m_time <= 0) {

		m_rot_target.y += DtoR(90);
		if (m_rot_target.y >= DtoR(360)) m_rot_target.y = 0;
		m_time = 10.0f;
	}
	
	Turn();
	
	owner->AddGravity();
	owner->AddMoveForce();

	owner->GetModel()->UpdateAnimation();
}

void SearchState::Exit()
{
}

void SearchState::Search()
{

	//���E�ɉ����������Ă�����
	CVector3D view_pos = owner->m_Transform.position+CVector3D(0,1.8,0);
	CVector3D view_rot = owner->m_Transform.rotation;
	CVector3D target_pos = Player::GetInstance()->m_Transform.position + CVector3D(0, 1.8, 0);
	float view_ang = DtoR(90);
	float length = 20.0f;

	if (Utility::IsViewInside(view_pos, view_rot, target_pos,view_ang, length)) {
	
		//printf("���E���ɓ����Ă��܂�\n");
		Utility::DrawLine(view_pos,target_pos, CVector4D(0, 1, 0, 1));
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
							owner->m_Transform.position, 10.0f)) {
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
			
			if (is_view) {
				printf("���E�̒��Ƀv���C���[�������Ă���I\n");
				owner->m_StateAI.ChangeState(EnemyState::BattleIdleState);

				SOUND(SoundResource::SE_MonsterCry1.c_str())->Play3D(owner->m_Transform.position, owner->m_Transform.m_pos_vec);
			}

		}
		

	}

}

void SearchState::Turn()
{
	float a = Utility::NormalizeAngle(m_rot_target.y - owner->m_Transform.rotation.y);

	//�����ɂ����
	//owner->transform.rotation.y += a* 0.1f* DELTA;

	
	//���ʉ�]�ɂ��p�x���
	float rot_speed = DtoR(30);
	if (a > rot_speed) {
		owner->m_Transform.rotation.y += rot_speed * DELTA;
		owner->GetModel()->ChangeAnimation(2);
	}
	else
		if (a < -rot_speed) {
			owner->m_Transform.rotation.y -= rot_speed * DELTA;
			owner->GetModel()->ChangeAnimation(2);
		}
		else {
			owner->m_Transform.rotation.y += a * DELTA;
			owner->GetModel()->ChangeAnimation(17);
		}
		

	//��Ԍ��-��(-180)�`��(180)�͈̔͂Ɋp�x�𐮂���
	owner->m_Transform.rotation.y = Utility::NormalizeAngle(owner->m_Transform.rotation.y);

}
