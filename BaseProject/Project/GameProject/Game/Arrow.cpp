#include "Arrow.h"


Arrow::Arrow() :Base(UpdatePriority::eUp_Player_Attack, "PlayerArrow"), mp_parent_model(nullptr), m_shot(false),m_rope_pos(nullptr)
{
	m_model = COPY_RESOURCE("Arrow", CModelObj);

	m_rad = 0.02f;

	// �`��^�X�N�ɓo�^
	m_Render.RegistRender(this, (RenderFunc)&Arrow::Render, RenderPriority::eRd_Player_Attack, "PlayerArrow");

	// �Փ˔���^�X�N�ɓo�^
	m_Collision.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Arrow::CollisionCheck, CollisionPriority::eCol_Player_Attack, "PlayerArrow");

	m_time = 0.0f;
}
void Arrow::Update() {

	m_s = m_world_matrix * CVector4D(0, 0, 0.5f, 1);
	m_e = m_world_matrix * CVector4D(0, 0, -0.5f, 1);

	if (mp_parent_model) return;
	if (m_rope_pos) {
		CVector3D dir = (*m_rope_pos) - m_Transform.position;

		if (dir.Length() <= 1.0f) {

			m_Transform.position = (*m_rope_pos);
		}
		else {

			const float gravity = 0.001f;
			m_Transform.m_pos_vec.y -= gravity;
			m_Transform.position += m_Transform.m_pos_vec;
		}
	}
	else {
		const float gravity = 0.001f;
		m_Transform.m_pos_vec.y -= gravity;
		m_Transform.position += m_Transform.m_pos_vec;
	}
	
	//�����鏈��
	if (m_shot && !mp_parent_model) {

		m_time += DELTA;
		if (m_time >= 5.0f) {
			SetKill();
		}
	}

}

void Arrow::CollisionCheck(CollisionTask* _task)
{

}

void Arrow::Render()
{
	
	if (mp_parent_model) {
		if (m_shot) {
			//��������
			//�G�ɖ�h�����Ă���
			CMatrix P = mp_parent_model->GetFrameMatrix(m_parent_idx);
			//�e�̃X�P�[����ł�����
			float s = P.GetFront().Length();
			P = P * CMatrix::MScale(s, s, s).GetInverse();
			m_world_matrix = P * m_local_matrix;
		}
		else {
			//�\����
			//�ʒu�̂݃{�[������擾
			m_Transform.position = mp_parent_model->GetFrameMatrix(m_parent_idx).GetPosition();
			//��]�s��͕�������
			CMatrix M;
			M.LookAt(CVector3D::zero, m_Transform.m_pos_vec, CVector3D::up);
			m_world_matrix = CMatrix::MTranselate(m_Transform.position) * M * CMatrix::MTranselate(-0.04f, 0.02f, 0.5f);
		}
	}
	else {
		//���˒�
		CMatrix M;
		M.LookAt(CVector3D::zero, m_Transform.m_pos_vec, CVector3D::up);
		m_world_matrix = CMatrix::MTranselate(m_Transform.position) * M;
	}
	m_model.Render(m_world_matrix * CMatrix::MScale(2, 2, 2));
	//	Utility::DrawCapsule(m_s, m_e, m_rad, CVector4D(1, 0, 0, 1));
}

void Arrow::SetParent(CModel* p, int idx)
{
	mp_parent_model = p;
	m_parent_idx = idx;

	CMatrix P = mp_parent_model->GetFrameMatrix(m_parent_idx);
	//�e�̃X�P�[���ɉe������A��̑傫�����ς��Ȃ��悤
	//�e�̃X�P�[����ł�����
	float s = P.GetFront().Length();
	P = P * CMatrix::MScale(s, s, s).GetInverse();
	//�e�̋t�s�񂩂烍�[�J���s����t�Z
	//��h����Ƃ��Ɏg�p
	m_local_matrix = P.GetInverse() * m_world_matrix;
}

void Arrow::SetDir(const CVector3D& dir)
{
	m_Transform.m_pos_vec = dir;
}

void Arrow::Shot(float pow,CVector3D* rope_pos)
{
	m_Transform.position = m_world_matrix.GetPosition();
	m_Transform.m_pos_vec = m_world_matrix.GetFront() * pow;
	m_shot = true;
	mp_parent_model = nullptr;
	if (rope_pos) m_rope_pos = rope_pos;
}
