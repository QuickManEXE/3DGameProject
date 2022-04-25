#include "CRect.h"


CLine::CLine() : m_start_x(0), m_start_y(0), m_start_z(0), m_end_x(0), m_end_y(0), m_end_z(0) {}

CLine::CLine(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z)
	: m_start_x(s_x), m_start_y(s_y), m_start_z(s_z), m_end_x(e_x), m_end_y(e_y), m_end_z(e_z) {}

CLine::CLine(const CVector3D& s_pos, const CVector3D& e_pos, float rad)
	: m_start_x(s_pos.x), m_start_y(s_pos.y), m_start_z(s_pos.z), m_end_x(e_pos.x), m_end_y(e_pos.y), m_end_z(e_pos.z) {}

void CLine::Set(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z)
{
	this->m_start_x = s_x; this->m_start_y = s_y; this->m_start_z = s_z;
	this->m_end_x = e_x; this->m_end_y = e_y; this->m_end_z = e_z;
}

void CLine::Set(const CVector3D& s_pos, const CVector3D& e_pos)
{
	Set(s_pos.x, s_pos.y, s_pos.z, e_pos.x, e_pos.y, e_pos.z);
}


CRect::CRect() :  m_left(0), m_top(0), m_right(0), m_bottom(0), m_pos(0, 0), m_size(0, 0)
{
}

CRect::CRect(float l, float t, float r, float b): m_left(l), m_top(t), m_right(r), m_bottom(b), m_pos(l, t), m_size(r-l, b-t)
{
}

CRect::CRect(const CVector2D& pos, const CVector2D& size): m_left(pos.x), m_top(pos.y), m_right(pos.x+size.x), m_bottom(pos.y + size.y), m_pos(pos), m_size(size)
{
}

CRect::CRect(const CRect& rect): m_left(rect.m_left), m_top(rect.m_top), m_right(rect.m_right), m_bottom(rect.m_bottom), m_pos(rect.m_pos), m_size(rect.m_size)
{
}

CRect CRect::operator=(CRect& rect)
{

	m_left = rect.m_left;
	m_top = rect.m_top;
	m_right = rect.m_right;
	m_bottom = rect.m_bottom;
	m_pos = rect.m_pos;
	m_size = rect.m_size;
	return *this;
}

void CRect::Set(float l, float t, float r, float b)
{

	m_left = l;
	m_top = t;
	m_right = r;
	m_bottom = b;
	m_pos.x = l;
	m_pos.y = t;
	m_size.x = r - l;
	m_size.y = b - t;
}

void CRect::Set(const CVector2D& pos, const CVector2D& size)
{
	m_pos = pos;
	m_size = size;
	m_left = m_pos.x;
	m_top = m_pos.y;
	m_right = m_pos.x+size.x;
	m_bottom = m_pos.y + size.y;
}

CSphere::CSphere() : m_x(0), m_y(0), m_z(0), m_rad(0) {}

CSphere::CSphere(float x, float y, float z, float rad) : m_x(x), m_y(y), m_z(z), m_rad(rad) {}

CSphere::CSphere(const CVector3D& pos, float rad) : m_x(pos.x),m_y(pos.y),m_z(pos.z), m_rad(rad){}

void CSphere::Set(float x, float y, float z, float rad)
{
	m_x = x; m_y = y; m_z = z; this->m_rad = rad;
}

void CSphere::Set(CVector3D pos, float rad)
{
	Set(pos.x, pos.y, pos.z, rad);
}

CCapsule::CCapsule() : m_rad(0) {}

CCapsule::CCapsule(float s_x, float s_y, float s_z, float e_x, float e_y, float e_z, float rad)
	:CLine(s_x, s_y, s_z, e_x, e_y, e_z), m_rad(rad) {}

CCapsule::CCapsule(const CVector3D& s_pos, const CVector3D& e_pos, float rad)
	: CLine(s_pos.x, s_pos.y, s_pos.z, e_pos.x, e_pos.y, e_pos.z), m_rad(rad){}

void CCapsule::SetRad(float rad)
{
	this->m_rad = rad;
}
