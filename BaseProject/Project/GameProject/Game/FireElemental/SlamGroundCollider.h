#pragma once
#include"../../Base/Base.h"

class SlamGroundCollider : public Base
{
private:
	RenderTask m_Render;

	CollisionTask m_Col;

	float m_limit_time;

	float m_rad;
public:
	SlamGroundCollider(UpdatePriority priority,const CVector3D& pos,float rad,float time);
	~SlamGroundCollider();
	void Update()override;
	void Render();
	void CollisionCheck(CollisionTask* task);
};