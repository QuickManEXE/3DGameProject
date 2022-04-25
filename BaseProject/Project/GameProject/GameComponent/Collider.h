#ifndef INCLUDE_COLLIDER_GAMEPARTS
#define INCLUDE_COLLIDER_GAMEPARTS
#include "../stdafx.h"

class CircleCollider {
public:

	float m_rad;
	CVector3D m_pos;

public:
	CircleCollider(const CVector3D _pos,float _rad);
};
#endif // !INCLUDE_COLLIDER_GAMEPARTS