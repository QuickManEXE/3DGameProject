#pragma once
#include"../Base/Base.h"


//���[�v�̊�{�N���X
class Wire : public Base {
private:
	CModelObj m_model;

	RenderTask m_render;
public:

	float rope_length;

	CVector3D lineS;
	CVector3D lineE;

	//�e�̃��[�v�|�C���^�[
	Wire* m_parent_wire;

	CMatrix m_top_matrix;
	CMatrix m_bottom_matrix;

	CCapsule m_capsule;

public:
	float ang_rope;

	float m_rad;
public:
	Wire(const Transform& transform, float length, Wire* wire = nullptr);
	void Update();
	void Render();
};