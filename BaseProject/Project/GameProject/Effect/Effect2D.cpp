#include "Effect2D.h"

Effect2D::Effect2D(const char* _image,
	const CVector2D& _pos,
	const CVector2D& _vec,
	const CVector2D& _vec_change,
	float _rot,
	float _rot_change,
	CRect _size,
	float _size_change,
	float _alpha,
	float _alpha_change,
	float _time,
	float _time_change)
	:Base(UpdatePriority::eUp_Effect, "Effect"),
	m_VecChange(_vec_change),
	m_RotChange(_rot_change),
	m_SizeChange(_size_change),
	m_Alpha(_alpha),
	m_AlphaSpeed(_alpha_change),
	m_Time(_time),
	m_TimeSpeed(_time_change)
{
	m_image = COPY_RESOURCE(_image, CImage);
	m_Transform.position = _pos;
	m_Transform.m_pos_vec = _vec;
	m_Rot = _rot;
	m_Size = _size;

	m_Render.RegistRender(this, (RenderFunc)&Effect2D::Render, RenderPriority::eRd_Effect, "Effect");
}


Effect2D::~Effect2D()
{

}

void Effect2D::Update() {

	m_Transform.m_pos_vec += m_VecChange * DELTA;
	m_Transform.position += m_Transform.m_pos_vec * DELTA;
	m_Rot += m_RotChange * DELTA;
	//m_size += m_size_change * DELTA;
	m_Alpha += m_AlphaSpeed * DELTA;
	m_Time -= m_TimeSpeed * DELTA;
	

	if (m_Time <= 0) SetKill();

}

void Effect2D::Render() {

	m_image.SetPos(m_Transform.position); //ˆÊ’uÝ’è
	m_image.SetColor(1, 1, 1, m_Alpha);
	m_image.SetAng(m_Rot);
	m_image.SetSize(m_Size.m_width,m_Size.m_height);
	//m_image.SetRect(m_size.m_left, m_size.m_top, m_size.m_right, m_size.m_bottom);
	m_image.Draw();
}