#pragma once
#include"../../Base/Base.h"

class StoneAttack : public Base
{
private:
	CModelObj m_model;
	CModel* mp_parent_model;
	CMatrix m_local_matrix;
	CMatrix m_world_matrix;
	int		m_parent_idx;
	bool	m_shot;

	RenderTask m_Render;

	CollisionTask m_Col;

	float t;

	CVector3D m_target_point;

	CSphere m_sphere;
public:
	StoneAttack(UpdatePriority priority, const char* model_name,const Transform& trans,const CVector3D& target_pos,float rad);
	~StoneAttack();
	void Update()override;
	void Render();
	void CollisionCheck(CollisionTask* task);
	void SetParent(CModel* p, int idx);
	void SetShot() {
		m_shot = true;
	};
};