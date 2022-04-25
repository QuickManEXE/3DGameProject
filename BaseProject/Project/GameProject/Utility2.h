#pragma once
#include "stdafx.h"


namespace Utility2 {

	//������
	//�ړ��L�[���͎擾
	CVector3D GetInputKeyDir();
	//�}�E�X�̈ړ��Ŏ��_�ړ�


	//���[�J���x�N�g���\��
	void DebugLocalDir(CVector3D pos, CVector3D rot);
	//target��viewpoint�̎���p�Ɣ͈͂ɓ����Ă��邩����
	bool IsViewInside(const CVector3D& view_pos, const CVector3D& view_rot,
		const CVector3D& target_pos, float view_ang, float view_length);
	//�����_���Ńv���X���}�C�i�X�����߂ĕԂ�
	int IsPlusMinus();
	//����͈͂̊ȈՕ\��
	void DrawViewPoint(CVector3D view_point, CVector3D view_rot, float view_ang, float view_length);
	//3D�x�N�g���������_���ŕԂ�
	const CVector3D& RandVec3D(float min, float max);
	//�����蔻��

	//�~���m�̔���B����A�ւ̗͂�Ԃ�
	bool CollisionSphere(const CVector3D& a_center, float a_rad,
		const CVector3D& b_center, float b_rad, CVector3D* push_back);


	bool CollisionCapsuleSphere(const CVector3D& c_top, const CVector3D& c_bottom, float c_rad,
		const CVector3D& s_center, float s_rad, CVector3D* push_back);


	//�J�v�Z�����m�̔���B����A�ւ̗͂�Ԃ�
	bool CollisionCapsule(const CVector3D& a_top, const CVector3D& a_bottom, float a_rad,
		const CVector3D& b_top, const CVector3D& b_bottom, float b_rad, CVector3D* push_back);
}