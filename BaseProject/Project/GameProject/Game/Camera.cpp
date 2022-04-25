#include "Camera.h"
#include"NonAnimObject.h"

Camera* Camera::mp_instance = nullptr;

Camera::Camera() : Base(UpdatePriority::eUp_Camera,"Camera"),mp_camera(nullptr)
{
	SetProtect(true);

	m_render.RegistRender(this, (RenderFunc)&Camera::Render, RenderPriority::eRd_Camera, "Camera");

	m_col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Camera::CollisionCheck, 
		CollisionPriority::eCol_Camera,"Camera");
}

Camera* Camera::GetInstance()
{
	if (!mp_instance) {
		mp_instance = new Camera();
		return mp_instance;
	}
	else return mp_instance;
}

void Camera::Update()
{
	if (mp_camera)
		mp_camera->UpdateCamera();
}

void Camera::Render()
{
	
}

void Camera::SetCamera(ICamera* c)
{
	mp_camera = c;
}

void Camera::CollisionCheck(CollisionTask* task)
{
	if (task->GetPriority() != CollisionPriority::eCol_Field)return;

	if (NonAnimObject* b = dynamic_cast<NonAnimObject*> (task->GetTask())) {


		CVector3D cam_pos = CCamera::GetCurrent()->GetPos();
		CVector3D cam_at = CCamera::GetCurrent()->GetAt();

		CVector3D cross, normal;
		if (b->GetModel().CollisionRay(&cross, &normal, cam_at, cam_pos)) {

			cam_pos = cross;

			CCamera::GetCurrent()->LookAt(cam_pos, cam_at, CVector3D(0, 1, 0));

		}
	}
}
