#pragma once
#include "../Base/Base.h"

class Arrow : public Base {
public:
	//���f���I�u�W�F�N�g
	CModelObj m_model;
	CModel* mp_parent_model;
	CMatrix m_local_matrix;
	CMatrix m_world_matrix;
	int		m_parent_idx;
	bool	m_shot;
	RenderTask m_Render;
	CollisionTask m_Collision;
	//���a
	float m_rad;
	//�J�v�Z���p
	CVector3D m_s;
	CVector3D m_e;

	CVector3D* m_rope_pos;
public:
	//�R���X�g���N�^
	Arrow();
	//�X�V
	void Update();
	//�`��
	void Render();
	//�����蔻�茟��
	void CollisionCheck(CollisionTask* _task);
	void SetParent(CModel* p, int idx);
	void SetDir(const CVector3D& dir);
	void Shot(float pow, CVector3D* rope_pos=nullptr);
};