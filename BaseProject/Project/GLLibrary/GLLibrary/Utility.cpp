#include "Utility.h"
#include "CCamera.h"
#include "CShader.h"

void Utility::ScreenToWorld(CVector3D *out, const CVector3D &spos, const CMatrix &mProj,const CMatrix &mView, const CRect& viewport){
	CMatrix mViewport;
	mViewport.Viewport(viewport.m_x, viewport.m_y, viewport.m_width, viewport.m_height);
	CVector4D o = (mView.GetInverse() * mProj.GetInverse() * mViewport.GetInverse()) * CVector4D(spos.x, spos.y -GL::window_height + viewport.m_y+ viewport.m_height, spos.z, 1);
	*out = CVector3D(o.x/o.w,o.y/o.w,o.z/o.w);
	
}
void Utility::ScreenToWorld(CVector3D *out, const CVector3D &spos) {
	ScreenToWorld(out, spos, CCamera::GetCurrent()->GetProjectionMatrix(), CCamera::GetCurrent()->GetViewMatrix(), CCamera::GetCurrent()->GetViewportRect());
}

void Utility::ScreenToWorld(CVector3D *out, const CVector3D &spos, const CCamera* camera) {
	ScreenToWorld(out, spos, camera->GetProjectionMatrix(), camera->GetViewMatrix(), camera->GetViewportRect());
}

void Utility::WorldToScreen(CVector3D *out, const CVector3D &spos, const CMatrix &mProj, const CMatrix &mView, const CRect& viewport) {
	CMatrix mViewport;
//	mViewport.Viewport(viewport.m_x, viewport.m_y, viewport.m_width, viewport.m_height);
	mViewport.Viewport(0, 0, viewport.m_width, viewport.m_height);
	CVector4D o = mViewport * mProj * mView * CVector4D(spos.x,spos.y,spos.z,1);

	*out =  CVector3D(o.x/o.w, o.y/o.w,o.z/o.w);

}
void Utility::WorldToScreen(CVector3D *out, const CVector3D &spos) {
	WorldToScreen(out, spos, CCamera::GetCurrent()->GetProjectionMatrix(), CCamera::GetCurrent()->GetViewMatrix(), CCamera::GetCurrent()->GetViewportRect());
}
void Utility::WorldToScreen(CVector3D *out, const CVector3D &spos, const CCamera* camera) {
	WorldToScreen(out, spos, camera->GetProjectionMatrix(), camera->GetViewMatrix(), camera->GetViewportRect());
}
void Utility::WorldToScreen(CVector2D *out, const CVector3D &spos) {
	CVector3D p;
	WorldToScreen(&p, spos, CCamera::GetCurrent()->GetProjectionMatrix(), CCamera::GetCurrent()->GetViewMatrix(), CCamera::GetCurrent()->GetViewportRect());
	*out = CVector2D(p.x, p.y);
}


void Utility::DrawLine(const CVector3D &s, const CVector3D &e, const  CVector4D &color) {
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix();
	
	const CVector3D vertex[2] = { s,e };
	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);
	
	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, &vertex);


	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(CShader::eVertexLocation);
	

	shader->disable();




}
static const SVector3D cube_points[] = {
	-0.5f,-0.5f,-0.5f,  //v0
	 0.5f,-0.5f,-0.5f,  //v1
	 0.5f, 0.5f,-0.5f,  //v2
	-0.5f, 0.5f,-0.5f,  //v3

	-0.5f,-0.5f,0.5f, //v4
	 0.5f,-0.5f,0.5f, //v5
	 0.5f, 0.5f,0.5f, //v6
	-0.5f, 0.5f,0.5f, //v7
};
static const GLuint cube_index[] = {
	0,1,2,3,
	1,5,6,2,
	5,4,7,6,
	4,0,3,7,
	4,5,1,0,
	3,2,6,7
};

static const SVector3D quad_points[] = {
	0,0,0,  //v0
	1.0f,0,0,  //v1
	1.0f,1.0f,0,  //v2
	0,1.0f,0,  //v3
};
void Utility::DrawCube(const CVector3D &pos, const float size, const CVector4D &color) {

	//�J�����O����
	glDisable(GL_CULL_FACE); 
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix() * CMatrix::MTranselate(pos)*CMatrix::MScale(size, size, size);

	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, &cube_points);
	
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, cube_index);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->disable();

	glEnable(GL_CULL_FACE);
}
void Utility::DrawCube(const CMatrix &mat, const float size, const CVector4D &color) {
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix() * mat*CMatrix::MScale(size, size, size);

	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, &cube_points);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, cube_index);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->disable();
}
void Utility::DrawLine(const CVector2D &s, const CVector2D &e, const CVector4D &color) {
	

	//�J�����O����
	glDisable(GL_CULL_FACE);
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glDisable(GL_DEPTH_TEST);
	//
	//���˓��e�̍s����쐬
	//
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();


	const CVector3D vertex[2] = { s,e };
	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, mProj.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, &vertex);


	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->disable();

	
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_LIGHTING);
	
	glEnable(GL_DEPTH_TEST);




}
void Utility::DrawQuad(const CVector2D &pos, const CVector2D &size, const CVector4D &color) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//�e��@�\�𖳌���
	//�J�����O����
	glDisable(GL_CULL_FACE);
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glDisable(GL_DEPTH_TEST);
	//
	//���˓��e�̍s����쐬
	//
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();


	CMatrix pvm = mProj *CMatrix::MTranselate(pos)* CMatrix::MScale(size.x, size.y, 1);
	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, &quad_points);

	glDrawArrays(GL_QUADS,0, 4);


	glDisableVertexAttribArray(CShader::eVertexLocation);

	shader->disable();

	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	glPopAttrib();

}

void Utility::DrawSphere(const CVector3D &pos, const float size, const CVector4D &color) {
	DrawSphere(CMatrix::MTranselate(pos), size, color);
}

void Utility::DrawSphere(const CMatrix &mat, const float size, const CVector4D &color) {
	const int cut = 12;
	CVector3D* vertex = new CVector3D[cut*cut*4];
	float *s = new float[cut+1];
	float *c = new float[cut+1];
	for (int i = 0; i <= cut; ++i) {
		s[i] = sin(M_PI * 2 * i / cut);
		c[i] = cos(M_PI * 2 * i / cut);
	}

	int idx = 0;
	for (int i = 0; i < cut; ++i) {
		const float& is1 = s[i];
		const float& is2 = s[i+1];
		const float& ic1 = c[i];
		const float& ic2 = c[i+1];
		for (int j = 0; j < cut; ++j) {
			const float& js1 = s[j];
			const float& js2 = s[j+1];
			const float& jc1 = c[j];
			const float& jc2 = c[j+1];

			vertex[idx] = CVector3D(js1*is1, ic1, jc1*is1);
			vertex[idx + 1] = CVector3D(js2*is1, ic1, jc2*is1);
			vertex[idx + 2] = CVector3D(js2*is2, ic2, jc2*is2);
			vertex[idx + 3] = CVector3D(js1*is2, ic2, jc1*is2);
			idx += 4;
		}
	}
	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix() * mat*CMatrix::MScale(size, size, size);

	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, vertex);

	glDrawArrays(GL_QUADS,0,cut*cut*4);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->disable();

	delete[] vertex;
	delete[] s;
	delete[] c;
}
void Utility::DrawCircle(const CVector2D & pos, const float size, const CVector4D & color)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	//�e��@�\�𖳌���
	//�J�����O����
	glDisable(GL_CULL_FACE);
	//���C�e�B���O����
	glDisable(GL_LIGHTING);
	//�f�v�X�e�X�g����
	glDisable(GL_DEPTH_TEST);
	//
	//���˓��e�̍s����쐬
	//
	const CMatrix& mProj = CCamera::GetCurrent()->Get2DProjectionMatrix();

	const int cut = 8;
	CVector3D* vertex = new CVector3D[cut + 2];
	int idx = 0;
	vertex[0] = CVector3D(0, 0, 0);
	idx++;
	for (int i = 0; i <= cut; ++i,++idx) {
		float s = sin(M_PI * 2 * i / cut);
		float c = cos(M_PI * 2 * i / cut );
		vertex[idx] = CVector3D(s, c, 0);
	}


	CMatrix pvm = mProj * CMatrix::MTranselate(pos)* CMatrix::MScale(size, size, 1);
	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();


	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, vertex);

	glDrawArrays(GL_TRIANGLE_FAN, 0, cut+2);


	glDisableVertexAttribArray(CShader::eVertexLocation);

	shader->disable();

	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);

	glPopAttrib();

	delete[] vertex;

}
void Utility::DrawCapsule(const CVector3D &s, const CVector3D &e, const float rad, const CVector4D &color) {
	DrawSphere(s, rad, color);
	DrawSphere(e, rad, color);
	

	CMatrix pvm = CCamera::GetCurrent()->GetProjectionMatrix() * CCamera::GetCurrent()->GetViewMatrix();

	CShader* shader = CShader::GetInstance("Solid");
	shader->enable();
	const int cut = 16;
	float l = (e - s).Length();
	CVector3D f = (e - s).GetNormalize();
	CVector3D u,r;
	if (float d = abs(CVector3D::Dot(f, CVector3D(0, 1, 0))) > 0.99f) {
		u = CVector3D::Cross(f, CVector3D(0, 0, d > 0 ? 1:-1)).GetNormalize();
	} else {
		u = CVector3D::Cross(f, CVector3D(0, 1, 0)).GetNormalize();
	}
	r = CVector3D::Cross(u, f).GetNormalize();
	CMatrix mat(r.x, u.x, f.x, 0,
				r.y, u.y, f.y, 0,
				r.z, u.z, f.z, 0,
				0, 0, 0, 1);

	CVector3D *vertex = new CVector3D[(cut+1)*2];
	for (int i = 0; i <= cut; i++) {
		CVector3D n = mat*CMatrix::MRotationZ(i*M_PI * 2 / cut)*CVector3D(0, 1, 0);
		CVector3D ee = e + n * rad;

		vertex[i*2] = CVector3D(ee.x, ee.y, ee.z);
		CVector3D ss = s + n * rad;
		vertex[i * 2+1] = CVector3D(ss.x, ss.y, ss.z);

	}

	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "PVWMatrix"), 1, GL_FALSE, pvm.f);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "Color"), 1, color.v);

	glEnableVertexAttribArray(CShader::eVertexLocation);

	glVertexAttribPointer(CShader::eVertexLocation, 3, GL_FLOAT, GL_FALSE, 0, vertex);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (cut+1) * 2);


	glDisableVertexAttribArray(CShader::eVertexLocation);


	shader->disable();
	delete[] vertex;




}

CVector3D Utility::GetInputKeyDir()
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
void Utility::DebugLocalDir(CVector3D pos, CVector3D rot)
{
	CMatrix m = CMatrix::MRotation(rot);

	//�L�����N�^�[�̃��[�J����xyz����
	Utility::DrawLine(pos, pos + m.GetLeft(), CVector4D(1, 0, 0, 1.0f));
	Utility::DrawLine(pos, pos + m.GetUp(), CVector4D(0, 1, 0, 1.0f));
	Utility::DrawLine(pos, pos + m.GetFront(), CVector4D(0, 0, 1, 1.0f));
}
bool Utility::IsViewInside(const CVector3D& view_pos, const CVector3D& view_rot, const CVector3D& target_pos, float view_ang, float view_length)
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
int Utility::IsPlusMinus()
{
	int i = Rand(0, 2);
	if (i == 0) {
		return 1;
	}
	else return -1;
}
void Utility::DrawViewPoint(CVector3D view_point, CVector3D view_rot, float view_ang, float view_length)
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


const CVector3D& Utility::RandVec3D(float min,float max){

	return CVector3D(Utility::Rand(min,max),Utility::Rand(min,max),Utility::Rand(min,max));

}


bool Utility::CollisionSphere(const CVector3D& a_center,float a_rad,
	const CVector3D& b_center, float b_rad, CVector3D* push_back)
{

	CVector3D dir = a_center - b_center;
	float length = dir.Length();
	float rad = a_rad + b_rad;

	if (length < rad) {

		float power = rad - length;

		//�����߂�
		if(push_back) (*push_back) = dir.GetNormalize() * power;

		return true;

	}
	else return false;

}

bool Utility::CollisionCapsuleSphere(const CVector3D& c_top, const CVector3D& c_bottom, float c_rad, const CVector3D& s_center, float s_rad, CVector3D* push_back)
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

			if(push_back)(*push_back) = dir * s;
		}

	}else return false;
}
/*
bool Utility::CollisionCapsule(const CVector3D& a_top, const CVector3D& a_bottom,float a_rad,
	const CVector3D& b_top, const CVector3D& b_bottom,float b_rad,CVector3D* push_back)
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
		
	}

	if (length < a_rad + b_rad) {
		return true;
	}else return false;
}
*/


//
//void Utility::DrawCapsule(CVector2D & s, CVector2D &e, float size, CVector4D & color)
//{
//	glPushAttrib(GL_ALL_ATTRIB_BITS);
//	//�e��@�\�𖳌���
//	//�J�����O����
//	glDisable(GL_CULL_FACE);
//	//���C�e�B���O����
//	glDisable(GL_LIGHTING);
//	//�f�v�X�e�X�g����
//	glDisable(GL_DEPTH_TEST);
//	//
//	//���˓��e�̍s����쐬
//	//
//	CMatrix	mProj = CMatrix(
//		2.0f / CCamera::GetCurrent()->GetWhidth(), 0.0f, 0.0f, -1.0f,
//		0.0f, -2.0f / CCamera::GetCurrent()->GetHeight(), 0.0f, 1.0f,
//		0.0f, 0.0f, 1.0f, 0.0f,
//		0.0f, 0.0f, 0.0f, 1.0f
//	);
//
//	//���e�s����p�̕��ɐݒ�
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadMatrixf(mProj.f);
//
//	//���f�����r���[�s����Đݒ�
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//
//	glLoadIdentity();
//	//�J���[�ݒ�
//	glColor4fv(color.v);
//
//	CVector2D v = e - s;
//	CVector2D dir = v.GetNormalize();
//	CVector3D cross = CVector3D::Cross(CVector3D(dir.x, dir.y, 0), CVector3D(0, 0, 1));
//	CVector2D c = CVector2D(cross.x, cross.y);
//	CVector2D *p[2] = { &s,&e };
//	float ang = atan2(c.x, c.y);
//
//	glDisable(GL_CULL_FACE);
//	glCullFace(GL_NONE);
//	glBegin(GL_QUADS);
//	glVertex2fv(CVector2D(s + c * size).v);
//	glVertex2fv(CVector2D(s + c * -size).v);
//	glVertex2fv(CVector2D(e + c * -size).v);
//	glVertex2fv(CVector2D(e + c * size).v);
//	glEnd();
//
//	for (int i = -1, idx = 0; i < 2; i += 2, idx++) {
//		glBegin(GL_TRIANGLE_FAN);
//		glVertex2fv(CVector2D(*p[idx]).v);
//		for (int j = 0; j <= 8; j++) {
//			float a = ang + j * M_PI / 8;
//			glVertex2fv(CVector2D(*p[idx] + CVector2D(sin(a), cos(a))*(-i * size)).v);
//		}
//		glEnd();
//	}
//	//���̍s��Ɛݒ�ɖ߂�
//	glPopMatrix();
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//
//
//	glPopAttrib();
//
//
//
//}
