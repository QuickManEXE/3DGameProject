#include "Transform.h"


Transform::Transform(const CVector3D& position, const CVector3D& rotation, const CVector3D& size)
: position(position), rotation(rotation), scale(size), m_pos_vec(CVector3D::zero),m_rot_vec(CVector3D::zero),m_dir(CVector3D::front)
{
}

void Transform::Translate(const CVector3D& translation)
{ 
	position += translation;
}

void Transform::Rotate(float x,float y,float z) 
{ 
	this->rotation.x += x;this->rotation.y += y;this->rotation.z += z;
}

void Transform::Rotate(const CVector3D& rotation)
{ 
	Rotate(rotation.x,rotation.y,rotation.z);
}

void Transform::LookAt(const Transform& transform)
{
	CVector3D v = transform.position - position;
}
