#include"Utility2.h"

CVector3D Utility2::GetInputKeyDir()
{
	//�����L�[�̓��͕����x�N�g��
	CVector3D key_dir(0, 0, 0);

	//�����L�[������͕����x�N�g����ݒ�
	if (CInput::GetState(0, CInput::eHold, CInput::eUp)) key_dir.z = 1;
	if (CInput::GetState(0, CInput::eHold, CInput::eDown)) key_dir.z = -1;
	if (CInput::GetState(0, CInput::eHold, CInput::eLeft)) key_dir.x = 1;
	if (CInput::GetState(0, CInput::eHold, CInput::eRight)) key_dir.x = -1;

	return key_dir;
}
void Utility2::DebugLocalDir(CVector3D pos, CVector3D rot)
{
	CMatrix m = CMatrix::MRotation(rot);

	//�L�����N�^�[�̃��[�J����xyz����
	Utility::DrawLine(pos, pos + m.GetLeft(), CVector4D(1, 0, 0, 1.0f));
	Utility::DrawLine(pos, pos + m.GetUp(), CVector4D(0, 1, 0, 1.0f));
	Utility::DrawLine(pos, pos + m.GetFront(), CVector4D(0, 0, 1, 1.0f));
}
bool Utility2::IsViewInside(const CVector3D& view_pos, const CVector3D& view_rot, const CVector3D& target_pos, float view_ang, float view_length)
{
	CVector3D dir = target_pos - view_pos;

	float length = dir.Length();

	float ang = acos(CVector3D::Dot(dir.GetNormalize(), CMatrix::MRotation(view_rot).GetFront()));

	if (Utility::NormalizeAngle(ang) < view_ang && length < view_length) {

		//�^��Ԃ�
		return true;

	}
	//�U��Ԃ�
	return false;
}
int Utility2::IsPlusMinus()
{
	int i = Utility::Rand(0, 2);
	if (i == 0) {
		return 1;
	}
	else return -1;
}
void Utility2::DrawViewPoint(CVector3D view_point, CVector3D view_rot, float view_ang, float view_length)
{
	CVector3D pos = view_point; CVector3D rot = view_rot;
	float ang = view_ang; float length = view_length;


	CVector3D array[] = {
		CVector3D(0, 0, 0),
		CVector3D(0, ang, 0),
		CVector3D(0, -ang, 0),
		CVector3D(ang,0 , 0),
		CVector3D(-ang,0 , 0),
	};

	for (int i = 0; i < 5; i++) {

		Utility::DrawLine(pos,
			pos + CMatrix::MRotation(rot + array[i]).GetFront() * length
			, CVector4D(1, 1, 0, 1));

	}
}


const CVector3D& Utility2::RandVec3D(float min, float max) {

	return CVector3D(Utility::Rand(min, max), Utility::Rand(min, max), Utility::Rand(min, max));

}


bool Utility2::CollisionSphere(const CVector3D& a_center, float a_rad,
	const CVector3D& b_center, float b_rad, CVector3D* push_back)
{

	CVector3D dir = a_center - b_center;
	float length = dir.Length();
	float rad = a_rad + b_rad;

	if (length < rad) {

		float power = rad - length;

		//�����߂�
		if (push_back) (*push_back) = dir.GetNormalize() * power;

		return true;

	}
	else return false;

}

bool Utility2::CollisionCapsuleSphere(const CVector3D& c_top, const CVector3D& c_bottom, float c_rad, const CVector3D& s_center, float s_rad, CVector3D* push_back)
{
	//���e�p�̐���
	CVector3D p = s_center - c_top;
	//�����̃x�N�g��
	CVector3D line_dir = c_bottom - c_top;
	//�ŋߓ_�܂ł̒���
	float d_len = CVector3D::Dot(p, line_dir.GetNormalize());


	if (d_len < 0)
		d_len = 0;
	if (d_len > line_dir.Length())
		d_len = line_dir.Length();

	CVector3D C(c_top + line_dir.GetNormalize() * d_len);

	//�_�Ɛ����Ƃ̋���
	float	length = (s_center - C).Length();


	//���ƃJ�v�Z�����Փ˂��Ă����
	if (length < c_rad + s_rad) {


		{
			//�����߂�
			CVector3D dir = (s_center - C).GetNormalize();
			float s = s_rad + c_rad - length;

			if (push_back)(*push_back) = dir * s;
		}

	}
	else return false;
}

bool Utility2::CollisionCapsule(const CVector3D& a_top, const CVector3D& a_bottom, float a_rad,
	const CVector3D& b_top, const CVector3D& b_bottom, float b_rad, CVector3D* push_back)
{
	//�����Ɛ����̍ŒZ����
	float length = 0;

	//line1S��line1E�̃x�N�g��
	CVector3D vec1SE = a_bottom - a_top;
	//line2S��line2E�̃x�N�g��
	CVector3D vec2SE = b_bottom - b_top;
	//�Q�̐����ƒ��p�ɂȂ�x�N�g��
	CVector3D cross = CVector3D::Cross(vec1SE, vec2SE);
	//�Q�̐����Ɛ����ȕ���
	CVector3D dir = cross.GetNormalize();

	//�����`�F�b�N
	//�����Q������P�Ɠ������ꕽ�ʏ�ɍ��킹��
	float l = CVector3D::Dot(dir, a_top - b_top);
	CVector3D m = dir * l;
	CVector3D S2 = a_bottom + m;
	CVector3D E2 = b_bottom + m;

	//lineS1��S2�̃x�N�g��
	CVector3D vecS1S2 = S2 - a_top;
	//lineS1��E2�̃x�N�g��
	CVector3D vecS1E2 = E2 - a_top;
	//S2��lineS1�̃x�N�g��
	CVector3D vecS2S1 = a_top - S2;
	//S2��lineE1�̃x�N�g��
	CVector3D vecS2E1 = a_bottom - S2;

	//�Q�̐������������Ă��邩���ׂ�
	float d1 = CVector3D::Dot(CVector3D::Cross(vec1SE, vecS1S2), CVector3D::Cross(vec1SE, vecS1E2));
	float d2 = CVector3D::Dot(CVector3D::Cross(vec2SE, vecS2S1), CVector3D::Cross(vec2SE, vecS2E1));

	//�������Ă���
	if (d1 < 0 &&
		d2 < 0) {
		//�������m�̋����͐����P�̎n�_��������Q�̎n�_�ւ̃x�N�g����
		CVector3D vecS1S2 = a_top - b_top;
		//�Q�̐����Ɛ����ȕ���dir�֓��e��������
		//���̐��ɂȂ邱�Ƃ�����̂Ő�Βl�ŋ��߂�
		length = abs(CVector3D::Dot(vecS1S2, dir));

	}
	else {
		//�������Ă��Ȃ�
		//�e�����̎n�_�A�I�_�ƕЕ��̐����Ƃ̋����ōł��Z�������𒲂ׂ�
		//�����P�̎n�_�ƁA�����Q�Ƃ̋���
		/*
		float l1s = CCollision::DistancePointToLine(line2S, line2E, line1S);
		//�����P�̏I�_�ƁA�����Q�Ƃ̋���
		float l1e = CCollision::DistancePointToLine(line2S, line2E, line1E);
		//�����Q�̎n�_�ƁA�����P�Ƃ̋���
		float l2s = CCollision::DistancePointToLine(line1S, line1E, line2S);
		//�����Q�̏I�_�ƁA�����P�Ƃ̋���
		float l2e = CCollision::DistancePointToLine(line1S, line1E, line2E);
		//��ԒZ���̂��������m�̋���
		length = min(min(l1s, l1e), min(l2s, l2e));
		*/
	}

	if (length < a_rad + b_rad) {
		return true;
	}
	else return false;
}
