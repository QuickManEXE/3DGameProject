#ifndef INCLUDE_TRANSFORM_GAMEPARTS
#define INCLUDE_TRANSFORM_GAMEPARTS

#include "../stdafx.h"

class Transform {
public:
	//���W
	CVector3D position;
	//��]
	CVector3D rotation;
	//�傫��
	CVector3D scale;
	//�ړ��x�N�g��
	CVector3D m_pos_vec;

	CVector3D m_rot_vec;

	CVector3D m_front;

	CVector3D m_left;

	CVector3D m_up;
	//
	CVector3D m_dir;
public:
	Transform(const CVector3D& position = CVector3D::zero,
			  const CVector3D& rotation = CVector3D::zero, 
		      const CVector3D& size = CVector3D::one);
	/*
	���[�J�����W�ňړ�����
	*/
	void Translate(const CVector3D& _translation);
	/*
	���[���h���W�ňړ�����
	*/
	void Rotate(float x, float y, float z);
	void Rotate(const CVector3D& _rotation);
	void LookAt(const Transform& _transform);
};
#endif // !INCLUDE_TRANSFORM_GAMEPARTS