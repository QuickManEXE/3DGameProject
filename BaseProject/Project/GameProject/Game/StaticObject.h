#pragma once
#include".../../../Base/Base.h"

class StaticObject : public Base {
protected:

	CModelObj m_model;

	RenderTask m_render;

	CollisionTask m_collision;

	CSphere m_sphere;

	CCapsule m_capsule;
public:
	StaticObject(const Transform& transform, const char* model_name,float rad,float height);
	virtual void Update()override;
	virtual void Render();
	void DebugInfo();
	virtual void CollisionCheck(CollisionTask* task);
};