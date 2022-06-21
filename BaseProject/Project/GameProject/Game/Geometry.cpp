#include "Geometry.h"


Geometry::Geometry(const Transform& transform,const char* model_name):
	StaticMeshObject(UpdatePriority::eUp_Field,"Geometry" ,model_name, RenderPriority::eRd_Field)
{
	m_Col.RegistCollision(CollisionTask::eLayer_Game, this, (CollisionFunc)&Geometry::CollisionCheck, CollisionPriority::eCol_Field, model_name);

	SetTransform(transform);
	m_Model.SetPos(transform.position);
	m_Model.SetRot(transform.rotation);
	m_Model.SetScale(transform.scale);
	m_Model.UpdateMatrix();
}

void Geometry::CollisionCheck(CollisionTask* _task)
{
}
