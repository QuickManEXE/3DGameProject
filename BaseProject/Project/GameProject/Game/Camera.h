#pragma once
#include "../Base/Base.h"

class ICamera {
public:
	virtual void UpdateCamera() = 0;
};

class Camera : public Base {
private:
	static Camera* mp_instance;
	Camera();
private:
	ICamera* mp_camera;

	RenderTask m_render;

	CollisionTask m_col;
public:
	static Camera* GetInstance();
	void Update();
	void Render();
	void SetCamera(ICamera* c);
	void CollisionCheck(CollisionTask* task);
};